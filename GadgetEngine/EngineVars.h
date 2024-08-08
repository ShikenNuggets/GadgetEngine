#ifndef GADGET_ENGINE_VARS_H
#define GADGET_ENGINE_VARS_H

#include "LocManager.h"
#include "Utils/StringID.h"
#include "Utils/Utils.h"
#include "Utils/Var.h"

namespace Gadget{
	struct ConfigSection{
		ConfigSection(StringID name_) : name(name_), vars(){}

		const StringID name;
		std::map<StringID, Var> vars;
	};

	class EngineVars{
	public:
		struct Core : public ConfigSection{
			static const StringID sectionName;

			static const StringID languageKey;

			Core() : ConfigSection(sectionName){
				vars.emplace(languageKey, LocManager::DefaultLanguageKey);
			}
		};

		struct Display : public ConfigSection{
			static const StringID sectionName;

			static const StringID fullscreenKey;
			static const StringID displayWidthKey;
			static const StringID displayHeightKey;
			static const StringID lastWindowXKey;
			static const StringID lastWindowYKey;
			static const StringID targetFPSKey;
			static const StringID vsyncKey;

			Display() : ConfigSection(sectionName){
				vars.emplace(fullscreenKey, false);
				vars.emplace(displayWidthKey, 1920);
				vars.emplace(displayHeightKey, 1080);
				vars.emplace(lastWindowXKey, 0);
				vars.emplace(lastWindowYKey, 0);
				vars.emplace(targetFPSKey, 0); //0 is treated as no framerate cap
				vars.emplace(vsyncKey, false);
			}
		};

		struct Physics : public ConfigSection{
			static const StringID sectionName;

			static const StringID gravityConstantKey;
			static const StringID physicsUpdatesKey;

			Physics() : ConfigSection(sectionName){
				vars.emplace(gravityConstantKey, -9.81);
				vars.emplace(physicsUpdatesKey, 240);
			}
		};

		struct Render : ConfigSection{
			static const StringID sectionName;

			static const StringID gpuValidationKey;

			Render() : ConfigSection(sectionName){
				vars.emplace(gpuValidationKey, false);
			}
		};

		struct Audio : ConfigSection{
			static const StringID sectionName;

			static const StringID masterVolumeKey;
			static const StringID musicVolumeKey;
			static const StringID dialogueVolumeKey;
			static const StringID sfxVolumeKey;
			static const StringID ambientVolumeKey;

			Audio() : ConfigSection(sectionName){
				vars.emplace(masterVolumeKey, 1.0);
				vars.emplace(musicVolumeKey, 1.0);
				vars.emplace(dialogueVolumeKey, 1.0);
				vars.emplace(sfxVolumeKey, 1.0);
				vars.emplace(ambientVolumeKey, 1.0);

				GADGET_BASIC_ASSERT(vars.size() == static_cast<size_t>(VolumeChannel::VolumeChannel_MAX));
			}
		};

		std::vector<ConfigSection*> sections{
			new Core(),
			new Display(),
			new Physics(),
			new Render(),
			new Audio(),
		};

		constexpr Var GetValue(StringID key_) const{
			for(const auto& s : sections){
				GADGET_BASIC_ASSERT(s != nullptr);
				if(s == nullptr){
					Debug::Log(SID("CONFIG"), "nullptr found in config sections!", Debug::Error, __FILE__, __LINE__);
					continue;
				}

				if(Utils::ContainsKey(s->vars, key_)){
					return s->vars.at(key_);
				}
			}

			return Var(nullptr);
		}

		void SetValue(StringID key_, Var value_){
			for(const auto& s : sections){
				if(s == nullptr){
					Debug::Log(SID("CONFIG"), "nullptr found in config sections!", Debug::Error, __FILE__, __LINE__);
					continue;
				}

				if(Utils::ContainsKey(s->vars, key_)){
					s->vars.at(key_) = value_;
				}else{
					s->vars.emplace(key_, value_);
				}
			}
		}

		void SetValue(StringID section_, StringID key_, Var value_){
			for(const auto& s : sections){
				GADGET_BASIC_ASSERT(s != nullptr);
				if(s == nullptr){
					Debug::Log(SID("CONFIG"), "nullptr found in config sections!", Debug::Error, __FILE__, __LINE__);
					continue;
				}

				if(section_ == s->name){
					if(Utils::ContainsKey(s->vars, key_)){
						s->vars.at(key_) = value_;
					}else{
						s->vars.emplace(key_, value_);
					}

					return;
				}
			}

			GADGET_ASSERT(false, "Unrecognized EngineVars section[" + section_.GetString() + "]!");
			Debug::Log(SID("CONFIG"), "", Debug::Warning, __FILE__, __LINE__);
		}
	};
}

#endif //!GADGET_ENGINE_VARS_H