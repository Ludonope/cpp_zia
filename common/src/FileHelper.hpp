#pragma once

#include <string>
#include <fstream>

namespace zia::fs
{
	inline bool exists(std::string const & name) {
		std::ifstream f(name.c_str());
		return f.good();
	}
}