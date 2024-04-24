#ifndef GADGET_BASIC_SCENE_MANAGER_H
#define GADGET_BASIC_SCENE_MANAGER_H

#include "Game/Scene.h"

namespace Gadget{
	class BasicSceneManager{
	public:
		BasicSceneManager();
		~BasicSceneManager();
		DISABLE_COPY_AND_MOVE(BasicSceneManager);

		void Update(float deltaTime_);

		void AddScene(Scene* scene_);

		//Immediately load the desired scene - Avoid calling this directly unless you've considered the consequences!
		void LoadScene(size_t index_);
		void LoadScene(StringID name_);

		//Request for a new scene to be loaded when it's safe to do so
		void RequestSceneLoad(size_t index_);
		void RequestSceneLoad(StringID name_);

		Scene* CurrentScene();
		inline constexpr size_t GetNumScenes(){ return scenes.size(); }

	private:
		std::vector<Scene*> scenes;
		size_t sceneIndex;
		std::queue<size_t> sceneLoadRequests;
	};
}

#endif //!GADGET_BASIC_SCENE_MANAGER_H