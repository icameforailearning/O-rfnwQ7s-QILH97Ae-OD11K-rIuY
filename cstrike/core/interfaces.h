#pragma once

#include "../common.h"
#include "d3d11.h"
// used: globalvariables
#include "../sdk/interfaces/iglobalvars.h"

#pragma region sdk_definitons
#define GAME_RESOURCE_SERVICE_CLIENT CS_XOR("GameResourceServiceClientV00")
#define SOURCE2_CLIENT CS_XOR("Source2Client00")
#define SCHEMA_SYSTEM CS_XOR("SchemaSystem_00")
#define INPUT_SYSTEM_VERSION CS_XOR("InputSystemVersion00")
#define SOURCE2_ENGINE_TO_CLIENT CS_XOR("Source2EngineToClient00")
#define ENGINE_CVAR CS_XOR("VEngineCvar00")
#define LOCALIZE CS_XOR("Localize_00")
#define NETWORK_CLIENT_SERVICE CS_XOR("NetworkClientService_00")
#define MATERIAL_SYSTEM2 CS_XOR("VMaterialSystem2_00")
#define RESOURCE_SYSTEM CS_XOR("ResourceSystem013")
#define RESOURCE_HANDLE_UTILS CS_XOR("ResourceHandleUtils001")
#define C_PREDICTION CS_XOR("Source2ClientPrediction001")
#define PARTICLE_MGR CS_XOR("ParticleSystemMgr003")

// @source: master/game/shared/shareddefs.h
#define TICK_INTERVAL 0.015625f
#define TIME_TO_TICKS(TIME) (static_cast<int>(0.5f + static_cast<float>(TIME) / TICK_INTERVAL))
#define TICKS_TO_TIME(TICKS) (TICK_INTERVAL * static_cast<float>(TICKS))
#define ROUND_TO_TICKS(TIME) (TICK_INTERVAL * TIME_TO_TICKS(TIME))
#define TICK_NEVER_THINK (-1)
#pragma endregion

// game interfaces
class ISwapChainDx11;
class IMemAlloc;
class CCSGOInput;
class ISchemaSystem;
class IInputSystem;
class IGameResourceService;
class ISource2Client;
class IEngineClient;
class IEngineCVar;
class INetworkClientService;
class IMaterialSystem2;
class IResourceSystem;
class CResourceHandleUtils;
class CPVS;
class CGameTraceManager;
class CGameRules;
class CGameEventManager;
class c_game_particle_manager_system;
class c_particle_manager;
class CPanoramaUIEngine;
class CPrediction;
class CInterfaceRegister;
// [d3d] struct
struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11RenderTargetView;

class CLocalize
{
public:
	auto FindSafe(const char* tokenName)
	{
		return MEM::CallVFunc<const char*, 17U>(this, tokenName);
	}
};

namespace I
{
	inline const CInterfaceRegister* GetRegisterList(const wchar_t* wszModuleName);
	bool Setup();

	/* @section: helpers */
	// create and destroy render target view for handling resize
	void CreateRenderTarget();
	void DestroyRenderTarget();

	inline CPrediction* Prediction = nullptr;
	inline CPanoramaUIEngine* PanoramaUIEngine = nullptr;
	inline c_particle_manager* ParticleManager = nullptr;
	inline c_game_particle_manager_system* GameParticleManagerSystem = nullptr;
	inline CGameEventManager* GameEventManager = nullptr;
	inline ID3D11ShaderResourceView* Maintexture = nullptr;
	inline CGameRules* GameRules = nullptr;
	inline CLocalize* Localize = nullptr;
	inline IMemAlloc* MemAlloc = nullptr;
	inline ISwapChainDx11* SwapChain = nullptr;
	inline ID3D11Device* Device = nullptr;
	inline ID3D11DeviceContext* DeviceContext = nullptr;
	inline ID3D11RenderTargetView* RenderTargetView = nullptr;
	inline CCSGOInput* Input = nullptr;
	inline ISchemaSystem* SchemaSystem = nullptr;
	inline IGlobalVars* GlobalVars = nullptr;
	inline IInputSystem* InputSystem = nullptr;
	inline IGameResourceService* GameResourceService = nullptr;
	inline ISource2Client* Client = nullptr;
	inline IEngineClient* Engine = nullptr;
	inline IEngineCVar* Cvar = nullptr;
	inline INetworkClientService* NetworkClientService = nullptr;
	inline IMaterialSystem2* MaterialSystem2 = nullptr;
	inline IResourceSystem* ResourceSystem = nullptr;
	inline CResourceHandleUtils* ResourceHandleUtils = nullptr;
	inline CPVS* PVS = nullptr;
	inline CGameTraceManager* GameTraceManager = nullptr;

	template<typename T>
	T* Capture(const CInterfaceRegister* pModuleRegister, const char* szInterfaceName);
}
