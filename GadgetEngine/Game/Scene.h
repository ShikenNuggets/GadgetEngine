#ifndef GADGET_SCENE_H
#define GADGET_SCENE_H

#include "GameObject.h"

namespace Gadget{
	class Scene{
	public:
		Scene(StringID name_);
		~Scene();

		void CreateObject(GameObject* gameObject_);

		StringID GetName() const{ return name; }

		//THIS FUNCTION IS SLOW
		template <class T> std::vector<T*> GetAllComponentsInScene() const{
			static_assert(std::is_base_of<Component, T>::value, "T must inherit from Component");

			std::vector<T*> comps;

			for(const auto& go : gameObjects){
				//TODO - This assumes only one of each kind of component on an object
				T* t = go->GetComponent<T>();
				if(t != nullptr){
					comps.push_back(t);
				}
			}

			return comps;
		}

	protected:
		friend class BasicSceneManager;

		StringID name;
		std::vector<GameObject*> gameObjects;

		virtual void SetToDefaultState();
	};
}

#endif //!GADGET_SCENE_H