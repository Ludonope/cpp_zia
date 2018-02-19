#include <exception>
#include "PhpQuery.hpp"

namespace zia::php
{
	PhpQuery::PhpQuery(std::string_view query)
	{
		using std::operator""s;

		auto &self = *this;
		auto param = ""s;
		auto value = ""s;
		auto isParam = true;

		for (auto c : query)
		{
			if (isParam && c == '=')
			{
				isParam = false;
			}
			else if (!isParam && c == '&')
			{
				if (param == "")
				{
					std::invalid_argument("Empty parameter name in php query");
				}
				isParam = true;
				self[param] = value;
				param = "";
				value = "";
			}
			else if (isParam)
			{
				param += c;
			}
			else
			{
				value += c;
			}
		}

		if (isParam && (param != "" || this->size() != 0))
		{
			std::invalid_argument("Invalid php query");
		}

		if (param != "")
		{
			self[param] = value;
		}
	}
}