#pragma once
#include "rx-scheduler.hpp"

// TODO: C++17 Networking TS
#ifdef WITHOUT_BOOST
// Standalone ASIO
#include "asio.hpp"

namespace asio_ns = ::asio;
namespace system_ns = ::std;
#else
// Boost.ASIO
#include <boost/asio.hpp>

namespace asio_ns = ::boost::asio;
namespace system_ns = ::boost::system;
#endif

namespace rxcpp {

	namespace schedulers {
		inline std::string thread_to_str(std::thread::id myid) {
			std::stringstream ss;
			ss << myid;
			return ss.str();
		}

		class strand_worker;

		class strand_service : public scheduler_interface
		{
		private:
			// rxcpp types
			using composite_subscription = rxcpp::composite_subscription;
			asio_ns::io_service& io_service_;

			typedef strand_service this_type;

			strand_service(const this_type&) = delete;


			class strand_worker : public worker_interface
			{
			private:
				typedef strand_worker this_type;

				composite_subscription lifetime;
				mutable asio_ns::io_service::strand strand_;


			public:
				explicit strand_worker(composite_subscription cs, asio_ns::io_service& io_service)
					: lifetime(cs), strand_(io_service)
				{
					printf("strand worker %p created\n", this);
				}

				virtual ~strand_worker()
				{
					printf("strand worker %p destroyed\n", this);
					lifetime.unsubscribe();
				}

				virtual clock_type::time_point now() const override
				{
					return clock_type::now();
				}

				virtual void schedule(const schedulable& scbl) const override
				{
					if (!scbl.is_subscribed())
						return;

					auto keep_alive = shared_from_this();
					strand_.post([=]() {						
						(void)(keep_alive);
						// allow recursion
						scbl(recursion(true).get_recurse());
					});

				}

				virtual void schedule(clock_type::time_point when,
					const schedulable& scbl) const
				{
					if (scbl.is_subscribed()) {
						printf("scheduled on %p with timeout\n", this);
						auto keep_alive = shared_from_this();
						auto timer = std::make_shared<asio_ns::basic_waitable_timer<clock_type>>
							(strand_.get_io_service(), when);
						timer->async_wait([=](const system_ns::error_code&) {
							(void)(keep_alive);
							(void)(timer);
							// allow recursion
							scbl(recursion(true).get_recurse());
						});
					}

				}
			};

		public:
			explicit strand_service(asio_ns::io_service& io_service)
				: io_service_(io_service)
			{
			}

			virtual clock_type::time_point now() const
			{
				return clock_type::now();
			}

			virtual worker create_worker(composite_subscription cs) const
			{
				return worker(cs, std::make_shared<strand_worker>(cs,io_service_));
			}
		};

		inline scheduler make_strand(asio_ns::io_service& io_service)
		{
			return rxcpp::schedulers::make_scheduler<strand_service>(io_service);
		}

	}  // End of namespace schedulers

	inline observe_on_one_worker observe_on_strand(asio_ns::io_service& io_service)
	{
		return observe_on_one_worker(rxsc::make_strand(io_service));
	}

	inline synchronize_in_one_worker synchronize_in_strand(asio_ns::io_service& io_service)
	{
		return synchronize_in_one_worker(rxsc::make_strand(io_service));
	}

	inline identity_one_worker identitiy_strand(asio_ns::io_service& io_service)
	{
		return identity_one_worker(rxsc::make_strand(io_service));
	}

	inline serialize_one_worker serialize_strand(asio_ns::io_service& io_service)
	{
		return serialize_one_worker(rxsc::make_strand(io_service));
	}
}