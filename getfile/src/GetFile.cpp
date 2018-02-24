#include <sstream>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include "GetFile.hpp"
#include "Uri.hpp"

// Evil hack to make this code work on Windows
#if defined _WIN32
#define stat _stat
#endif

namespace zia
{
	bool GetFile::config(api::Conf const &)
	{
		return true;
	}

	bool GetFile::exec(api::HttpDuplex &http)
	{
		if (http.req.method != api::http::Method::get)
		{
			return false;
		}


		auto &res = http.resp;
		auto uri = http::Uri(http.req.uri);
		auto file = uri.path();
		struct stat st;

		if (stat(file.c_str(), &st) != 0)
		{
			res.version = api::http::Version::http_1_1;
			res.status = api::http::common_status::not_found;
			res.reason = "Not-Found";

			return true;
		}

		auto is = std::ifstream(file.c_str());

		if (!is.good() || !is.is_open())
		{
			res.version = api::http::Version::http_1_1;
			res.status = api::http::common_status::internal_server_error;
			res.reason = "Internal-Server-Error";

			return true;
		}

		auto os = std::ostringstream();

		os << is.rdbuf();

		auto body = os.str();
			
		res.body.clear();
		res.body.resize(body.length());

		res.version = api::http::Version::http_1_1;
		res.status = api::http::common_status::ok;
		res.reason = "OK";
		res.headers["Content-Length"] = std::to_string(body.length());
		res.headers["Content-Type"] = "text/html; charset=UTF-8";
		std::memcpy(res.body.data(), body.data(), body.length());
		
		return true;
	}
}