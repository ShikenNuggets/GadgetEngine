#include <iostream>

#pragma warning(disable : 26819) //Kill unfixable warning from SDL2
#include <SDL.h>
#pragma warning(default : 26819)

#include <App.h>

int main(int argc, char* argv[]){
	if(argc > 0){
		std::cout << "Launching with arguments: " << std::endl;
	}

	for(int i = 0; i < argc; i++){
		std::cout << argv[i] << std::endl;
	}

	try{
		Gadget::App::GetInstance()->Run();
	}catch(std::runtime_error e){
		_ASSERT(false);
	}

#ifdef GADGET_DEBUG
	Gadget::App::DeleteInstance();
	std::cout << "The program is now complete." << std::endl;
#endif //_DEBUG
	return 0;
}