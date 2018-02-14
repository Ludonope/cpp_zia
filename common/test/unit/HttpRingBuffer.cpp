#include <gtest/gtest.h>
#include <cstring>

#define protected public
#define private public // Evil hack for unit tests
#include "HttpRingBuffer.hpp"

TEST(HttpRingBuffer, getHeaderLength_Empty)
{
	zia::network::HttpRingBuffer	buffer;

	ASSERT_EQ(buffer.getHeaderLength(), 0);
}

TEST(HttpRingBuffer, getHeaderLength_Trash)
{
	char const *payload = "Hello World !";
	std::byte const *payloadPtr =
		reinterpret_cast<decltype(payloadPtr)>(payload);
	zia::network::HttpRingBuffer	buffer;

	buffer.write(payloadPtr, std::strlen(payload));
	ASSERT_EQ(buffer.getHeaderLength(), 0);
}

TEST(HttpRingBuffer, getHeaderLength_Trash2)
{
	char const *payload = "Hello World !\r\n";
	std::byte const *payloadPtr =
		reinterpret_cast<decltype(payloadPtr)>(payload);
	zia::network::HttpRingBuffer	buffer;

	buffer.write(payloadPtr, std::strlen(payload));
	ASSERT_EQ(buffer.getHeaderLength(), 0);
}

TEST(HttpRingBuffer, getHeaderLength_Valid)
{
	char const *payload = 
	"GET /hello.htm HTTP/1.1\r\n"
	"User-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT)\r\n"
	"Host: www.tutorialspoint.com\r\n"
	"Accept-Language: en-us\r\n"
	"Accept-Encoding: gzip, deflate\r\n"
	"Connection: Keep-Alive\r\n"
	"\r\n";
	std::byte const *payloadPtr =
		reinterpret_cast<decltype(payloadPtr)>(payload);
	zia::network::HttpRingBuffer	buffer;

	buffer.write(payloadPtr, std::strlen(payload));
	ASSERT_EQ(buffer.getHeaderLength(), std::strlen(payload));
}

TEST(HttpRingBuffer, getHeaderLength_ValidWithMoreAfter)
{
	char const *payload = 
	"GET /hello.htm HTTP/1.1\r\n"
	"User-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT)\r\n"
	"Host: www.tutorialspoint.com\r\n"
	"Accept-Language: en-us\r\n"
	"Accept-Encoding: gzip, deflate\r\n"
	"Connection: Keep-Alive\r\n"
	"\r\nTrashContentLolololol";
	std::byte const *payloadPtr =
		reinterpret_cast<decltype(payloadPtr)>(payload);
	zia::network::HttpRingBuffer	buffer;

	buffer.write(payloadPtr, std::strlen(payload));
	ASSERT_EQ(buffer.getHeaderLength(), std::strlen(payload) -
		std::strlen("TrashContentLolololol"));
}

TEST(HttpRingBuffer, HasHeader_Empty)
{
	zia::network::HttpRingBuffer	buffer;

	ASSERT_EQ(buffer.hasHeader(), false);
}

TEST(HttpRingBuffer, HasHeader_Trash)
{
	char const *payload = "Hello World !";
	std::byte const *payloadPtr =
		reinterpret_cast<decltype(payloadPtr)>(payload);
	zia::network::HttpRingBuffer	buffer;

	buffer.write(payloadPtr, std::strlen(payload));
	ASSERT_EQ(buffer.hasHeader(), false);
}

TEST(HttpRingBuffer, HasHeader_Valid)
{
	char const *payload = 
	"GET /hello.htm HTTP/1.1\r\n"
	"User-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT)\r\n"
	"Host: www.tutorialspoint.com\r\n"
	"Accept-Language: en-us\r\n"
	"Accept-Encoding: gzip, deflate\r\n"
	"Connection: Keep-Alive\r\n"
	"\r\n";
	std::byte const *payloadPtr =
		reinterpret_cast<decltype(payloadPtr)>(payload);
	zia::network::HttpRingBuffer	buffer;

	buffer.write(payloadPtr, std::strlen(payload));
	ASSERT_EQ(buffer.hasHeader(), true);
}

TEST(HttpRingBuffer, HasHeader_ValidWithMoreAfter)
{
	char const *payload = 
	"GET /hello.htm HTTP/1.1\r\n"
	"User-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT)\r\n"
	"Host: www.tutorialspoint.com\r\n"
	"Accept-Language: en-us\r\n"
	"Accept-Encoding: gzip, deflate\r\n"
	"Connection: Keep-Alive\r\n"
	"\r\nTrashContentLolololol";
	std::byte const *payloadPtr =
		reinterpret_cast<decltype(payloadPtr)>(payload);
	zia::network::HttpRingBuffer	buffer;

	buffer.write(payloadPtr, std::strlen(payload));
	ASSERT_EQ(buffer.hasHeader(), true);
}

TEST(HttpRingBuffer, GetRequest)
{
	char const *payload = 
	"GET /hello.htm HTTP/1.1\r\n"
	"User-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT)\r\n"
	"Host: www.tutorialspoint.com\r\n"
	"Accept-Language: en-us\r\n"
	"Accept-Encoding: gzip, deflate\r\n"
	"Connection: Keep-Alive\r\n"
	"\r\n";
	std::byte const *payloadPtr =
		reinterpret_cast<decltype(payloadPtr)>(payload);
	zia::network::HttpRingBuffer	buffer;

	buffer.write(payloadPtr, std::strlen(payload));
	ASSERT_EQ(buffer.hasHeader(), true);

	auto const req = buffer.getRequest();
	ASSERT_EQ(req.size(), std::strlen(payload));
	ASSERT_EQ(std::strncmp(payload, 
		reinterpret_cast<char const *>(&req[0]), req.size()), 0);
}

TEST(HttpRingBuffer, GetRequest_TooLong)
{
	char const *payload = 
	"GET /hello.htm HTTP/1.1\r\n"
	"User-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT)\r\n"
	"Host: www.tutorialspoint.com\r\n"
	"Accept-Language: en-us\r\n"
	"Accept-Encoding: gzip, deflate\r\n"
	"Connection: Keep-Alive\r\n"
	"\r\nWithTrashLolololol";
	std::byte const *payloadPtr =
		reinterpret_cast<decltype(payloadPtr)>(payload);
	zia::network::HttpRingBuffer	buffer;

	buffer.write(payloadPtr, std::strlen(payload));
	ASSERT_EQ(buffer.hasHeader(), true);

	auto const req = buffer.getRequest();
	ASSERT_EQ(req.size() < std::strlen(payload), true);
	ASSERT_EQ(req.size(), std::strlen(payload) - std::strlen("WithTrashLolololol"));
	ASSERT_EQ(std::strncmp(payload, 
		reinterpret_cast<char const *>(&req[0]), req.size()), 0);
}

TEST(HttpRingBuffer, GetRequest_WithContent)
{
	char const *payload = 
	"POST /path/script.cgi HTTP/1.0\r\n"
	"From: frog@jmarshall.com\r\n"
	"User-Agent: HTTPTool/1.0\r\n"
	"Content-Type: application/x-www-form-urlencoded\r\n"
	"Content-Length: 32\r\n"
	"\r\n"
	"home=Cosby&favorite+flavor=flies";
	std::byte const *payloadPtr =
		reinterpret_cast<decltype(payloadPtr)>(payload);
	zia::network::HttpRingBuffer	buffer;

	buffer.write(payloadPtr, std::strlen(payload));
	ASSERT_EQ(buffer.hasHeader(), true);
	ASSERT_EQ(buffer.getHeaderLength(), std::strlen(payload) - 32);

	auto const req = buffer.getRequest();
	ASSERT_EQ(req.size(), std::strlen(payload));
	ASSERT_EQ(std::strncmp(payload, 
		reinterpret_cast<char const *>(&req[0]), req.size()), 0);
}
