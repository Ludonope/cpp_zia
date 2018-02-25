#include <iostream>
#include "CustomRoot.hpp"
#include "Uri.hpp"

namespace zia
{
	bool CustomRoot::config(api::Conf const &conf)
	{
		auto success = true;

		try
		{
			m_root = std::get<std::string>(conf.at("root").v);

			if (m_root.length() > 0 && m_root[m_root.length() - 1] != '/')
			{
				m_root += '/';
			}
		}
		catch (std::exception const &e)
		{
			std::cerr << e.what() << std::endl;
			m_root = "";
			success = false;
		}

		return success;
	}

	bool CustomRoot::exec(api::HttpDuplex &http)
	{
		auto uri = http::Uri(http.req.uri);
		auto const &path = uri.path();

		if (validatePath(path, http.resp) == false)
		{
			return false;
		}

		uri.setPath(m_root + path);
		http.req.uri = uri.toString();

		return true;
	}

	bool CustomRoot::validatePath(std::string_view path, api::HttpResponse &res) const
	{
		auto escape = false;
		auto count = 0;
		auto len = path.length();

		for (auto i = 0u; i < len; ++i)
		{
			switch (auto c = path[i]; c)
			{
			case '\\':
				escape = (escape == false);
				break;
			case '/':
				if (escape == false)
				{
					if (i <= len - 2 && path[i + 1] == '.' && path[i + 2] == '.')
					{
						--count;
					}
					else
					{
						++count;
					}
				}
				[[fallthrough]];
			default:
				escape = false;
				break;
			}
			if (count < 0)
			{
				res.version = api::http::Version::http_1_1;
				res.status = api::http::common_status::forbidden;
				res.reason = "Forbidden";

				return false;
			}
		}
		return true;
	}
}
