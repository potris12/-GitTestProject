#include "stdafx.h"
#include "DirectionalLightMesh.h"



bool CDirectionalLightMesh::CreateVertexBuffer() {
	m_nVertices = 4;
	m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;

	m_ppd3dVertexBuffers = nullptr;
	//derectional light mesh�� ���� 0���� hlsl�ڵ忡�� �˾Ƽ� �����Ѵ�.
	
	float fx = SPACE_SIZE, fy = SPACE_SIZE, fz = SPACE_SIZE;

	//create space mesh aabb
	//BoundingBox::CreateFromPoints(m_BoundingBox, XMVectorSet(0.f, 0.f, 0.f, 0.f), XMVectorSet(fx, fy, fz, 0.f));
	BoundingBox::CreateFromPoints(m_BoundingBox, XMVectorSet(+10.f, +10.f, +10.f, 0.f), XMVectorSet(-10.f, -10.f, -10.f, 0.f));

	return true;
}
bool CDirectionalLightMesh::CreateIndexBuffer() {

	return true;
}

CDirectionalLightMesh::CDirectionalLightMesh(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext) : CMesh(pd3dDevice, pd3dDeviceContext) {

}
CDirectionalLightMesh ::~CDirectionalLightMesh() {

}