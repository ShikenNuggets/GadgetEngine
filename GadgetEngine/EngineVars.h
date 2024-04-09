#ifndef GADGET_ENGINE_VARS_H
#define GADGET_ENGINE_VARS_H

#include "Utils/Utils.h"
#include "Utils/StringID.h"
#include "Utils/Var.h"

namespace Gadget{
	struct EngineVars{
		struct Core{
			static const StringID sectionName;

			static const StringID languageKey;

			std::map<StringID, Var> vars = {
				{ languageKey, SID("ENG")},
			};
		};

		struct Display{
			static const StringID sectionName;

			static const StringID fullscreenKey;
			static const StringID displayWidthKey;
			static const StringID displayHeightKey;
			static const StringID lastWindowXKey;
			static const StringID lastWindowYKey;
			static const StringID targetFPSKey;
			static const StringID vsyncKey;

			std::map<StringID, Var> vars = {
				{ fullscreenKey, false },
				{ displayWidthKey, 1920 },
				{ displayHeightKey, 1080 },
				{ lastWindowXKey, 0 },
				{ lastWindowYKey, 0 },
				{ targetFPSKey, 0 }, //0 is treated as no framerate cap
				{ vsyncKey, false },
			};
		};

		struct Physics{
			static const StringID sectionName;

			static const StringID gravityConstantKey;
			static const StringID physicsUpdatesKey;

			std::map<StringID, Var> vars = {
				{ gravityConstantKey, -9.81 },
				{ physicsUpdatesKey, 240 }
			};
		};

		struct Render{
			static const StringID sectionName;

			static const StringID gpuValidationKey;

			std::map<StringID, Var> vars = {
				{ gpuValidationKey, false }
			};
		};

		Core core;
		Display display;
		Physics physics;
		Render render;

		constexpr Var GetValue(StringID key_) const{
			if(Utils::ContainsKey(core.vars, key_)){
				return core.vars.at(key_);
			}

			if(Utils::ContainsKey(display.vars, key_)){
				return display.vars.at(key_);
			}

			if(Utils::ContainsKey(physics.vars, key_)){
				return physics.vars.at(key_);
			}

			if(Utils::ContainsKey(render.vars, key_)){
				return render.vars.at(key_);
			}

			return Var(nullptr);
		}

		constexpr void SetValue(StringID section_, StringID key_, Var value_){
			if(section_ == Core::sectionName){
				if(Utils::ContainsKey(core.vars, key_)){
					core.vars.at(key_) = value_;
				}else{
					core.vars.emplace(key_, value_);
				}
			}else if(section_ == Display::sectionName){
				if(Utils::ContainsKey(display.vars, key_)){
					display.vars.at(key_) = value_;
				}else{
					display.vars.emplace(key_, value_);
				}
			}else if(section_ == Physics::sectionName){
				if(Utils::ContainsKey(physics.vars, key_)){
					physics.vars.at(key_) = value_;
				}else{
					physics.vars.emplace(key_, value_);
				}
			}else if(section_ == Render::sectionName){
				if(Utils::ContainsKey(render.vars, key_)){
					render.vars.at(key_) = value_;
				}else{
					render.vars.emplace(key_, value_);
				}
			}else{
				GADGET_ASSERT(false, "Unrecognized EngineVars section[" + section_.GetString() + "]!");
			}
		}
	};
}

#endif //!GADGET_ENGINE_VARS_H