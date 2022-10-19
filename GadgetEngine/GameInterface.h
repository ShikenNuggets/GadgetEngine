#ifndef GADGET_GAME_INTERFACE_H
#define GADGET_GAME_INTERFACE_H

#include <string>

namespace Gadget{
	class GameInterface{
	public:
		GameInterface(const std::string& name_) : name(name_){}

		virtual void LoadGame() = 0;

	protected:
		std::string name;
	};
}

#endif //!GADGET_GAME_INTERFACE_H