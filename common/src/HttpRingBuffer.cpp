#include <cassert>
#include <cstring>
#include <iostream>
#include "HttpRingBuffer.hpp"
#include "HttpRequestParser.hpp"

namespace zia::network
{
		HttpRingBuffer::HttpRingBuffer() noexcept :
			RingBuffer()
		{
		}

		HttpRingBuffer::HttpRingBuffer(HttpRingBuffer const &other) :
			RingBuffer(other)
		{

		}

		HttpRingBuffer::HttpRingBuffer(HttpRingBuffer &&other) :
			RingBuffer(std::move(other))
		{

		}

		HttpRingBuffer &HttpRingBuffer::operator=(HttpRingBuffer const &other)
		{
			RingBuffer::operator=(other);
			return *this;
		}

		HttpRingBuffer &HttpRingBuffer::operator=(HttpRingBuffer &&other)
		{
			RingBuffer::operator=(std::move(other));
			return *this;
		}

		bool HttpRingBuffer::hasHeader() const noexcept
		{
			return getHeaderLength() != 0;
		}

		bool HttpRingBuffer::hasRequest() noexcept
		{
			auto const headerLength = getHeaderLength();
			auto rc = true;

			if (headerLength == 0)
			{
				return false;
			}
			try
			{
				std::size_t contentLength = 0;
				// Datas is guaranted to contain at a double CRLF
				auto const datas = this->peek();
				std::cout << (char *)datas.data() << '\n';
				http::parseRequest(&datas[0], contentLength);
				rc = (datas.size() - headerLength) >= contentLength;
			}
			catch (std::exception const &e)
			{
				std::cout << e.what() << '\n';
				this->read(headerLength);
				rc = false;
			}
			return rc;
		}

		api::Net::Raw HttpRingBuffer::getRequest() noexcept
		{
			api::Net::Raw request;
			assert(hasRequest());
			auto const headerLength = getHeaderLength();
			auto const datas = this->peek();
			std::size_t bodySize = 0;
			http::parseRequest(&datas[0], bodySize);
			auto const totalSize = headerLength + bodySize;
			request.resize(totalSize, {});
			this->read(&request[0], totalSize);
			return request;
		}

		std::size_t HttpRingBuffer::getHeaderLength() const noexcept
		{
			std::size_t const max = this->getAvailableData();
			assert(max < detail::HTTP_BUFFER_SIZE);
			for (std::size_t i = 0; i < max; ++i)
			{
				if (i + 4u <= max)
				{
					std::array<std::byte, 5> buff = {};
					buff[0] = std::byte{RingBuffer::operator[](m_ndxRead + i + 0)};
					buff[1] = std::byte{RingBuffer::operator[](m_ndxRead + i + 1)};
					buff[2] = std::byte{RingBuffer::operator[](m_ndxRead + i + 2)};
					buff[3] = std::byte{RingBuffer::operator[](m_ndxRead + i + 3)};
					buff[4] = std::byte{0};
					if (!std::strncmp(reinterpret_cast<char const *>(buff.data()),
						"\r\n\r\n", 4))
					{
						return i + 3 + 1;
					}
				}
			}
			return 0;
		}
}
