#pragma once

#include <cstdint>
#include <array>
#include <cctype>
#include <cstddef>

namespace zia
{
	template <std::size_t S>
	class RingBuffer
	{
	public:
		RingBuffer() noexcept : m_data{}
		{
		}

		~RingBuffer() = default;

		RingBuffer(RingBuffer const &other) noexcept :
			m_data(other.m_data),
			m_ndxWrite(other.m_ndxWrite),
			m_ndxRead(other.m_ndxRead)
		{
		}

		RingBuffer(RingBuffer &&other) noexcept :
			m_data(std::move(other.m_data)),
			m_ndxWrite(std::move(other.m_ndxWrite)),
			m_ndxRead(std::move(other.m_ndxRead))
		{
		}

		RingBuffer &operator=(RingBuffer const &other) noexcept
		{
			if (this != &other)
			{
				m_data = other.m_data;
				m_ndxWrite = other.m_ndxWrite;
				m_ndxRead = other.m_ndxRead;
			}
			return *this;
		}

		RingBuffer &operator=(RingBuffer &&other) noexcept
		{
			if (this != &other)
			{
				m_data = std::move(other.m_data);
				m_ndxWrite = std::move(other.m_ndxWrite);
				m_ndxRead = std::move(other.m_ndxRead);
			}
			return *this;
		}

		void write(std::byte const *data, std::size_t const len) noexcept
		{
			if (data)
			{
				for (std::size_t i = 0; i < len; ++i)
				{
					if (m_ndxWrite == sizeof(m_data))
					{
						m_ndxWrite = 0;
					}
					m_data[m_ndxWrite] = data[i];
					++m_ndxWrite;
				}
			}
		}

		void read(std::byte *data, std::size_t const len) noexcept
		{
			if (data)
			{
				for (std::size_t i = 0; i < len; ++i)
				{
					if (m_ndxRead == sizeof(m_data))
					{
						m_ndxRead = 0;
					}
					data[i] = m_data[m_ndxRead];
					m_data[m_ndxRead] = '\0';
					++m_ndxRead;
				}
			}
		}

	protected:
		std::array<std::byte, S> m_data;
		std::size_t m_ndxWrite = 0;
		std::size_t m_ndxRead = 0;
	};
}
