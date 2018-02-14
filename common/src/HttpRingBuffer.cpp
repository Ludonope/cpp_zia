#include "HttpRingBuffer.hpp"

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

		bool HttpRingBuffer::hasRequest() const noexcept
		{
			// TODO
		}

		api::Net::Raw HttpRingBuffer::getRequest() noexcept
		{
			// TODO
		}
}
