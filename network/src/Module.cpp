#include "NetworkImpl.hpp"

#if defined _WIN32
#define ZIA_MODULE_API __declspec(dllexport)
#else
#define ZIA_MODULE_API
#endif

extern "C" {
	ZIA_MODULE_API zia::api::Net	*create() {
		return new zia::network::NetworkImpl();
	}

#if defined _WIN32
	BOOLEAN WINAPI DllMain(HINSTANCE hDllHandle,
		DWORD nReason, LPVOID Reserved)
	{
		return (true);
	}
#endif
}