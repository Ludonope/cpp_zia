#pragma once

#if defined(__linux__) || (__APPLE__)
#include <dlfcn.h>
#elif defined(_WIN32)
#include <Windows.h>
#endif

#include <string>
#include <functional>

namespace zia::core
{
	// Uniform type declaration
#if defined(__linux__) || (__APPLE__)
	using gen_lib_t = void *;
#elif defined(_WIN32)
	using gen_lib_t = HINSTANCE;
#else
#error "Unsupported dynamic library format"
#endif

	class LibLoader
	{
	public:
		LibLoader() = default;
		LibLoader(std::string const &filename);
		~LibLoader();
		LibLoader(LibLoader const &other) = delete;
		LibLoader &operator=(LibLoader const &) = delete;
		LibLoader &operator=(LibLoader &&);
		LibLoader(LibLoader &&);
		void load(std::string const &filename);

		template <typename T>
		std::function<T> getFunction(std::string const &sym) const
		{
#if defined(__linux__) || (__APPLE__)
			std::function<T> func =
				reinterpret_cast<T *>(dlsym(m_libPtr,
							sym.c_str()));
#elif defined(_WIN32)
			std::function<T> func =
				reinterpret_cast<T *>(GetProcAddress(m_libPtr,
							sym.c_str()));
#endif
			return (func);
		}

	private:
		std::string m_filename = "";
		gen_lib_t   m_libPtr = nullptr;
	};
}
