#include "stdafx.h"
#include "BoostMesh.h"

/*������ ������ ��������(Random) �����ϱ� ���� ����Ѵ�. �� ������ ������ ����(Random Number)�� �����Ͽ� �����Ѵ�.*/
#define RANDOM_COLOR XMFLOAT4(((rand() * 0xFFFFFF) / RAND_MAX),((rand() * 0xFFFFFF) / RAND_MAX),((rand() * 0xFFFFFF) / RAND_MAX),((rand() * 0xFFFFFF) / RAND_MAX))


#define WIDTH 5
#define HEIGHT 5
#define DEPTH 5

bool CBoostMesh::CreateVertexBuffer() {

	m_nVertices = 8;
	m_d3dPrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;

	float fx = WIDTH*0.5f, fy = HEIGHT*0.5f, fz = DEPTH*0.5f;

	//������ü �޽��� 2���� ���� ����(��ġ ���� ���ۿ� ���� ����)�� �����ȴ�.
	//������ü �޽��� ���� ����(��ġ ���� ����)�� �����Ѵ�.
	XMFLOAT3 pxmf3Positions[8];
	pxmf3Positions[0] = XMFLOAT3(-fx, +fy, -fz);
	pxmf3Positions[1] = XMFLOAT3(+fx, +fy, -fz);
	pxmf3Positions[2] = XMFLOAT3(+fx, +fy, +fz);
	pxmf3Positions[3] = XMFLOAT3(-fx, +fy, +fz);
	pxmf3Positions[4] = XMFLOAT3(-fx, -fy, -fz);
	pxmf3Positions[5] = XMFLOAT3(+fx, -fy, -fz);
	pxmf3Positions[6] = XMFLOAT3(+fx, -fy, +fz);
	pxmf3Positions[7] = XMFLOAT3(-fx, -fy, +fz);

	D3D11_BUFFER_DESC d3dBufferDesc;
	ZeroMemory(&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));
	d3dBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	d3dBufferDesc.ByteWidth = sizeof(XMFLOAT3) * m_nVertices;
	d3dBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	d3dBufferDesc.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA d3dBufferData;
	ZeroMemory(&d3dBufferData, sizeof(D3D11_SUBRESOURCE_DATA));
	d3dBufferData.pSysMem = pxmf3Positions;
	m_pd3dDevice->CreateBuffer(&d3dBufferDesc, &d3dBufferData, &m_pd3dPositionBuffer);

	//������ü �޽��� ���� ����(���� ����)�� �����Ѵ�.
	XMFLOAT4 pxmf4Colors[8];
	for (int i = 0; i < 8; i++) pxmf4Colors[i] = XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f);

	d3dBufferDesc.ByteWidth = sizeof(XMFLOAT4) * m_nVertices;
	d3dBufferData.pSysMem = pxmf4Colors;
	m_pd3dDevice->CreateBuffer(&d3dBufferDesc, &d3dBufferData, &m_pd3dColorBuffer);

	//-----------------------------------���� ����-----------------------------------
	ID3D11Buffer *pd3dBuffers[2] = { m_pd3dPositionBuffer, m_pd3dColorBuffer };
	UINT pnBufferStrides[2] = { sizeof(XMFLOAT3), sizeof(XMFLOAT4) };
	UINT pnBufferOffsets[2] = { 0, 0 };
	AssembleToVertexBuffer(2, pd3dBuffers, pnBufferStrides, pnBufferOffsets);
	//-----------------------------------���� ����-----------------------------------


	//create space mesh aabb
	BoundingBox::CreateFromPoints(m_BoundingBox, XMVectorSet(-fx, -fy, -fz, 0.f), XMVectorSet(fx, fy, fz, 0.f));


	if (m_ppd3dVertexBuffers)	return true;

	return false;
}
bool CBoostMesh::CreateIndexBuffer() {

	m_nIndices = 18;

	short pIndices[18];
	pIndices[0] = 5; //5,6,4 - cw
	pIndices[1] = 6; //6,4,7 - ccw
	pIndices[2] = 4; //4,7,0 - cw
	pIndices[3] = 7; //7,0,3 - ccw
	pIndices[4] = 0; //0,3,1 - cw
	pIndices[5] = 3; //3,1,2 - ccw
	pIndices[6] = 1; //1,2,2 - cw 
	pIndices[7] = 2; //2,2,3 - ccw
	pIndices[8] = 2; //2,3,3 - cw  - Degenerated Index(2)
	pIndices[9] = 3; //3,3,7 - ccw - Degenerated Index(3)
	pIndices[10] = 3;//3,7,2 - cw  - Degenerated Index(3)
	pIndices[11] = 7;//7,2,6 - ccw
	pIndices[12] = 2;//2,6,1 - cw
	pIndices[13] = 6;//6,1,5 - ccw
	pIndices[14] = 1;//1,5,0 - cw
	pIndices[15] = 5;//5,0,4 - ccw
	pIndices[16] = 0;
	pIndices[17] = 4;
	/*UINT pIndices[36];
	//�� �ո�(Front) �簢���� ���� �ﰢ��
	pIndices[0] = 3; pIndices[1] = 1; pIndices[2] = 0;
	//�� �ո�(Front) �簢���� �Ʒ��� �ﰢ��
	pIndices[3] = 2; pIndices[4] = 1; pIndices[5] = 3;
	//�� ����(Top) �簢���� ���� �ﰢ��
	pIndices[6] = 0; pIndices[7] = 5; pIndices[8] = 4;
	//�� ����(Top) �簢���� �Ʒ��� �ﰢ��
	pIndices[9] = 1; pIndices[10] = 5; pIndices[11] = 0;
	//�� �޸�(Back) �簢���� ���� �ﰢ��
	pIndices[12] = 3; pIndices[13] = 4; pIndices[14] = 7;
	//�� �޸�(Back) �簢���� �Ʒ��� �ﰢ��
	pIndices[15] = 0; pIndices[16] = 4; pIndices[17] = 3;
	//�� �Ʒ���(Bottom) �簢���� ���� �ﰢ��
	pIndices[18] = 1; pIndices[19] = 6; pIndices[20] = 5;
	//�� �Ʒ���(Bottom) �簢���� �Ʒ��� �ﰢ��
	pIndices[21] = 2; pIndices[22] = 6; pIndices[23] = 1;
	//�� ����(Left) �簢���� ���� �ﰢ��
	pIndices[24] = 2; pIndices[25] = 7; pIndices[26] = 6;
	//�� ����(Left) �簢���� �Ʒ��� �ﰢ��
	pIndices[27] = 3; pIndices[28] = 7; pIndices[29] = 2;
	//�� ����(Right) �簢���� ���� �ﰢ��
	pIndices[30] = 6; pIndices[31] = 4; pIndices[32] = 5;
	//�� ����(Right) �簢���� �Ʒ��� �ﰢ��
	pIndices[33] = 7; pIndices[34] = 4; pIndices[35] = 6;
	*/
	D3D11_BUFFER_DESC d3dBufferDesc;
	ZeroMemory(&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));
	d3dBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	d3dBufferDesc.ByteWidth = sizeof(UINT) * m_nIndices;
	d3dBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	d3dBufferDesc.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA d3dBufferData;
	ZeroMemory(&d3dBufferData, sizeof(D3D11_SUBRESOURCE_DATA));
	d3dBufferData.pSysMem = pIndices;
	m_pd3dDevice->CreateBuffer(&d3dBufferDesc, &d3dBufferData, &m_pd3dIndexBuffer);


	if (m_pd3dIndexBuffer) return true;

	return false;
}
CBoostMesh::CBoostMesh(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext) : CMesh(pd3dDevice, pd3dDeviceContext) {

}
CBoostMesh::~CBoostMesh() {

}