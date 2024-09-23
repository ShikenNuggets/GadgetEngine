#ifndef GADGET_SCENE_H
#define GADGET_SCENE_H

#include "Data/Array.h"
#include "Game/GameObject.h"
#include "Game/SceneComponent.h"
#include "Utils/GUID.h"

namespace Gadget{
	class Scene{
	public:
		Scene(StringID name_);
		virtual ~Scene();

		void CreateObject(GameObject* gameObject_);
		void AddSceneComponent(SceneComponent* sceneComp_);

		virtual void Update(float deltaTime_);

		GUID GetGUID() const{ return guid; }
		StringID GetName() const{ return name; }

		GameObject* FindWithTag(StringID tag_);
		std::vector<GameObject*> FindObjectsWithTag(StringID tag_);

		//If T doesn't have a ComponentCollection, this function will be significantly slower
		template <class T> T* GetComponentInScene() const{
			static_assert(std::is_base_of_v<Component, T>, "T must inherit from Component");
			if constexpr(HasComponentCollection<T>){
				return T::GetCollection().GetAny();
			}else{
				GADGET_LOG_WARNING(SID("PERFORMANCE"), typeid(T).name() + std::string(" does not have a Component Collection. You can optimize Scene::GetComponentInScene by giving it one"));
				for(const auto& go : gameObjects){
					T* t = go->GetComponent<T>();
					if(t != nullptr){
						return t;
					}
				}

				return nullptr;
			}
		}

		//If T doesn't have a ComponentCollection, this function will be significantly slower
		//Prefer the overload that takes a preallocated Array to reduce copying and memory allocations
		template <class T> Array<T*> GetAllComponentsInScene() const{
			static_assert(std::is_base_of_v<Component, T>, "T must inherit from Component");
			if constexpr(HasComponentCollection<T>){
				return T::GetCollection().GetAllComponents();
			}else{
				GADGET_LOG_WARNING(SID("PERFORMANCE"), typeid(T).name() + std::string(" does not have a Component Collection. You can optimize Scene::GetAllComponentsInScene by giving it one"));
			
				Array<T*> comps;
				for(const auto& go : gameObjects){
					//TODO - This assumes only one of each kind of component on an object
					T* t = go->GetComponent<T>();
					if(t != nullptr){
						comps.Add(t);
					}
				}

				comps.ShrinkToFit();
				return comps;
			}
		}

		//If T doesn't have a ComponentCollection, this function will be significantly slower
		template <class T> void GetAllComponentsInScene(Array<T*>& inBuffer_) const{
			GADGET_ASSERT(inBuffer_.IsEmpty(), "Non-empty std::vector passed to GetAllComponentsInScene, existing data will be lost!");
			inBuffer_.Clear();
			inBuffer_.Reserve(gameObjects.size());

			if constexpr(HasComponentCollection<T>){
				T::GetCollection().GetAllComponents(inBuffer_);
				return;
			}else{
				GADGET_LOG_WARNING(SID("PERFORMANCE"), typeid(T).name() + std::string(" does not have a Component Collection. You can optimize Scene::GetAllComponentsInScene by giving it one"));
			
				for(const auto& go : gameObjects){
					//TODO - This assumes only one of each kind of component on an object
					T* t = go->GetComponent<T>();
					if(t != nullptr){
						inBuffer_.Add(t);
					}
				}
			}
		}

		template <class T> T* GetSceneComponent() const{
			static_assert(std::is_base_of_v<SceneComponent, T>, "T must inherit from SceneComponent");
			T* comp = nullptr;

			//Performance Note: dynamic casts are pretty slow, especially when they fail which will happen a lot here
			//This seems to be the simplest way to do this generically, but there are ways to avoid this
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
			OnShutdown();
		}

		virtual void OnShutdown(){
			DestroyAllGameObjects();
			DestroyAllSceneComponents();
		}

		void DestroyAllGameObjects();
		void DestroyAllSceneComponents();
	};
}

#endif //!GADGET_SCENE_H