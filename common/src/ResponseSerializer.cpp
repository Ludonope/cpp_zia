#include <sstream>
#include "ResponseSerializer.hpp"

namespace zia::http
{
	static std::string versionToString(api::http::Version ver)
	{
		auto s = std::string();

		switch (ver)
		{
		case api::http::Version::http_0_9:
			s = "HTTP/0.9";
			break;
		case api::http::Version::http_1_0:
			s = "HTTP/1.0";
			break;
		case api::http::Version::http_1_1:
			s = "HTTP/1.1";
			break;
		case api::http::Version::http_2_0:
			s = "HTTP/2.0";
			break;
		default:
			s = "HTTP/unknown";
			break;
		}

		return s;
	}

	std::string responseToString(api::HttpResponse const &res)
	{
		auto os = std::ostringstream();

		os << versionToString(res.version) << ' '
			<< res.status << ' '
			<< res.reason << "\r\n";

		for (auto const &[header, value] : res.headers)
		{
			os << header << ": " << value << "\r\n";
		}
		os << "\r\n";

		auto data = reinterpret_cast<char const *>(res.body.data());
		auto body = std::string_view(data, res.body.size());

		os << std::string(body);

		return os.str();
	}

	api::Net::Raw responseToRaw(api::HttpResponse const &res)
	{
		auto s = responseToString(res);
		auto raw = api::Net::Raw(s.size());
		auto srcPtr = reinterpret_cast<std::byte *>(s.data());

		std::memcpy(raw.data(), srcPtr, s.size());
		return raw;
	}
}