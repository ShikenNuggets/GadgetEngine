using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Workbench
{
    public static class CppTemplates
    {
//--------------------GameLogicComponent.h--------------------//
        public static readonly string GameLogicComponentH =
@"#ifndef {0}
#define {0}

#include ""Game/GameLogicComponent.h""

namespace {1}{{
	class {2} : public Gadget::GameLogicComponent{{
	public:
		{2}(Gadget::GameObject* parent_);

		virtual void OnStart() override;
		virtual void OnUpdate(float deltaTime_) override;
	}};
}}

#endif //!{0}";

//--------------------GameLogicComponent.cpp--------------------//
        public static readonly string GameLogicComponentCpp =
@"#include ""{0}.h""

using namespace {1};

{0}::{0}(Gadget::GameObject* parent_) : Gadget::GameLogicComponent(parent_){{
}}

void {0}::OnStart(){{
}}

void {0}::OnUpdate([[maybe_unused]] float deltaTime_){{
}}";
    }
}
