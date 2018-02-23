#include <algorithm>
#include <array>
#include <string_view>
#include <sstream>
#include <cstdio>
#include <cstdlib>
#include <iomanip>
#include "PhpModule.hpp"
#include "Uri.hpp"
#include "PhpQuery.hpp"

#if defined _WIN32
#define popen _popen
#define pclose _pclose
#endif

using std::operator""s;

namespace zia::php
{
	bool PhpModule::config(api::Conf const &conf)
	{
		return true;
	}

	bool PhpModule::exec(api::HttpDuplex &http)
	{
		static const std::array<const char *, 6> valid = {{
			".php3", ".php4", ".php5", ".php7", ".phtml", ".php"
		}};
		auto uri = http::Uri(http.req.uri);
		auto path = std::string_view(uri.path());

		// Check each valid php file extension
		for (auto ext : valid)
		{
			auto len = std::strlen(ext);
			auto sub = path.substr(path.length() - std::min(len, path.length()), len);

			if (sub == ext)
			{
				return this->executeRequest(http.resp, uri);
			}
		}

		return false;
	}

	bool PhpModule::executeRequest(api::HttpResponse &res, http::Uri const &uri)
	{
		char psBuffer[4096];
		FILE *iopipe;
		std::ostringstream os;
		auto query = PhpQuery(uri.query());

		os << "php-cgi -f " << std::quoted(uri.path());

		for (auto const &[param, value] : query)
		{
			os << ' ' << std::quoted(param + '=' + value);
		}

		//command = "echo %cd%";
		auto command = os.str();

		if ((iopipe = popen(command.c_str(), "r")) == nullptr)
			return false;

		os.str("");
		os.clear();

		while (!feof(iopipe))
		{
			if (fgets(psBuffer, 4096, iopipe) != nullptr)
			{
				os << psBuffer;
			}
		}

		auto ret = pclose(iopipe);

		if (ret == 0)
		{
			auto body = os.str();
			
			res.body.clear();
			res.body.resize(body.length());

			res.version = api::http::Version::http_1_1;
			res.status = api::http::common_status::ok;
			res.reason = "OK";
			res.headers["Content-Length"] = std::to_string(body.length());
			res.headers["Content-Type"] = "text/html; charset=UTF-8";
			std::memcpy(res.body.data(), body.data(), body.length());
		}

		return ret == 0;
	}
}