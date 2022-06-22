#include <iostream>

#include <App.h>

int main(int argc, char* argv[]){
	try{
		Gadget::App::GetInstance()->Run();
	}catch(std::runtime_error e){
		_ASSERT(false);
	}

#ifdef _DEBUG
	Gadget::App::DeleteInstance();
	std::cout << "The program is now complete." << std::endl;
#endif //_DEBUG
	return 0;
}