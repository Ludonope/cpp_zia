#pragma once

#include "api/net.h"
#include "RingBuffer.hpp"

namespace zia::network
{
	namespace detail
	{
		static constexpr auto HTTP_BUFFER_SIZE = 4096 * 1024;
	}

	class HttpRingBuffer final : public RingBuffer<detail::HTTP_BUFFER_SIZE>
	{
	public:
		HttpRingBuffer() noexcept;
		~HttpRingBuffer() = default;
		HttpRingBuffer(HttpRingBuffer const &other);
		HttpRingBuffer(HttpRingBuffer &&other);
		HttpRingBuffer &operator=(HttpRingBuffer const &);
		HttpRingBuffer &operator=(HttpRingBuffer &&);

		bool hasHeader() const noexcept;
		bool hasRequest() noexcept;
		api::Net::Raw getRequest() noexcept;

	private:
		std::size_t getHeaderLength() const noexcept;
	};
}
