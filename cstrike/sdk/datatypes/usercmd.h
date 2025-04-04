#pragma once

// used: QAngle_t
#include "qangle.h"
// used: MEM_PAD
#include "../../utilities/memory.h"
// used: memalloc
#include "../../core/interfaces.h"
#include "../interfaces/imemalloc.h"
#include "../../utilities/log.h"

// @source: server.dll
enum ECommandButtons : std::uint64_t
{
	IN_ATTACK = 1 << 0,
	IN_JUMP = 1 << 1,
	IN_DUCK = 1 << 2,
	IN_FORWARD = 1 << 3,
	IN_BACK = 1 << 4,
	IN_USE = 1 << 5,
	IN_LEFT = 1 << 7,
	IN_RIGHT = 1 << 8,
	IN_MOVELEFT = 1 << 9,
	IN_MOVERIGHT = 1 << 10,
	IN_SECOND_ATTACK = 1 << 11,
	IN_RELOAD = 1 << 13,
	IN_SPRINT = 1 << 16,
	IN_JOYAUTOSPRINT = 1 << 17,
	IN_SHOWSCORES = 1ULL << 33,
	IN_ZOOM = 1ULL << 34,
	IN_LOOKATWEAPON = 1ULL << 35
};

// compiled protobuf messages and looked at what bits are used in them
enum ESubtickMoveStepBits : std::uint32_t
{
	MOVESTEP_BITS_BUTTON = 0x1U,
	MOVESTEP_BITS_PRESSED = 0x2U,
	MOVESTEP_BITS_WHEN = 0x4U,
	MOVESTEP_BITS_ANALOG_FORWARD_DELTA = 0x8U,
	MOVESTEP_BITS_ANALOG_LEFT_DELTA = 0x10U
};

enum EInputHistoryBits : std::uint32_t
{
	INPUT_HISTORY_BITS_VIEWANGLES = 0x1U,
	INPUT_HISTORY_BITS_SHOOTPOSITION = 0x2U,
	INPUT_HISTORY_BITS_TARGETHEADPOSITIONCHECK = 0x4U,
	INPUT_HISTORY_BITS_TARGETABSPOSITIONCHECK = 0x8U,
	INPUT_HISTORY_BITS_TARGETANGCHECK = 0x10U,
	INPUT_HISTORY_BITS_CL_INTERP = 0x20U,
	INPUT_HISTORY_BITS_SV_INTERP0 = 0x40U,
	INPUT_HISTORY_BITS_SV_INTERP1 = 0x80U,
	INPUT_HISTORY_BITS_PLAYER_INTERP = 0x100U,
	INPUT_HISTORY_BITS_RENDERTICKCOUNT = 0x200U,
	INPUT_HISTORY_BITS_RENDERTICKFRACTION = 0x400U,
	INPUT_HISTORY_BITS_PLAYERTICKCOUNT = 0x800U,
	INPUT_HISTORY_BITS_PLAYERTICKFRACTION = 0x1000U,
	INPUT_HISTORY_BITS_FRAMENUMBER = 0x2000U,
	INPUT_HISTORY_BITS_TARGETENTINDEX = 0x4000U
};

enum EButtonStatePBBits : uint32_t
{
	BUTTON_STATE_PB_BITS_BUTTONSTATE1 = 0x1U,
	BUTTON_STATE_PB_BITS_BUTTONSTATE2 = 0x2U,
	BUTTON_STATE_PB_BITS_BUTTONSTATE3 = 0x4U
};

enum EBaseCmdBits : std::uint32_t
{
	BASE_BITS_MOVE_CRC = 0x1U,
	BASE_BITS_BUTTONPB = 0x2U,
	BASE_BITS_VIEWANGLES = 0x4U,
	BASE_BITS_COMMAND_NUMBER = 0x8U,
	BASE_BITS_CLIENT_TICK = 0x10U,
	BASE_BITS_FORWARDMOVE = 0x20U,
	BASE_BITS_LEFTMOVE = 0x40U,
	BASE_BITS_UPMOVE = 0x80U,
	BASE_BITS_IMPULSE = 0x100U,
	BASE_BITS_WEAPON_SELECT = 0x200U,
	BASE_BITS_RANDOM_SEED = 0x400U,
	BASE_BITS_MOUSEDX = 0x800U,
	BASE_BITS_MOUSEDY = 0x1000U,
	BASE_BITS_CONSUMED_SERVER_ANGLE = 0x2000U,
	BASE_BITS_CMD_FLAGS = 0x4000U,
	BASE_BITS_ENTITY_HANDLE = 0x8000U
};

enum ECSGOUserCmdBits : uint32_t
{
	CSGOUSERCMD_BITS_BASECMD = 0x1U,
	CSGOUSERCMD_BITS_LEFTHAND = 0x2U,
	CSGOUSERCMD_BITS_PREDICTING_BODY_SHOT = 0x4U,
	CSGOUSERCMD_BITS_PREDICTING_HEAD_SHOT = 0x8U,
	CSGOUSERCMD_BITS_PREDICTING_KILL_RAGDOLLS = 0x10U,
	CSGOUSERCMD_BITS_ATTACK3START = 0x20U,
	CSGOUSERCMD_BITS_ATTACK1START = 0x40U,
	CSGOUSERCMD_BITS_ATTACK2START = 0x80U
};

template <typename T>
struct RepeatedPtrField_t
{
	struct Rep_t
	{
		int nAllocatedSize;
		T* tElements[(std::numeric_limits<int>::max() - 2 * sizeof(int)) / sizeof(void*)];
	};

	void* pArena;
	int nCurrentSize;
	int nTotalSize;
	Rep_t* pRep;
};

class CBasePB
{
public:
	MEM_PAD(0x8) // 0x0 VTABLE
	std::uint32_t nHasBits; // 0x8
	std::uint64_t nCachedBits; // 0xC

	void SetBits(std::uint64_t nBits)
	{
		// @note: you don't need to check if the bits are already set as bitwise OR will not change the value if the bit is already set
		nCachedBits |= nBits;
	}
};
static_assert(sizeof(CBasePB) == 0x18);

class CMsgQAngle : public CBasePB
{
public:
	QAngle_t angValue; // 0x18
};
static_assert(sizeof(CMsgQAngle) == 0x28);

class CMsgVector : public CBasePB
{
public:
	Vector4D_t vecValue; // 0x18
};
static_assert(sizeof(CMsgVector) == 0x28);

class CCSGOInterpolationInfoPB : public CBasePB
{
public:
	float flFraction; // 0x18
	int nSrcTick; // 0x1C
	int nDstTick; // 0x20
};
class CCSGOInterpolationInfoPB_CL : public CBasePB {
public:
	float m_flFraction; // 0x18
};
static_assert(sizeof(CCSGOInterpolationInfoPB_CL) == 0x20);
static_assert(sizeof(CCSGOInterpolationInfoPB) == 0x28);

class CCSGOInputHistoryEntryPB : public CBasePB
{
public:
	CMsgQAngle* pViewAngles; // 0x18 also correct
	CCSGOInterpolationInfoPB_CL* cl_interp; // 0x20 CL interp at 0x20 in IDA
	CCSGOInterpolationInfoPB* sv_interp0; // 0x28 Interp here in ida
	CCSGOInterpolationInfoPB* sv_interp1; // 0x30 correct
	CCSGOInterpolationInfoPB* player_interp; // 0x38 correct.
	CMsgVector* pShootPosition; // 0x40 // correct
	CMsgVector* pTargetHeadPositionCheck; // 0x48 correct
	CMsgVector* pTargetAbsPositionCheck; // 0x50 correct
	CMsgQAngle* pTargetAngPositionCheck; // 0x58 correct
	//rest of this is fine
	int nRenderTickCount; // 0x60
	float flRenderTickFraction; // 0x64
	int nPlayerTickCount; // 0x68
	float flPlayerTickFraction; // 0x6C
	int nFrameNumber; // 0x70
	int nTargetEntIndex; // 0x74

	CCSGOInterpolationInfoPB_CL* create_CLinterp_message() {
		this->nCachedBits |= 1;
		DWORD64 a1;

		if ((nHasBits & 1) != 0) {
			a1 = *(DWORD64*)(nHasBits & 0xFFFFFFFFFFFFFFFC);
		}
		else {
			a1 = nHasBits & 0xFFFFFFFFFFFFFFFC;
		}

		using def = CCSGOInterpolationInfoPB_CL * (__fastcall)(DWORD64);
		static auto fn = reinterpret_cast<def*>(
			MEM::GetAbsoluteAddress(
				MEM::FindPattern(CLIENT_DLL, CS_XOR("E8 ? ? ? ? 48 89 43 20 48 8B 43 20 F3 0F 10 45")),
				0x1,
				0x0
			)
			);
		return fn(a1);
	}

	CCSGOInterpolationInfoPB* create_interp_message() {
		this->nCachedBits |= 1;
		DWORD64 a1;

		if ((nHasBits & 1) != 0) {
			a1 = *(DWORD64*)(nHasBits & 0xFFFFFFFFFFFFFFFC);
		}
		else {
			a1 = nHasBits & 0xFFFFFFFFFFFFFFFC;
		}

		using def = CCSGOInterpolationInfoPB * (__fastcall)(DWORD64);
		static auto fn = reinterpret_cast<def*>(
			MEM::GetAbsoluteAddress(
				MEM::FindPattern(CLIENT_DLL, CS_XOR("E8 ? ? ? ? 48 89 43 38 8B 45 EF")),
				0x1,
				0x0
			)
			);
		return fn(a1);
	}

	CMsgVector* create_vector_message() {
		this->nCachedBits |= 1;
		DWORD64 a1;

		if ((nHasBits & 1) != 0) {
			a1 = *(DWORD64*)(nHasBits & 0xFFFFFFFFFFFFFFFC);
		}
		else {
			a1 = nHasBits & 0xFFFFFFFFFFFFFFFC;
		}

		using def = CMsgVector * (__fastcall)(DWORD64);
		static auto fn = reinterpret_cast<def*>(
			MEM::GetAbsoluteAddress(
				MEM::FindPattern(CLIENT_DLL, CS_XOR("E8 ? ? ? ? 48 89 43 48 48 8B 4B 48")),
				0x1,
				0x0
			)
			);
		return fn(a1);
	}


	CMsgQAngle* create_qangle_message() {
		this->nCachedBits |= 1;
		DWORD64 a1;

		if ((nHasBits & 1) != 0) {
			a1 = *(DWORD64*)(nHasBits & 0xFFFFFFFFFFFFFFFC);
		}
		else {
			a1 = nHasBits & 0xFFFFFFFFFFFFFFFC;
		}

		using def = CMsgQAngle * (__fastcall)(DWORD64);
		static auto fn = reinterpret_cast<def*>(
			MEM::GetAbsoluteAddress(
				MEM::FindPattern(CLIENT_DLL, CS_XOR("E8 ? ? ? ? 49 89 46 40 49 8B 56 40")),
				0x1,
				0x0
			)
			);

		return fn(a1);
	}

	void setRenderTickCount(int renderTickCount) {
		if (!nRenderTickCount) return;
		nRenderTickCount = renderTickCount;
		SetBits(INPUT_HISTORY_BITS_RENDERTICKCOUNT);
	}

	void setRenderTickFraction(float frac) {
		if (!flRenderTickFraction) return;
		flRenderTickFraction = frac;
		SetBits(INPUT_HISTORY_BITS_RENDERTICKFRACTION);
	}

	void setPlayerTickCount(int playerTickCount) {
		if (!nPlayerTickCount) return;
		nPlayerTickCount = playerTickCount;
		SetBits(INPUT_HISTORY_BITS_PLAYERTICKCOUNT);
	}

	void setPlayerTickFraction(float frac) {
		if (!flPlayerTickFraction) return;
		flPlayerTickFraction = frac;
		SetBits(INPUT_HISTORY_BITS_PLAYERTICKFRACTION);
	}

	void setClInterp(int srctick, int dsttick, float frac = 0.f) {
		if (!cl_interp) return;
		cl_interp->m_flFraction = frac;
		SetBits(INPUT_HISTORY_BITS_CL_INTERP);
	}

	void setSVInterp0(int srctick, int dsttick, float frac = 0.f) {
		if (!sv_interp0) return;
		sv_interp0->nSrcTick = srctick;
		sv_interp0->nDstTick = dsttick;
		sv_interp0->flFraction = frac;
		SetBits(INPUT_HISTORY_BITS_SV_INTERP0);
	}

	void setSVInterp1(int srctick, int dsttick, float frac = 0.f) {
		if (!sv_interp1) return;
		sv_interp1->nSrcTick = srctick;
		sv_interp1->nDstTick = dsttick;
		sv_interp1->flFraction = frac;
		SetBits(INPUT_HISTORY_BITS_SV_INTERP1);
	}
	void SetShootPos(Vector4D_t val) {
		if (!pShootPosition) return;
		pShootPosition->vecValue = val;
		SetBits(INPUT_HISTORY_BITS_SHOOTPOSITION);
	}
	void setPlayerInterp(int srctick, int dsttick, float frac = 0.f) {
		if (!player_interp) return;
		player_interp->nSrcTick = srctick;
		player_interp->nDstTick = dsttick;
		player_interp->flFraction = frac;
		SetBits(INPUT_HISTORY_BITS_PLAYER_INTERP);
	}
	void SetViewAngles(QAngle_t angle) {
		if (!pViewAngles) return;
		pViewAngles->angValue = angle;
		pViewAngles->SetBits(7);
		SetBits(INPUT_HISTORY_BITS_VIEWANGLES);
	}

};
static_assert(sizeof(CCSGOInputHistoryEntryPB) == 0x78);

struct CInButtonStatePB : CBasePB
{
	std::uint64_t nValue;
	std::uint64_t nValueChanged;
	std::uint64_t nValueScroll;
};
static_assert(sizeof(CInButtonStatePB) == 0x30);

struct CSubtickMoveStep : CBasePB
{
public:
	std::uint64_t nButton;
	bool bPressed;
	float flWhen;
	float flAnalogForwardDelta;
	float flAnalogLeftDelta;
};
static_assert(sizeof(CSubtickMoveStep) == 0x30);

class CBaseUserCmdPB : public CBasePB
{
public:
	RepeatedPtrField_t<CSubtickMoveStep> subtickMovesField;
	std::string* strMoveCrc;
	CInButtonStatePB* pInButtonState;
	CMsgQAngle* pViewAngles;
	std::int32_t nLegacyCommandNumber;
	std::int32_t nClientTick;
	float flForwardMove;
	float flSideMove;// and leftmove
	float flUpMove;
	std::int32_t nImpulse;
	std::int32_t nWeaponSelect;
	std::int32_t nRandomSeed;
	std::int32_t nMousedX;
	std::int32_t nMousedY;
	std::uint32_t nConsumedServerAngleChanges;
	std::int32_t nCmdFlags;
	std::uint32_t nPawnEntityHandle;

	

	int CalculateCmdCRCSize()
	{
		return MEM::CallVFunc<int, 7U>(this);
	}
};

static_assert(sizeof(CBaseUserCmdPB) == 0x80);

class CCSGOUserCmdPB
{
public:
	std::uint32_t nHasBits;
	std::uint64_t nCachedSize;
	RepeatedPtrField_t<CCSGOInputHistoryEntryPB> inputHistoryField;
	CBaseUserCmdPB* pBaseCmd;
	bool bLeftHandDesired;
	bool bIsPredictingBodyShotFX;
	bool bIsPredictingHeadShotFX;
	bool bIsPredictingKillRagdolls;
	std::int32_t nAttack3StartHistoryIndex; //  [default = -1]
	std::int32_t nAttack1StartHistoryIndex; //  [default = -1]
	std::int32_t nAttack2StartHistoryIndex; //  [default = -1]

	void setAttack1StartHistoryIndex(int32_t index) {
		nAttack1StartHistoryIndex = index;
		CheckAndSetBits(CSGOUSERCMD_BITS_ATTACK1START);
	}

	void setAttack2StartHistoryIndex(int32_t index) {
		nAttack2StartHistoryIndex = index;
		CheckAndSetBits(CSGOUSERCMD_BITS_ATTACK2START);
	}

	void setAttack3StartHistoryIndex(int32_t index) {
		nAttack3StartHistoryIndex = index;
		CheckAndSetBits(CSGOUSERCMD_BITS_ATTACK3START);
	}

	// @note: this function is used to check if the bits are set and set them if they are not
	void CheckAndSetBits(std::uint32_t nBits)
	{
		if (!(nHasBits & nBits))
			nHasBits |= nBits;
	}
};
static_assert(sizeof(CCSGOUserCmdPB) == 0x40);

struct CInButtonState
{
public:
	MEM_PAD(0x8) // 0x0 VTABLE
	std::uint64_t nValue; // 0x8
	std::uint64_t nValueChanged; // 0x10
	std::uint64_t nValueScroll; // 0x18
};
static_assert(sizeof(CInButtonState) == 0x20);

class CUserCmd 
{
public:
	MEM_PAD(0x8); // 0x0 VTABLE
	MEM_PAD(0x10); // TODO: find out what this is, added 14.08.2024
	CCSGOUserCmdPB csgoUserCmd; // 0x18
	CInButtonState nButtons; // 0x58
	MEM_PAD(0x10);
	bool bHasBeenPredicted;
	MEM_PAD(0xF);

	void adjust_input_history(CCSGOInputHistoryEntryPB* tick, const QAngle_t& angView, int t, int target_idx, Vector_t eyepos, int tickbase)
	{

		if (!tick) {
			return;
		}

		t += 1;

		//if (!tick->pViewAngles) {
		//	tick->pViewAngles = tick->create_qangle_message();
		//}
		if (tick->pViewAngles) {
			tick->pViewAngles->angValue = angView;
			tick->nCachedBits |= INPUT_HISTORY_BITS_VIEWANGLES;
			tick->pViewAngles->nCachedBits |= 7;//???
		}

		// Update player_interp
		/*if (!tick->player_interp) {
			tick->player_interp = tick->create_interp_message();
		}
		if (tick->player_interp) {
			tick->player_interp->nSrcTick = t + 1;
			tick->player_interp->nDstTick = t;
			tick->player_interp->flFraction = 0.999f;
			tick->nCachedBits |= INPUT_HISTORY_BITS_PLAYER_INTERP;
		}*/

		//if (!tick->cl_interp) {
		//	tick->cl_interp = tick->create_CLinterp_message();
		//}
		//if (tick->cl_interp) {
		//	tick->cl_interp->m_flFraction = 0.999f;
		//	tick->nCachedBits |= INPUT_HISTORY_BITS_CL_INTERP;
		//}


		tick->nRenderTickCount = t + 1;
		tick->nCachedBits |= INPUT_HISTORY_BITS_RENDERTICKCOUNT;

		tick->nPlayerTickCount = tickbase;
		tick->nCachedBits |= INPUT_HISTORY_BITS_PLAYERTICKCOUNT;



		//if (!tick->sv_interp0) {
		//	tick->sv_interp0 = tick->create_interp_message();
		//}
		//if (tick->sv_interp0) {
		//	tick->sv_interp0->nSrcTick = t + 1;
		//	tick->sv_interp0->nDstTick = t;
		//	tick->sv_interp0->flFraction = 0.999f;
		//	tick->nCachedBits |= INPUT_HISTORY_BITS_SV_INTERP0;

		//}

		//if (!tick->sv_interp1) {
		//	tick->sv_interp1 = tick->create_interp_message();
		//}
		//if (tick->sv_interp1) {
		//	tick->sv_interp1->nSrcTick = t + 1;
		//	tick->sv_interp1->nDstTick = t;
		//	tick->sv_interp1->flFraction = 0.999f;
		//	tick->nCachedBits |= INPUT_HISTORY_BITS_SV_INTERP1;
		//}


		//if (!tick->pShootPosition) {
		//	tick->pShootPosition = tick->create_vector_message();
		//}
		if (tick->pShootPosition) {
			tick->pShootPosition->vecValue = Vector4D_t(eyepos.x, eyepos.y, eyepos.z, 0.0f);
			tick->nCachedBits |= INPUT_HISTORY_BITS_SHOOTPOSITION;
		}

		tick->nTargetEntIndex = target_idx;
		tick->nCachedBits |= INPUT_HISTORY_BITS_TARGETENTINDEX;
	}

	CCSGOInputHistoryEntryPB* CreateNewInputHistory(RepeatedPtrField_t<CCSGOInputHistoryEntryPB> rept_ptr, void* arena)
	{
		using fnCreateNewInputHistory = CCSGOInputHistoryEntryPB * (CS_FASTCALL*)(void*);
		static auto CreateNewInputHistory = reinterpret_cast<fnCreateNewInputHistory>(MEM::GetAbsoluteAddress(MEM::FindPattern(CLIENT_DLL, "E8 ? ? ? ? 48 8B D0 48 8D 4E ? E8 ? ? ? ? 4C 8B F8"), 0x1)); // E8 ? ? ? ? 48 8B D0 49 8D 4E ? E8 ? ? ? ? 4C 8B E0

		auto InputHistory = CreateNewInputHistory(arena);

		using fnAddElementToRepFieldContainer = CCSGOInputHistoryEntryPB * (CS_FASTCALL*)(RepeatedPtrField_t<CCSGOInputHistoryEntryPB>&, void*);
		static auto AddElementToRepFieldContainer = reinterpret_cast<fnAddElementToRepFieldContainer>(MEM::GetAbsoluteAddress(MEM::FindPattern(CLIENT_DLL, "E8 ? ? ? ? 4C 8B F8 48 8B 44 24 ? 4C 8B CF 48 89 44 24 ? 45 0F B6 C4 49 8B D7"), 0x1)); // E8 ? ? ? ? 4C 8B E0 48 8B 44 24 ? 4C 8B CF

		return AddElementToRepFieldContainer(rept_ptr, InputHistory);
	}

	CCSGOInputHistoryEntryPB* AddInputHistory()
	{
		auto ReptFieldInputHistory = this->csgoUserCmd.inputHistoryField;
		CCSGOInputHistoryEntryPB* InputHistoryEntry = nullptr;

		if (ReptFieldInputHistory.pRep && ReptFieldInputHistory.nCurrentSize < ReptFieldInputHistory.pRep->nAllocatedSize)
			InputHistoryEntry = ReptFieldInputHistory.pRep->tElements[ReptFieldInputHistory.nCurrentSize++];
		else
			InputHistoryEntry = CreateNewInputHistory(ReptFieldInputHistory, ReptFieldInputHistory.pArena);

		return InputHistoryEntry;
	}

	CCSGOInputHistoryEntryPB* GetInputHistoryEntry(int nIndex)
	{
		if (nIndex >= csgoUserCmd.inputHistoryField.pRep->nAllocatedSize || nIndex >= csgoUserCmd.inputHistoryField.nCurrentSize)
			return nullptr;

		return csgoUserCmd.inputHistoryField.pRep->tElements[nIndex];
	}

	CSubtickMoveStep* CreateSubtickStep(void* arena)
	{
		using fnCreateSubtickStep = CSubtickMoveStep * (CS_FASTCALL*)(void*);
		static auto fn = reinterpret_cast<fnCreateSubtickStep>(MEM::FindPattern(CLIENT_DLL, CS_XOR("40 53 48 83 EC ? 48 8B D9 48 85 C9 75 ? 8D 4B ? E8 ? ? ? ? 48 85 C0 74 ? 33 D2 45 33 C0 48 8B C8 E8 ? ? ? ? 48 83 C4 ? 5B C3 48 83 C4 ? 5B C3 4C 8D 05 ? ? ? ? BA ? ? ? ? E8 ? ? ? ? 48 8B D3 45 33 C0 48 8B C8 E8 ? ? ? ? 48 83 C4 ? 5B C3 CC CC CC CC CC CC CC 48 89 5C 24 ? 57 48 83 EC ? 33 FF 48 8B D9 48 85 C9 75 ? 8D 4B ? E8 ? ? ? ? 48 85 C0 74 ? 48 89 78 ? EB ? 48 8B C7 48 8B 5C 24 ? 48 83 C4 ? 5F C3 4C 8D 05 ? ? ? ? BA ? ? ? ? E8 ? ? ? ? 48 89 58 ? 48 8B 5C 24 ? 48 8D 0D ? ? ? ? 48 89 08 33 C9 48 89 48 ? 48 89 78 ? 48 83 C4 ? 5F C3 CC CC CC CC CC CC CC 48 89 5C 24 ? 56")));

		return fn(arena);
	}

	CSubtickMoveStep* CreateSubtick()
	{
		if (!csgoUserCmd.pBaseCmd)
			return nullptr;

		if (csgoUserCmd.pBaseCmd->subtickMovesField.pRep && (csgoUserCmd.pBaseCmd->subtickMovesField.nCurrentSize < csgoUserCmd.pBaseCmd->subtickMovesField.pRep->nAllocatedSize))
			return csgoUserCmd.pBaseCmd->subtickMovesField.pRep->tElements[csgoUserCmd.pBaseCmd->subtickMovesField.nCurrentSize++];

		auto new_subtick = CreateSubtickStep(csgoUserCmd.pBaseCmd->subtickMovesField.pArena);

		using fnCreateSubtick = CSubtickMoveStep * (CS_FASTCALL*)(RepeatedPtrField_t<CSubtickMoveStep>&, CSubtickMoveStep*);
		static auto fn = reinterpret_cast<fnCreateSubtick>(MEM::FindPattern(CLIENT_DLL, CS_XOR("48 89 5C 24 08 57 48 83 EC 20 48 8B D9 48 8B FA 48 8B 49 10")));

		return fn(csgoUserCmd.pBaseCmd->subtickMovesField, new_subtick);
	}	

	void AddSubtick(bool pressed, int button)
	{
		auto subtick = CreateSubtick();

		if (!subtick)
			return;

		subtick->nCachedBits = 7;
		subtick->nButton = button;
		subtick->bPressed = pressed;
		subtick->flWhen = 0.999f;
	}

	void SetSubTickAngle(const QAngle_t& angView)
	{
		for (int i = 0; i < this->csgoUserCmd.inputHistoryField.pRep->nAllocatedSize; i++)
		{
			CCSGOInputHistoryEntryPB* pInputEntry = this->GetInputHistoryEntry(i);
			if (!pInputEntry || !pInputEntry->pViewAngles)
				continue;

			pInputEntry->pViewAngles->angValue = angView;
			pInputEntry->SetBits(EInputHistoryBits::INPUT_HISTORY_BITS_VIEWANGLES);
		}

	}
};
static_assert(sizeof(CUserCmd) == 0x98);
