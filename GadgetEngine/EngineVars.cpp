#include "EngineVars.h"

using namespace Gadget;

const StringID EngineVars::Core::sectionName = SID("Engine.Core");
const StringID EngineVars::Core::languageKey = SID("Language");

const StringID EngineVars::Display::sectionName = SID("Engine.Display");
const StringID EngineVars::Display::fullscreenKey = SID("Fullscreen");
const StringID EngineVars::Display::displayWidthKey = SID("DisplaySizeX");
const StringID EngineVars::Display::displayHeightKey = SID("DisplaySizeY");
const StringID EngineVars::Display::lastWindowXKey = SID("LastWindowX");
const StringID EngineVars::Display::lastWindowYKey = SID("LastWindowY");
const StringID EngineVars::Display::targetFPSKey = SID("TargetFPS");
const StringID EngineVars::Display::vsyncKey = SID("Vsync");

const StringID EngineVars::Physics::sectionName = SID("Engine.Physics");
const StringID EngineVars::Physics::gravityConstantKey = SID("GravityConstant");