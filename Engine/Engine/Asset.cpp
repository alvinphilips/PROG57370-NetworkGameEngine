#include "EngineCore.h"
#include "Asset.h"

#define NDEBUG_ASSET_INITIALIZE
#define NDEBUG_ASSET_LOAD

IMPLEMENT_ABSTRACT_CLASS(Asset)

void Asset::Initialize()
{
#ifdef DEBUG_ASSET_INITIALIZE
	LOG("Initializeing Asset: " << name);
#endif
}

void Asset::Load(json::JSON& json)
{
	// Initialize Asset name with filename.
	// This will be overriden in Object::Load() if a name was set.
	name = filepath;
	Object::Load(json);
#ifdef DEBUG_ASSET_LOAD
	LOG("Loading Asset: " << filepath);
#endif
}

void Asset::SetFilepath(std::string path)
{
	filepath = std::move(path);
}
