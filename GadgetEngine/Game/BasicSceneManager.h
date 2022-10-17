#ifndef GADGET_BASIC_SCENE_MANAGER_H
#define GADGET_BASIC_SCENE_MANAGER_H

#include "Scene.h"

namespace Gadget{
	class BasicSceneManager{
	public:
		BasicSceneManager();
		~BasicSceneManager();

		void AddScene(Scene* scene_);

		void LoadScene(size_t index_);
		void LoadScene(StringID name_);

		Scene* CurrentScene();

	private:
		std::vector<Scene*> scenes;
		size_t sceneIndex;
	};
}

#endif //!GADGET_BASIC_SCENE_MANAGER_H