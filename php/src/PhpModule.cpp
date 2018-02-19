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

#include <iostream> // TODO: remove

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

		std::cout << "Path: " << path << '\n';
		std::cout << "Uri parsed: " << uri.toString() << '\n';

		// Check each valid php file extension
		for (auto ext : valid)
		{
			auto len = std::strlen(ext);
			auto sub = path.substr(path.length() - std::min(len, path.length()), len);

			if (sub == ext)
			{
				std::cout << "Php file extension: " << ext << '\n';
				return this->executeRequest(http.resp, uri);
			}
		}

		return false;
	}

	bool PhpModule::executeRequest(api::HttpResponse &res, http::Uri const &uri)
	{
		auto command = "php-cgi -f "s + std::quoted(uri.path())._Ptr;
		char psBuffer[4096];
		FILE *iopipe;
		std::ostringstream os;
		auto query = PhpQuery(uri.query());

		for (auto const &[param, value] : query)
		{
			command += " \"" + param + '=' + value + '"';
		}

		//command = "echo %cd%";
		std::cout << "Command: " << command << '\n';

		if ((iopipe = popen(command.c_str(), "r")) == nullptr)
			return false;

		while (!feof(iopipe))
		{
			if (fgets(psBuffer, 4096, iopipe) != nullptr)
			{
				os << psBuffer;
			}
		}

		auto ret = pclose(iopipe);

		std::cout << "Command output: " << ret << '\n';

		if (ret == 0)
		{
			auto body = os.str();
			
			std::cout << "Content: \n" << body << "\n\n";

			res.body.clear();
			res.body.resize(body.length());

			std::memcpy(res.body.data(), body.data(), body.length());
			res.headers["Content-Length"] = std::to_string(body.length());
			res.version = api::http::Version::http_1_1;
			res.status = api::http::common_status::ok;
			res.reason = "OK";
		}

		return ret == 0;
	}
}