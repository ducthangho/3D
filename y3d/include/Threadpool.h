#pragma once
#include <asio.hpp>

#include <functional>
#include <thread>
#include <vector>
#include <memory>

class ThreadPool
{
public:
	inline ThreadPool(size_t threads = std::thread::hardware_concurrency());

	inline ~ThreadPool()
	{
		stop();
	}

	static ThreadPool& shared_instance() {
		static ThreadPool s_shared(std::thread::hardware_concurrency());		
		return s_shared;
	};

	asio::io_service& service() {
		return m_service;
	};

	inline void joinThreadPool();

	template <typename Handler>
	inline void post(Handler &&handler)
	{		
		m_service.post(handler);
	}

private:
	inline void start();
	inline void stop();
	inline void worker_thread_func();

	asio::io_service m_service;
	asio::io_service::work m_work;

	std::vector<std::thread> m_threads;
};

inline ThreadPool::ThreadPool(size_t threads)
	: m_service(threads), m_work(m_service), m_threads(threads)
{
	start();
}

inline void ThreadPool::start()
{	
	for (auto &i : m_threads)
	{
		i = std::thread(&ThreadPool::worker_thread_func, this);
	}

}

inline void ThreadPool::stop()
{	
	m_service.stop();

	for (auto &i : m_threads)
	{
		if (i.joinable())
		{
			i.join();
		}
	}
}

inline void ThreadPool::joinThreadPool()
{
	m_service.run();
}

inline void ThreadPool::worker_thread_func()
{
	joinThreadPool();
}

