#pragma once
#include <string>
#include <atomic>
#include <mutex>
#include <grpc++/grpc++.h>

#include "yevent.grpc.pb.h"
#include <chrono>
#include "rx.hpp"
#include <sstream>
#include "Threadpool.h"
#include "rx-strand.h"
#include "rx-asio.hpp"
#include "Delegate.h"
#include <type_traits>
#include <ppl.h>

#ifdef GRPC_CLIENT_EXPORT
#define GRPC_CLIENT_EXPORT_API __declspec( dllexport )   
#elif defined(GRPC_CLIENT_SERVICE_STATIC)
#define GRPC_CLIENT_EXPORT_API    
#else
#define GRPC_CLIENT_EXPORT_API __declspec( dllimport )   
#endif

class MainWorkerClient;

const std::string MainWorkerServerIP = "127.0.0.1:37001";
extern std::mutex mm_mutex;


typedef MainWorkerClient ClientType;
typedef std::atomic<ClientType*> ClientPtr;
extern ClientPtr clientPtr;
GRPC_CLIENT_EXPORT_API ClientType* getClientInstance();
GRPC_CLIENT_EXPORT_API void reset();


inline std::string thread_to_str(std::thread::id myid) {
	std::stringstream ss;
	ss << myid;
	return ss.str();
}

// struct for keeping state and data information
template <class RespType>
struct AsyncClientCall {
	// Container for the data we expect from the server.
	RespType reply;

	// Context for the client. It could be used to convey extra information to
	// the server and/or tweak certain RPC behaviors.
	grpc::ClientContext context;

	grpc::CompletionQueue* cq;

	// Storage for the status of the RPC upon completion.
	grpc::Status status;

	std::unique_ptr<grpc::ClientAsyncResponseReader<RespType>> response_reader;
	
};

template< typename T >
struct my_deleter
{
	void operator ()(T const * p)
	{
		Free((void*)p);
	}
};


template <class EVENT, class RESPTYPE>
struct async_delegate {
	typedef SA::delegate< std::unique_ptr< ::grpc::ClientAsyncResponseReader< RESPTYPE>>(::grpc::ClientContext* , const EVENT& , ::grpc::CompletionQueue*) > type;
};

template <class EVENT, class RESPTYPE, class T>
struct async_func {
	using type = std::unique_ptr< ::grpc::ClientAsyncResponseReader< RESPTYPE>>(T::*)(::grpc::ClientContext*, const EVENT&, ::grpc::CompletionQueue* );
};

#define DefineFunc(func,ReqType,RespType) \
	::grpc::Status func(::grpc::ClientContext* context, const ReqType& request, RespType* response){	\
		return stub_->##func (context,request,response);	\
	};	\
	\
	std::unique_ptr< ::grpc::ClientAsyncResponseReader< RespType >> Async##func (::grpc::ClientContext* context, const ReqType& request, ::grpc::CompletionQueue* cq){	\
		return stub_->Async##func (context,request,cq);	\
	}

#define DefineDelegate(func,ReqType,RespType) 	\
	using func##DelegateType = async_delegate<ReqType, RespType>::type;	\
	\
	static func##DelegateType&& func##Delegate() {	\
		return std::forward<func##DelegateType>(func##DelegateType::create<ClientType, &ClientType::Async##func>( getClientInstance() ));	\
	}



class GRPC_CLIENT_EXPORT_API MainWorkerClient {
private:
	std::unique_ptr<y3d::YServiceMainWorker::Stub> stub_;
public:
	MainWorkerClient(std::shared_ptr<grpc::Channel> channel);	
	DefineFunc(DoEvent, y3d::YEvent2, y3d::ResponseEvent2);
	DefineFunc(UpdateWorkers, y3d::YWorkerResp, y3d::ENone2);
	DefineFunc(CloseWorkerApp, y3d::YEvent2, y3d::ResponseEvent2);		
};


struct DelegateList {
	DefineDelegate(DoEvent, y3d::YEvent2, y3d::ResponseEvent2);
	DefineDelegate(UpdateWorkers, y3d::YWorkerResp, y3d::ENone2);
	DefineDelegate(CloseWorkerApp, y3d::YEvent2, y3d::ResponseEvent2);

};

#define GRPC_CALL(func,...)	{	\
	auto* client = getClientInstance();	\
	if (client)	{	\
		grpc::Status status = client->##func(__VA_ARGS__);	\
		if (!status.ok()) {	\
			reset();	\
			client = getClientInstance();	\
			status = client->##func(__VA_ARGS__);	\
			if (!status.ok()) logserver::LOG("Execution of method {} failed\n",#func);	\
		}	\
	} else {	\
		logserver::LOG("Cannot connect to MainWorker server\n");	\
	};	\
}

struct tag_grpc {};

template <class EVENT, class RESPTYPE>
class grpc_call_base {
	typedef EVENT value_type;
	typedef tag_grpc grpc_tag;
};

template<class T>
class is_grpc_call
{
	template<class C>
	static typename C::grpc_tag* check(int);
	template<class C>
	static void check(...);
public:
	static const bool value = std::is_convertible<decltype(check<rxu::decay_t<T>>(0)), tag_grpc*>::value;
};

template <class T>
struct Deleter {
	void operator()(T* e) const {
		e->~T();
		concurrency::Free(e);		
	}
};

thread_local static std::unique_ptr<grpc::CompletionQueue> cq = std::make_unique<grpc::CompletionQueue>();

template <class EVENT, class RESPTYPE>
class grpc_async_call : public grpc_call_base<EVENT, RESPTYPE> {	

	typedef typename async_delegate<EVENT, RESPTYPE>::type delegate_type;
	typedef AsyncClientCall<RESPTYPE>* call_ctx_type;
	typedef typename std::shared_ptr< AsyncClientCall<RESPTYPE> > call_type;

public:
	grpc_async_call(delegate_type&& f) : func(f) {
		
	};

	//template <class Derived, typename = std::enable_if< is_grpc_call<Derived>::value >, void >
	call_ctx_type operator() (EVENT&& e) {
		void* buf = concurrency::Alloc(sizeof(AsyncClientCall<RESPTYPE>));		
		call = new (buf) (AsyncClientCall<RESPTYPE>);
		call->cq = cq.get();
		//call->response_reader = client->AsyncDoEvent(&call->context, e, &cq);
		call->response_reader = func(&call->context, e, call->cq);
		//std::function< std::unique_ptr<grpc::ClientAsyncResponseReader<RESPTYPE>>(grpc::ClientContext*, const EVENT &, grpc::CompletionQueue*) > func = &MainWorkerClient::AsyncDoEvent;
		call->response_reader->Finish(&(call->reply), &(call->status), (void*)(call));
		return call;
	}

	auto wait_for_call(call_type e,int TIMEOUT = 5000) {
		LOG("TAP - thread {}.\n", thread_to_str(std::this_thread::get_id()));

		void* got_tag;
		bool ok = false;

		std::pair<RESPTYPE, grpc::Status> rs;
		auto* cq = e->cq;
		LOG("CQ addr - thread {}.\n", (void*)cq);
		//std::this_thread::sleep_for(std::chrono::milliseconds(10000));
		
		if (TIMEOUT == 0) {
			rs.second = grpc::Status::CANCELLED;
			if (cq->Next(&got_tag, &ok)) {
				rs.second = call->status;
				if (call->status.ok()) {
					rs.first = call->reply;
					LOG("Got tag {}\n", call->reply.msg());
				}
			}
			return rs;
		}

		const std::chrono::system_clock::time_point deadline = std::chrono::system_clock::now() + std::chrono::milliseconds(TIMEOUT);
		
		// Block until the next result is available in the completion queue "cq".

		do {
			auto status = cq->AsyncNext(&got_tag, &ok, deadline);
			AsyncClientCall<RESPTYPE>* call = static_cast<AsyncClientCall<RESPTYPE>*>(got_tag);

			switch (status)
			{
			case grpc::CompletionQueue::SHUTDOWN:
				rs.second = grpc::Status::CANCELLED;				
				LOG("RPC Shutdown\n");				
				break;
			case grpc::CompletionQueue::TIMEOUT:
				rs.second = grpc::Status::CANCELLED;
				LOG("RPC Timeout\n");				
				break;
			case grpc::CompletionQueue::GOT_EVENT:

				if (!ok) {
					rs.second = call->status;
					LOG("RPC failed\n");
				}
				else {
					// The tag in this example is the memory location of the call object						
					// Verify that the request was completed successfully. Note that "ok"
					// corresponds solely to the request for updates introduced by Finish().					
					rs.second = call->status;
					if (call->status.ok()) {
						rs.first = call->reply;
						LOG("Got tag {}\n", call->reply.msg());
					}					
				}
				break;
			default:
				break;
			}

			break;
		} while (true);


		return rs;
	}
	

private:	
	call_ctx_type call;
	delegate_type func;	
};

//typedef decltype(std::result_of<decltype(&ClientType::AsyncDoEvent)(Type, Args)>::type) RetType;
//template <class RESPTYPE>
//using RetType = std::unique_ptr<grpc::ClientAsyncResponseReader<RESPTYPE>> (ClientType::*)(::grpc::ClientContext*, const EVENT&, ::grpc::CompletionQueue*);
//typename RetType<RESPTYPE> T::*TMethod(::grpc::ClientContext* , const EVENT& , ::grpc::CompletionQueue*) 


template <class EVENT, class RESPTYPE, int TIMEOUT = 0 >
struct EventBus
{
	typedef typename async_delegate<EVENT, RESPTYPE>::type delegate_type;
	typedef typename std::shared_ptr< AsyncClientCall<RESPTYPE> > call_ctx_type;
	typedef typename Deleter < AsyncClientCall<RESPTYPE> > deleter_type;

	EventBus(delegate_type&& f) : _subscriber(s.get_subscriber()), func( std::forward<delegate_type>(f) ) {		
	};

	~EventBus() {
		_subscriber.on_completed();
		_subscriber.unsubscribe();
		cq.Shutdown();
	};

	auto& complete_queue() {
		return cq;
	}

	bool is_subscribed() {
		return _subscriber.is_subscribed();
	}

	void subscribe(std::function<rxcpp::observable<EVENT>(rxcpp::observable<EVENT>)> fo) {
		observable(fo).subscribe([](auto e) {
			//y3d::ResponseEvent2& reply = e.first;

			LOG("On next {} \n", 1);
		}, [](auto e) {
			LOG("On error \n");
		}, []() {
			LOG("On completed\n");
		});
	}

	void subscribe() {
		observable().subscribe([](auto e) {
			//y3d::ResponseEvent2& reply = e.first;
			LOG("On next {} \n", 1);
		}, [](auto e) {
			LOG("On error \n");
		}, []() {
			LOG("On completed\n");
		});
	}

	void initialize() {
		subscribe();
	};

	void initialize(std::function<rxcpp::observable<EVENT>(rxcpp::observable<EVENT>)> fo) {
		subscribe(fo);
	};

	auto observable(std::function<rxcpp::observable<EVENT>(rxcpp::observable<EVENT>)> fo)
	{
		return fo(s.get_observable()).observe_on(rxcpp::observe_on_asio(ThreadPool::shared_instance().service()))
			.map([this](EVENT e) {
			LOG("Subscribing - thread {}.\n", thread_to_str(std::this_thread::get_id()));
			return call_ctx_type(func(std::forward<EVENT>(e)), deleter_type());
		}).map([this](auto e) {
			return func.wait_for_call(e, TIMEOUT);
		}).retry();
			
	}

	auto observable()
	{		
		return s.get_observable()
			.observe_on(rxcpp::observe_on_asio(ThreadPool::shared_instance().service()))
			.map([this](EVENT e) {
				LOG("Subscribing - thread {}.\n", thread_to_str(std::this_thread::get_id()));							
				return call_ctx_type(func(std::forward<EVENT>(e)), deleter_type() );
			}).map([this](auto e) {
				return func.wait_for_call(e, TIMEOUT);
			}).retry();
	}

	auto publish(EVENT e) {
		return _subscriber.on_next(e);
	}

	void unsubscribe() {
		_subscriber.unsubscribe();
	}

	void release() {
		return cq.Shutdown();
	}

	void complete() {
		_subscriber.on_completed();		
	};
private:
	grpc::CompletionQueue cq;	
	rxcpp::subjects::subject<EVENT> s;
	rxcpp::subscriber<EVENT> _subscriber;
	grpc_async_call<EVENT, RESPTYPE> func;	
};


