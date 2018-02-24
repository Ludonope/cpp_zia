#include "api/module.h"
#include "PhpModule.hpp"

#if defined _WIN32
#include <Windows.h>
#define ZIA_MODULE_API __declspec(dllexport)
#else
#define ZIA_MODULE_API
#endif

extern "C" {
	ZIA_MODULE_API zia::api::Module *create() {
		return new zia::php::PhpModule();
	}

#if defined _WIN32
	BOOLEAN WINAPI DllMain(HINSTANCE hDllHandle,
		DWORD nReason, LPVOID Reserved)
	{
		return (true);
	}
#endif
}
