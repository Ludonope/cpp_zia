#include <iostream>
#include <sstream>
#include "LibLoader.hpp"

namespace zia::core
{
	LibLoader::LibLoader(std::string const &filename)
		: m_filename(filename), m_libPtr(nullptr)
	{
		this->load(filename);
	}

	LibLoader::~LibLoader()
	{
		if (m_libPtr)
		{
#if defined(__linux__) || (__APPLE__)
			dlclose(m_libPtr);
#elif defined(_WIN32)
			FreeLibrary(m_libPtr);
#endif
			m_libPtr = nullptr;
		}
	}

	LibLoader &LibLoader::operator=(LibLoader &&other)
	{
		if (&other != this)
		{
			m_filename = other.m_filename;
			m_libPtr = other.m_libPtr;
			other.m_libPtr = nullptr;
		}
		return (*this);
	}

	LibLoader::LibLoader(LibLoader &&other)
		: m_filename(other.m_filename), m_libPtr(other.m_libPtr)
	{
		other.m_libPtr = nullptr;
	}

	void LibLoader::load(std::string const &filename)
	{
		m_filename = filename;
#if defined(__linux__) || (__APPLE__)
		m_libPtr = dlopen(m_filename.c_str(), RTLD_NOW | RTLD_GLOBAL);
		if (!m_libPtr)
		{
			throw std::runtime_error("Cannot load '" + m_filename +
				"' ! Error : " + dlerror());
		}
#elif defined(_WIN32)
		m_libPtr = LoadLibraryA(m_filename.c_str());
		if (!m_libPtr)
		{
			std::stringstream ss;

			ss << "Cannot load '" << m_filename
				<< "' ! Error : " << GetLastError();
			throw std::runtime_error(ss.str());
		}
#endif
	}
}
