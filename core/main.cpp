#include <cstdlib>
#include "ConfigLoader.hpp"

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
		auto const modules = ModuleLoader(confLoader.getConfiguration());
		// Create the pipeline
		while (1)
		{
			if (confLoader.shouldReload())
			{
				// TODO: Wait for current processing to end
				break;
			}
			// Process requests
		}
	}
	return EXIT_SUCCESS;
}
