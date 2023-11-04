#ifndef GADGET_SCENE_H
#define GADGET_SCENE_H

#include "GameObject.h"
#include "SceneComponent.h"
#include "Utils/GUID.h"

namespace Gadget{
	class Scene{
	public:
		Scene(StringID name_);
		virtual ~Scene();

		void CreateObject(GameObject* gameObject_);
		void AddSceneComponent(SceneComponent* sceneComp_);

		void Update(float deltaTime_);

		GUID GetGUID() const{ return guid; }
		StringID GetName() const{ return name; }

		GameObject* FindWithTag(StringID tag_);
		std::vector<GameObject*> FindObjectsWithTag(StringID tag_);

		//THIS FUNCTION IS SLOW - Avoid calling it unless necessary, and cache result when possible
		template <class T> T* GetComponentInScene() const{
			static_assert(std::is_base_of<Component, T>::value, "T must inherit from Component");
			for(const auto& go : gameObjects){
				T* t = go->GetComponent<T>();
				if(t != nullptr){
					return t;
				}
			}

			return nullptr;
		}

		//THIS FUNCTION IS SLOW - Avoid calling it unless necessary, and cache results when possible
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

		template <class T> T* GetSceneComponent() const{
			static_assert(std::is_base_of<SceneComponent, T>::value, "T must inherit from SceneComponent");
			T* comp = nullptr;

			//Performance Note: dynamic casts are pretty slow, especially when they fail which will happen a lot here
			//This seems to be the simplest way to do this generically, but one could optimize this on a per-project basis if necessary
			for(SceneComponent* c : sceneComponents){
				comp = dynamic_cast<T*>(c);
				if(comp != nullptr){
					return comp;
				}
			}

			return nullptr;
		}

	protected:
		friend class BasicSceneManager;

		GUID guid;
		StringID name;
		std::vector<GameObject*> gameObjects;
		std::vector<SceneComponent*> sceneComponents;

		virtual void SetToDefaultState(){
			DestroyAllGameObjects();
			DestroyAllSceneComponents();
		}

		void DestroyAllGameObjects();
		void DestroyAllSceneComponents();
	};
}

#endif //!GADGET_SCENE_H