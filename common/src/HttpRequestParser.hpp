#pragma once

#include "api/http.h"
#include "api/net.h"

namespace zia::http
{
	api::HttpRequest parseRequest(std::string_view input);
	api::HttpRequest parseRequest(std::string_view input, std::size_t &bodyLen);
	api::HttpRequest parseRequest(api::Net::Raw const &input);
	api::HttpRequest parseRequest(std::byte const *input, std::size_t &bodyLen);
}
