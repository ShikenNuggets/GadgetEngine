#include "BuildOptions.h"

#include <iostream>

#include <Utils/Utils.h>

using namespace DB;

BuildOptions::BuildOptions(int argc, char* argv[]){
	std::cout << "Checking arguments..." << std::endl;
	for(int i = 0; i < argc; i++){
		std::string arg = Gadget::Utils::ToLower(argv[i]);

		if(arg == "-f" || std::string(argv[i]) == "--forcerebuild"){
			std::cout << "Force Rebuild = true" << std::endl;
			forceRebuild = true;
		}

		//Handle other options here
	}
}