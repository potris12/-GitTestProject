#pragma once
#include "Mesh.h"

#define START_MESH_INDEX 0

class CUseFBXMesh : public CMesh {

public:
	//----------------------------dxobject-----------------------------
	bool Begin(UINT index);
	virtual bool End();
	//----------------------------dxobject-----------------------------

	//---------------------------mesh----------------------------------
	//begin func
	virtual bool CreateVertexBuffer();
	virtual bool CreateIndexBuffer();
	void ProcessGetVertexBuffer(FbxNode* pNode);
	//begin func
	//---------------------------mesh----------------------------------
protected:
	//node
	FbxNode* m_pRootNode{ nullptr };
	//fbx data
	FbxManager *m_manager;
	FbxIOSettings *m_ioSettings;

	FbxImporter *m_importer{ nullptr };
	FbxScene *m_scene{ nullptr };

	int m_nMeshCnt{ 0 };
	int m_nIndex{ 0 };
private:
	//fbx data
	//get vertex

	XMFLOAT3* m_pNormals{ nullptr };
	int m_nNormals{ 0 };
	XMFLOAT2* m_pUVs{ nullptr };
	int m_nUVs{ 0 };
	
	//----------------------vertex buffers---------------------------
	ID3D11Buffer* m_pd3dPositionBuffer{ nullptr };
	ID3D11Buffer* m_pd3dNormalBuffer{ nullptr };
	ID3D11Buffer* m_pd3dTBuffer{ nullptr };
	ID3D11Buffer* m_pd3dBBuffer{ nullptr };
	ID3D11Buffer* m_pd3dUVBuffer{ nullptr };
	//----------------------vertex buffers---------------------------

	//helper func
	void GetPositionData(FbxMesh* pMesh);
	void GetNormalData(FbxMesh* pMesh);
	void GetUVData(FbxMesh* pMesh);
public:
	CUseFBXMesh(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext);
	virtual ~CUseFBXMesh();
};