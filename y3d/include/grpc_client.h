#pragma once
#include <string>
#include <atomic>
#include <mutex>
#include <grpc++/grpc++.h>
#include "yevent.grpc.pb.h"
#include <chrono>
#include "rx.hpp"
#include <sstream>
#include "rx-asio.hpp"

#ifdef GRPC_CLIENT_EXPORT
#define GRPC_CLIENT_EXPORT_API __declspec( dllexport )   
#elif defined(GRPC_CLIENT_SERVICE_STATIC)
#define GRPC_CLIENT_EXPORT_API    
#else
#define GRPC_CLIENT_EXPORT_API __declspec( dllimport )   
#endif



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

	// Storage for the status of the RPC upon completion.
	grpc::Status status;

	std::unique_ptr<grpc::ClientAsyncResponseReader<RespType>> response_reader;

	~AsyncClientCall() {
	}
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
struct EventBus
{

	EventBus() : work(io_service),_subscriber(s.get_subscriber()), worker(rxcpp::observe_on_asio(io_service) ),  init(false) {
	};

	~EventBus() {
		_subscriber.on_completed();
		_subscriber.unsubscribe();
		cq.Shutdown();
		io_service.stop();
		for (int i = 0; i < pools.size(); ++i)
			pools[i].join();
	};

	bool is_subscribed() {
		return _subscriber.is_subscribed();
	}

	auto observable()
	{
		if (!init) {
			if (AllocConsole()) {				
				SetConsoleTitle(L"Debug console");
				freopen("CONOUT$", "w", stdout);
				freopen("CONIN$", "r", stdin);
				freopen("CONOUT$", "w", stderr);

				//Clear the error state for each of the C++ standard stream objects. We need to do this, as
				//attempts to access the standard streams before they refer to a valid target will cause the
				//iostream objects to enter an error state. In versions of Visual Studio after 2005, this seems
				//to always occur during startup regardless of whether anything has been read from or written to
				//the console or not.
				std::wcout.clear();
				std::cout.clear();
				std::wcerr.clear();
				std::cerr.clear();
				std::wcin.clear();
				std::cin.clear();
			}
			
			
			init = true;
			for (int i = 0; i < 4; ++i) {
				pools.emplace_back([this, i]() {
					LOG("Thread {} initialized\n", i);
					io_service.run();
					LOG("Thread {} exited\n", i);
				});
			}
		}
		
		return s.get_observable().map([this](EVENT e) {
			LOG("Subscribing - thread {}.\n", thread_to_str(std::this_thread::get_id()));
			auto* client = getClientInstance();
			grpc::ClientContext context;
			void* buf = Alloc(sizeof(AsyncClientCall<RESPTYPE>));
			AsyncClientCall<RESPTYPE>* call = new (buf) (AsyncClientCall<RESPTYPE>);

			call->response_reader = client->AsyncDoEvent(&call->context, e, &cq);
			//std::function< std::unique_ptr<grpc::ClientAsyncResponseReader<RESPTYPE>>(grpc::ClientContext*, const EVENT &, grpc::CompletionQueue*) > func = &MainWorkerClient::AsyncDoEvent;
			call->response_reader->Finish(&(call->reply), &(call->status), (void*)call);
			return call;
		}).observe_on( worker )
			.map([this](AsyncClientCall<RESPTYPE>* e) {
			LOG("TAP - thread {}.\n", thread_to_str(std::this_thread::get_id()));

			void* got_tag;
			bool ok = false;

			std::pair<RESPTYPE, grpc::Status> rs;
			const std::chrono::system_clock::time_point deadline = std::chrono::system_clock::now() + std::chrono::milliseconds(5000);
			// Block until the next result is available in the completion queue "cq".

			do {
				auto status = cq.AsyncNext(&got_tag, &ok, deadline);
				AsyncClientCall<RESPTYPE>* call = static_cast<AsyncClientCall<RESPTYPE>*>(got_tag);
				switch (status)
				{
				case grpc::CompletionQueue::SHUTDOWN:
					rs.second = grpc::Status::CANCELLED;
					e->~AsyncClientCall<RESPTYPE>();
					Free(e);
					break;
				case grpc::CompletionQueue::TIMEOUT:
					rs.second = grpc::Status::CANCELLED;
					e->context.TryCancel();
					e->~AsyncClientCall<RESPTYPE>();
					Free(e);
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
							//LOG("Got tag {}\n", call->reply.msg());
						}
						call->~AsyncClientCall<RESPTYPE>();
						Free(call);
					}
					break;
				default:
					break;
				}

				break;
			} while (true);


			return rs;
		});
	}

	auto publish(EVENT e) {
		return _subscriber.on_next(e);
	}

	void release() {
		return cq.Shutdown();
	}

	void complete() { _subscriber.on_completed(); };
private:
	asio::io_service io_service;
	asio::io_service::work work;
	std::vector<std::thread> pools;
	rxcpp::observe_on_one_worker worker;
	grpc::CompletionQueue cq;
	rxcpp::subjects::subject<EVENT> s;
	rxcpp::subscriber<EVENT> _subscriber;
	bool init;
};



#define DefineFunc(func,ReqType,RespType) \
	::grpc::Status func(::grpc::ClientContext* context, const ReqType& request, RespType* response){	\
		return stub_->##func (context,request,response);	\
	};	\
	\
	std::unique_ptr< ::grpc::ClientAsyncResponseReader< RespType >> Async##func (::grpc::ClientContext* context, const ReqType& request, ::grpc::CompletionQueue* cq){	\
		return stub_->Async##func (context,request,cq);	\
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


const std::string MainWorkerServerIP = "127.0.0.1:37001";
extern std::mutex mm_mutex;


typedef MainWorkerClient ClientType;
typedef std::atomic<ClientType*> ClientPtr;
extern ClientPtr clientPtr;
GRPC_CLIENT_EXPORT_API ClientType* getClientInstance();
GRPC_CLIENT_EXPORT_API void reset();

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