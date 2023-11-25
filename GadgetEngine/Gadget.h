#ifndef GADGET_GADGET_H
#define GADGET_GADGET_H

//Convenience include header for engine users
#include "App.h"
#include "Config.h"
#include "Debug.h"
#include "EngineVars.h"
#include "Random.h"

#include "Core/Time.h"

#include "Game/Component.h"
#include "Game/GameLogicComponent.h"
#include "Game/GameObject.h"
#include "Game/Scene.h"

#include "Graphics/Components/CameraComponent.h"
#include "Graphics/Components/LightComponent.h"
#include "Graphics/Components/RenderComponent.h"
#include "Graphics/GUI/CanvasSceneComponent.h"
#include "Graphics/GUI/GuiButton.h"
#include "Graphics/GUI/GuiCanvas.h"
#include "Graphics/GUI/GuiTextElement.h"
#include "Graphics/GUI/GuiTextureElement.h"
#include "Graphics/Materials/ColorMaterial.h"
#include "Graphics/Materials/DiffuseTextureMaterial.h"
#include "Graphics/Text/TextMesh.h"

#include "Input/Input.h"
#include "Input/InputGesture.h"

#include "Math/Angle.h"
#include "Math/Euler.h"
#include "Math/Math.h"
#include "Math/Quaternion.h"
#include "Math/Vector.h"

#include "Physics/BoxCollider2D.h"
#include "Physics/CubeCollider.h"
#include "Physics/Rigidbody.h"

#include "Resource/ResourceManager.h"

#include "Utils/GUID.h"
#include "Utils/StringID.h"
#include "Utils/Utils.h"
#include "Utils/Var.h"

#endif //!GADGET_GADGET_H