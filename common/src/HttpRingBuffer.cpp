#include <cassert>
#include "HttpRingBuffer.hpp"
#include "HttpRequestParser.hpp"

#include <iostream> // TODO: remove

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

		bool HttpRingBuffer::hasRequest() const noexcept
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
				http::parseRequest(&datas[0], contentLength);
				std::cout << reinterpret_cast<char const *>(&datas[0]) << std::endl;
				rc = (datas.size() - headerLength) >= contentLength;
			}
			catch (std::exception const &)
			{
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
			auto const carriageReturn = std::byte{0x0D};
			auto const lineFeed = std::byte{0x0A};
			std::size_t const max = this->getAvailableData();
			assert(max < detail::HTTP_BUFFER_SIZE);
			auto ndx = m_ndxRead;

			while (ndx < max)
			{
				auto data = RingBuffer::operator[](ndx);
				auto dataNext = RingBuffer::operator[](ndx + 1);
				if (data == carriageReturn && (ndx + 3 < max) &&
					dataNext == lineFeed)
				{
					data = RingBuffer::operator[](ndx + 2);
					dataNext = RingBuffer::operator[](ndx + 3);
					if (data == carriageReturn && dataNext == lineFeed) {
						return ndx + 3 + 1; // Add one for the length of the string
					}
				}
				++ndx;
			}
			return 0;
		}
}
