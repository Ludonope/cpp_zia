#include <sys/types.h>
#include <sys/stat.h>
#include <iostream>
#include "AutoIndex.hpp"
#include "Uri.hpp"

// Evil hack to make this code work on Windows
#if defined _WIN32
#define stat _stat
#endif

namespace zia
{
	bool AutoIndex::config(api::Conf const &conf)
	{
		auto success = true;

		try
		{
			auto vals = std::get<api::ConfArray>(conf.at("default_files").v);

			m_defaultFiles.clear();
			for (auto const &val : vals)
			{
				m_defaultFiles.push_back(std::get<std::string>(val.v));
			}
		}
		catch (std::exception const &e)
		{
			std::cerr << e.what() << std::endl;
			success = false;
			m_defaultFiles = { "index.html", "index.php" };
		}

		return success;
	}

	bool AutoIndex::exec(api::HttpDuplex &http)
	{
		auto uri = http::Uri(http.req.uri);
		auto const &path = uri.path();

		if (path.length() > 0 && path[path.length() - 1] == '/')
		{
			for (auto const &file : m_defaultFiles)
			{
				struct stat st;
				auto filePath = path + file;

				if (stat(filePath.c_str(), &st) == 0 &&
					(st.st_mode & S_IFMT) == S_IFREG)
				{
					uri.setPath(filePath);
					http.req.uri = uri.toString();
					return true;
				}
			}

		}

		return true;
	}
}