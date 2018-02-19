#pragma once

#include <chrono>

namespace zia::core
{
	template <typename T>
	class Timer
	{
	public:
		Timer(std::uint32_t const threshold) noexcept :
			m_threshold(threshold)
		{
		}

		~Timer() = default;

		Timer(Timer const &other) :
			m_start(other.m_start), m_threshold(other.m_threshold)
		{
		}

		Timer(Timer &&other) :
			m_start(std::move(other.m_start)),
			m_threshold(std::move(other.m_threshold))
		{
		}

		Timer &operator=(Timer const &other)
		{
			if (this != &other)
			{
				m_start = other.m_start;
				m_threshold = other.m_threshold;
			}
			return *this;
		}

		Timer &operator=(Timer &&other)
		{
			if (this != &other)
			{
				m_start = std::move(other.m_start);
				m_threshold = std::move(other.m_threshold);
			}
			return *this;
		}

		void reset() noexcept
		{
			m_start = std::chrono::system_clock::now();
		}

		bool check() noexcept
		{
			auto now = std::chrono::system_clock::now();
			auto elapsedTime =
				std::chrono::duration_cast<T>(now - m_start).count();
			if (elapsedTime > m_threshold)
			{
				m_start = now;
				return true;
			}
			return false;
		}

	private:
		std::chrono::time_point<std::chrono::system_clock> m_start = std::chrono::system_clock::now();
		std::uint32_t m_threshold;
	};

}
