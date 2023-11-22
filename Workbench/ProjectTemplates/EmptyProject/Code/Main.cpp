#include <iostream>

#ifdef GADGET_DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif //GADGET_DEBUG

class GameProjectInterface : public Gadget::GameInterface{
public:
	GameProjectInterface() : GameInterface("GameProject"){}

	virtual void LoadGame() final override{}
};

int main(int argc, char* argv[]){
	if(argc > 0){
		std::cout << "Launching with arguments: " << std::endl;
	}

	for(int i = 0; i < argc; i++){
		std::cout << argv[i] << std::endl;
	}

	try{
		GameProjectInterface gpi = GameProjectInterface();
		Gadget::App::GetInstance().Run(gpi);
	}catch(std::runtime_error e){
		_ASSERT(false);
	}

	#ifdef GADGET_DEBUG
	Gadget::App::DeleteInstance();
	std::cout << "The program is now complete." << std::endl;

	_CrtDumpMemoryLeaks();
	#endif //_DEBUG

	return 0;
}