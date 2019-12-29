#include "ModuleAudio.h"

#include "wwise.h"
#include "Wwise/IO/Win32/AkFilePackageLowLevelIOBlocking.h"
#include "Game\Assets\Sounds\Wwise_IDs.h"
#include "Application.h"

#define BANKNAME_INIT "Assets/Sounds/Init.bnk"

using namespace AK;

CAkDefaultIOHookBlocking g_lowLevelIO;

ModuleAudio::ModuleAudio(bool start_enabled) : Module("Audio", start_enabled) {}

ModuleAudio::~ModuleAudio() {}

bool ModuleAudio::Init(json file)
{
	InitWwise();

	LoadSoundBank("Main");

	return true;
}

bool ModuleAudio::Start()
{
	return true;
}

update_status ModuleAudio::PostUpdate(float dt)
{
	SoundEngine::RenderAudio();

	return UPDATE_CONTINUE;
}

bool ModuleAudio::CleanUp()
{
	TerminateWwise();
	return true;
}

void ModuleAudio::InitWwise()
{
	// Initialize audio engine
	// Memory.
	AkMemSettings memSettings;

	memSettings.uMaxNumPools = 20;

	// Streaming.
	AkStreamMgrSettings stmSettings;
	StreamMgr::GetDefaultSettings(stmSettings); 

	// Device Settings
	AkDeviceSettings deviceSettings;
	StreamMgr::GetDefaultDeviceSettings(deviceSettings);  

	// Sound Engine
	AkInitSettings l_InitSettings;
	AkPlatformInitSettings l_platInitSetings;
	SoundEngine::GetDefaultInitSettings(l_InitSettings);
	SoundEngine::GetDefaultPlatformInitSettings(l_platInitSetings);

	// Music Engine
	AkMusicSettings musicInit;
	MusicEngine::GetDefaultInitSettings(musicInit);

	// Create and initialise an instance of our memory manager.
	if (MemoryMgr::Init(&memSettings) != AK_Success)
	{
		assert(!"Could not create the memory manager.");
		return;
	}

	// Create and initialise an instance of the default stream manager.
	if (!StreamMgr::Create(stmSettings))
	{
		assert(!"Could not create the Stream Manager");
		return;
	}

	// Create an IO device.
	if (g_lowLevelIO.Init(deviceSettings) != AK_Success)
	{
		assert(!"Cannot create streaming I/O device");
		return;
	}

	// Initialize sound engine.
	if (SoundEngine::Init(&l_InitSettings, &l_platInitSetings) != AK_Success)
	{
		assert(!"Cannot initialize sound engine");
		return;
	}

	// Initialize music engine.
	if (MusicEngine::Init(&musicInit) != AK_Success)
	{
		assert(!"Cannot initialize music engine");
		return;
	}

#ifndef AK_OPTIMIZED
	// Initialize communication.
	AkCommSettings settingsComm;
	Comm::GetDefaultInitSettings(settingsComm);
	if (Comm::Init(settingsComm) != AK_Success)
	{
		assert(!"Cannot initialize music communication");
		return;
	}
#endif // AK_OPTIMIZED

	AkBankID bankID;
	AKRESULT retValue;
	retValue = SoundEngine::LoadBank(BANKNAME_INIT, AK_DEFAULT_POOL_ID, bankID);
	assert(retValue == AK_Success);
}

void ModuleAudio::TerminateWwise()
{
	// Terminate audio engine

#ifndef AK_OPTIMIZED
	// Terminate Communication Services
	Comm::Term();
#endif // AK_OPTIMIZED

	// Terminate Music Engine
	MusicEngine::Term();

	// Terminate Sound Engine
	SoundEngine::Term();

	// Terminate Streaming Manager
	g_lowLevelIO.Term();
	if (IAkStreamMgr::Get())
		IAkStreamMgr::Get()->Destroy();

	// Terminate Memory Manager
	MemoryMgr::Term();
}

void ModuleAudio::LoadSoundBank(const char * path)
{
	string fullPath = "Assets/Sounds/";
	fullPath += path;
	fullPath += ".bnk";

	AkBankID bankID;
	SoundEngine::LoadBank(fullPath.c_str(), AK_DEFAULT_POOL_ID, bankID);
}

void ModuleAudio::Tests(AkGameObjectID id)
{
	//SoundEngine::SetPanningRule(AkPanningRule_Speakers);
	//SoundEngine::SetVolumeThreshold();
	//SoundEngine::SetOutputVolume();
	//SoundEngine::SetRTPCValue();

	AkAuxSendValue reverb;
	reverb.listenerID = AK_INVALID_GAME_OBJECT;
	reverb.auxBusID = AK::AUX_BUSSES::REVERB;
	reverb.fControlValue = 1.0f;

	SoundEngine::SetGameObjectAuxSendValues(id, NULL, 0);
}

WwiseGameObject::WwiseGameObject(unsigned __int64 id, const char* name)
{
	this->id = id;
	this->name = name;

	SoundEngine::RegisterGameObj(this->id, this->name);
}

WwiseGameObject::~WwiseGameObject()
{
	SoundEngine::UnregisterGameObj(id);
}

void WwiseGameObject::SetPosition(float posX, float posY, float posZ, float frontX, float frontY, float frontZ, float topX, float topY, float topZ)
{
	position.X = -posX;
	position.Y = posY;
	position.Z = -posZ;

	orientationFront.X = frontX;
	orientationFront.Y = frontY;
	orientationFront.Z = frontZ;
	
	orientationTop.X = topX;
	orientationTop.Y = topY;
	orientationTop.Z = topZ;

	AkSoundPosition soundPosition;
	soundPosition.Set(position, orientationFront, orientationTop);
	SoundEngine::SetPosition(id, soundPosition);
}

void WwiseGameObject::PlayEvent(uint id)
{
	SoundEngine::PostEvent(id, this->id);
}

void WwiseGameObject::PauseEvent(uint id)
{
	SoundEngine::ExecuteActionOnEvent(id, AK::SoundEngine::AkActionOnEventType::AkActionOnEventType_Pause, this->id);
}

void WwiseGameObject::ResumeEvent(uint id)
{
	SoundEngine::ExecuteActionOnEvent(id, AK::SoundEngine::AkActionOnEventType::AkActionOnEventType_Resume, this->id);
}

void WwiseGameObject::StopEvent(uint id)
{
	SoundEngine::ExecuteActionOnEvent(id, AK::SoundEngine::AkActionOnEventType::AkActionOnEventType_Stop, this->id);
}

void WwiseGameObject::SetVolume(uint id, float volume)
{
	SoundEngine::SetGameObjectOutputBusVolume(this->id, AK_INVALID_GAME_OBJECT, volume);
	this->volume = volume;
}

WwiseGameObject* WwiseGameObject::CreateAudioSource(uint id, const char * name, vec3 position)
{
	WwiseGameObject* go = new WwiseGameObject(id, name);
	go->SetPosition(position.x, position.y, position.z);

	return go;
}

WwiseGameObject* WwiseGameObject::CreateAudioListener(uint id, const char * name, vec3 position)
{
	WwiseGameObject* go = new WwiseGameObject(id, name);

	AkGameObjectID listenerID = go->GetID();
	SoundEngine::SetDefaultListeners(&listenerID, 1);
	go->SetPosition(position.x, position.y, position.z);
	App->audio->currentListenerID = listenerID;
	return go;
}

void WwiseGameObject::SetAuxSends()
{
	AkAuxSendValue reverb[1];
	reverb[0].listenerID = AK_INVALID_GAME_OBJECT;
	reverb[0].auxBusID = AUX_BUSSES::REVERB;
	reverb[0].fControlValue = 1.0f;

	AKRESULT ret;
	ret = SoundEngine::SetGameObjectAuxSendValues(id, reverb, 1);
	assert(ret == AK_Success);
}

uint WwiseGameObject::GetID()
{
	return id;
}

