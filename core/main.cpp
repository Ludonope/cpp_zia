#include <cstdlib>
#include <chrono>
#include <thread>
#include "ConfigLoader.hpp"
#include "ModuleManager.hpp"

int main(int ac, char *av[])
{
	if (ac != 2)
	{
		std::cout << "Usage: " << *av << " config_file" << std::endl;
		return EXIT_FAILURE;
	}
	auto const confFile = std::string(av[1]);
	zia::core::ConfigLoader		confLoader(confFile);

	while (1)
	{
		confLoader.load();
		auto modules = zia::core::ModuleManager(confLoader.getConfiguration());
		modules.start();
		while (1)
		{
			if (confLoader.shouldReload())
			{
				modules.stop();
				break;
			}
			using namespace std::chrono_literals;

			std::this_thread::sleep_for(250ms);
		}
	}
	return EXIT_SUCCESS;
}
