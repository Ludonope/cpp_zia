#pragma once

#include <cstdint>
#include <mutex>
#include <queue>

namespace zia
{
template <typename T>
class TSQueue final
{
public:
	TSQueue() : m_queue(), m_mutex()
	{
	}

	~TSQueue() = default;

	TSQueue(TSQueue const &other) = delete;
	TSQueue(TSQueue &&other) = delete;
	TSQueue &operator=(TSQueue const &other) = delete;
	TSQueue &operator=(TSQueue &&other) = delete;

	std::size_t size() const
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		return m_queue.size();
	}

	void push(T const &type)
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		m_queue.push(type);
	}
	
	void push(T &&type)
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		m_queue.push(std::move(type));
	}

	T &front()
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		return m_queue.front();
	}

	T const &front() const
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		return m_queue.front();
	}

	T &back()
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		return m_queue.back();
	}

	T const &back() const
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		return m_queue.back();
	}

	bool empty() const
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		return m_queue.empty();
	}

	T pop()
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		
		T elem = std::move(m_queue.front());
		m_queue.pop();
		return elem;
	}

private:
	std::queue<T> m_queue;
	mutable std::mutex m_mutex;
};
}
