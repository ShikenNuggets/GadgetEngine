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

			std::map<StringID, Var> vars = {
				{ fullscreenKey, false },
				{ displayWidthKey, 1920 },
				{ displayHeightKey, 1080 },
				{ lastWindowXKey, 0 },
				{ lastWindowYKey, 0 },
			};
		};

		Core core;
		Display display;

		constexpr Var GetValue(StringID key_) const{
			if(Utils::ContainsKey(core.vars, key_)){
				return core.vars.at(key_);
			}

			if(Utils::ContainsKey(display.vars, key_)){
				return display.vars.at(key_);
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
			}else{
				GADGET_ASSERT(false, "Unrecognized EngineVars section[" + section_.GetString() + "]!");
			}
		}
	};
}

#endif //!GADGET_ENGINE_VARS_H