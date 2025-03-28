#include "PlayerInventory.h"
#include "../cstrike/sdk/entity.h"
#include "../cstrike/utilities/memory.h"
#include "../cstrike/core/hooks.h"

static CGCClientSharedObjectTypeCache* CreateBaseTypeCache(
	CCSPlayerInventory* pInventory)
{
	if (MEM::fnGetClientSystem() == nullptr)
	{
		L_PRINT(LOG_ERROR) << ("failed to get client sys");
		return nullptr;
	}
	CGCClientSystem* pGCClientSystem = MEM::fnGetClientSystem();

	if (!pGCClientSystem)
		return nullptr;

	CGCClient* pGCClient = pGCClientSystem->GetCGCClient();
	if (!pGCClient)
		return nullptr;

	CGCClientSharedObjectCache* pSOCache =
		pGCClient->FindSOCache(pInventory->GetOwner());
	if (!pSOCache)
		return nullptr;

	return pSOCache->CreateBaseTypeCache(k_EEconTypeItem);
}

CCSPlayerInventory* CCSPlayerInventory::GetInstance()
{
	CCSInventoryManager* pInventoryManager = CCSInventoryManager::GetInstance();
	if (!pInventoryManager)
		return nullptr;

	return pInventoryManager->GetLocalInventory();
}

bool CCSPlayerInventory::AddEconItem(CEconItem* pItem)
{
	// Helper function to aid in adding items.
	if (!pItem)
		return false;

	CGCClientSharedObjectTypeCache* pSOTypeCache = ::CreateBaseTypeCache(this);
	if (!pSOTypeCache || !pSOTypeCache->AddObject((CSharedObject*)pItem))
		return false;

	SOCreated(GetOwner(), (CSharedObject*)pItem, eSOCacheEvent_Incremental);
	return true;
}

void CCSPlayerInventory::RemoveEconItem(CEconItem* pItem)
{
	// Helper function to aid in removing items.
	if (!pItem)
		return;

	CGCClientSharedObjectTypeCache* pSOTypeCache = ::CreateBaseTypeCache(this);
	if (!pSOTypeCache)
		return;

	const CUtlVector<CEconItem*>& pSharedObjects =
		pSOTypeCache->GetVecObjects<CEconItem*>();
	if (!pSharedObjects.Find(pItem))
		return;

	SODestroyed(GetOwner(), (CSharedObject*)pItem, eSOCacheEvent_Incremental);
	pSOTypeCache->RemoveObject((CSharedObject*)pItem);

	pItem->Destruct();
}

std::pair<uint64_t, uint32_t> CCSPlayerInventory::GetHighestIDs()
{
	uint64_t maxItemID = 0;
	uint32_t maxInventoryID = 0;

	CGCClientSharedObjectTypeCache* pSOTypeCache = ::CreateBaseTypeCache(this);
	if (pSOTypeCache)
	{
		const CUtlVector<CEconItem*>& vecItems =
			pSOTypeCache->GetVecObjects<CEconItem*>();

		for (CEconItem* it : vecItems)
		{
			if (!it)
				continue;

			// Checks if item is default.
			if ((it->m_ulID & 0xF000000000000000) != 0)
				continue;

			maxItemID = std::max(maxItemID, it->m_ulID);
			maxInventoryID = std::max(maxInventoryID, it->m_unInventory);
		}
	}

	return std::make_pair(maxItemID, maxInventoryID);
}

C_EconItemView* CCSPlayerInventory::GetItemViewForItem(uint64_t itemID)
{
	C_EconItemView* pEconItemView = nullptr;

	const CUtlVector<C_EconItemView*>& pItems = GetItemVector();
	for (C_EconItemView* it : pItems)
	{
		if (it && it->GetItemIDCS2() == itemID)
		{
			pEconItemView = it;
			break;
		}
	}

	return pEconItemView;
}

CEconItem* CCSPlayerInventory::GetSOCDataForItem(uint64_t itemID)
{
	CEconItem* pSOCData = nullptr;

	CGCClientSharedObjectTypeCache* pSOTypeCache = ::CreateBaseTypeCache(this);
	if (pSOTypeCache)
	{
		const CUtlVector<CEconItem*>& vecItems =
			pSOTypeCache->GetVecObjects<CEconItem*>();

		for (CEconItem* it : vecItems)
		{
			if (it && (int)it->m_ulID == (int)itemID)
			{
				pSOCData = it;
				return pSOCData;

				break;
			}
		}
	}

	return pSOCData;
}
