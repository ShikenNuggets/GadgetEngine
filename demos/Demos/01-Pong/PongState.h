#ifndef PONG_STATE_H
#define PONG_STATE_H

namespace Pong{
	enum class GameMode{
		SoloEasy,
		SoloHard,
		TwoPlayer
	};

	namespace PongState{
		static GameMode currentMode;
	};
}

#endif //!PONG_STATE_H