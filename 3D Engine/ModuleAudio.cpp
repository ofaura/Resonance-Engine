#include "ModuleAudio.h"

#include "Wwise/AK/SoundEngine/Common/AkSoundEngine.h"
#include "Wwise/AK/SoundEngine/Common/AkModule.h"
#include "Wwise/AK/MusicEngine/Common/AkMusicEngine.h"
#include "Wwise/AK/Tools/Common/AkLock.h"
#include "Wwise/AK/Tools/Common/AkMonitorError.h"
#include "Wwise/AK/Tools/Common/AkPlatformFuncs.h"
#include "Wwise/AkDefaultIOHookBlocking.h"
#include "Wwise/AK/SoundEngine/Common/AkStreamMgrModule.h"
#include "Wwise//AK/Comm/AkCommunication.h"

#define BANKNAME_INIT "Init.bnk"

using namespace AK;

ModuleAudio::ModuleAudio(bool start_enabled) : Module("Audio", start_enabled) {}

ModuleAudio::~ModuleAudio() {}

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
