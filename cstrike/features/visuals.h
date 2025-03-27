#pragma once

class CMeshData;

namespace F::VISUALS
{
	bool Setup();
	void OnDestroy();
	bool OnDrawObject(void* pAnimatableSceneObjectDesc, void* pDx11, CMeshData* arrMeshDraw, int nDataCount, void* pSceneView, void* pSceneLayer, void* pUnk, void* pUnk2);
}
