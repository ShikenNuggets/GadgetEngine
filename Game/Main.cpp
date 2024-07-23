#include <iostream>

#pragma warning(disable : 26819) //Kill unfixable warning from SDL2
#include <SDL.h>
#pragma warning(default : 26819)

#ifdef GADGET_DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif //GADGET_DEBUG

#include <Gadget.h>
#include <Memory/GlobalAllocator.h>

#include "ExampleGame.h"
#include "Demos/01-Pong/PongGame.h"

using namespace Example;

int main(int argc, char* argv[]){
#ifdef GADGET_DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif // GADGET_DEBUG

	if(argc > 0){
		std::cout << "Launching with arguments: " << std::endl;
	}

	for(int i = 0; i < argc; i++){
		std::cout << argv[i] << std::endl;
	}

	ExampleGame game = ExampleGame();
	//Pong::PongGame game = Pong::PongGame();

	try{
		Gadget::App::GetInstance().Run(game);
	}catch(std::runtime_error e){
		_ASSERT(false);
	}

	Gadget::App::DeleteInstance();

#ifdef GADGET_DEBUG
	std::cout << Gadget::GlobalAllocator::GetNumAllocs() << " - " << Gadget::GlobalAllocator::GetNumFrees() << std::endl;
	std::cout << "Total time spent allocating memory: " << Gadget::GlobalAllocator::GetAllocTime() << " seconds" << std::endl;
	std::cout << "Total time spent freeing memory: " << Gadget::GlobalAllocator::GetFreeTime() << " seconds" << std::endl;

	std::cout << "The program is now complete." << std::endl;
#endif //_DEBUG

	return 0;
}