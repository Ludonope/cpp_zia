#pragma once

#include <string_view>
#include "api/conf.h"

namespace zia::conf
{
	api::Conf parseConf(std::string_view input);
	//api::Conf parseConfFile(std::string const &file);
}