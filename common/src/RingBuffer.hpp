#pragma once

#include <cassert>
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

		std::byte	operator[](std::size_t const ndx) const
		{
			return m_data[ndx % S];
		}

		// Write datas to the buffer
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
					++m_availableData;
				}
			}
		}

		// Read datas from the buffer, and update indexes
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
					m_data[m_ndxRead] = std::byte{'\0'};
					++m_ndxRead;
					assert(m_availableData > 0);
					--m_availableData;
				}
			}
		}

		// Simulate a read
		void read(std::size_t const len) noexcept
		{
			for (std::size_t i = 0; i < len; ++i)
			{
				if (m_ndxRead == sizeof(m_data))
				{
					m_ndxRead = 0;
				}
				m_data[m_ndxRead] = std::byte{'\0'};
				++m_ndxRead;
				assert(m_availableData > 0);
				--m_availableData;
			}
		}

		// Get differences of byte between read cursor and write cursor (a.k.a: available datas)
		inline std::size_t getAvailableData() const noexcept
		{
			return m_availableData % (S + 1);
		}

		// Get all the "available data" (see above), without modifying the indexes
		std::vector<std::byte> peek() const noexcept
		{
			auto datas = std::vector<std::byte>();
			auto const remainingSize = getAvailableData();
			for (std::size_t i = 0, ndx = m_ndxRead;
				i < remainingSize; ++i, ++ndx)
			{
				if (ndx == S)
				{
					ndx = 0;
				}
				datas.push_back(m_data[ndx]);
			}
			return datas;
		}

	protected:
		std::array<std::byte, S> m_data;
		std::size_t m_ndxWrite = 0;
		std::size_t m_ndxRead = 0;
		std::size_t m_availableData = 0;
	};
}
