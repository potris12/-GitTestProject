#pragma once
#include "ConstantBuffer.h"


struct TERRAIN_GLOBAL_VALUE {
	float OneSpaceSize{ 0 };//�ϳ��� ������ ũ��
	float OneSideSpaceNumRcp{ 0 };//1 / ���� ���̵忡 �ִ� ���� �� 
};

class CTerrainGlobalBuffer : public CConstantBuffer {
public:
	//object
	bool Begin(float fOneSpaceSize, float fOneSidSpaceNumRcp);
	virtual bool End();
	//object

	//---------------------globalbuffer-----------------------------------
	virtual void UpdateShaderState();
	virtual void SetData(void* pData);

private:
	//������ ����� �ְ�
	TERRAIN_GLOBAL_VALUE  m_value;


public:
	CTerrainGlobalBuffer(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext);
	~CTerrainGlobalBuffer();

};