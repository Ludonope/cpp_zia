#pragma once

#include "api/http.h"

namespace zia::http
{
	std::string responseToString(api::HttpResponse const &res);
	api::Net::Raw responseToRaw(api::HttpResponse const &res);
}