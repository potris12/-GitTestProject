#pragma once
#include "GlobalObject.h"

//�̳��� ���� ������ ������ �ֱ� ����..
//�ͷ����� ź����Ų ��ü�ν� ���� ��������� ������ set�ϱ� ���� ��ü�̴�.
//����, ��ü �������� ������ ������ �ش� ��ü�� ���� ������� �����͸� �������� �Ѵ�.
//�� ��ü�� ����ؼ� �ʿ��� ������ set�ϰ� setGlobalBufferinfo�Լ��� ����� �ȴ�.
struct TERRAIN_GLOBAL_VALUE {
	float OneSpaceSizeRcp{ 0 };// 1/�ϳ��� ������ ũ��
	float OneSideSpaceNumRcp{ 0 };// 1/���� ���̵忡 �ִ� ���� �� 
	float OneSideSpaceNum{ 0 };//1 / ���� ���̵忡 �ִ� ���� �� 
	float HeightScale{ 0 };//���̰� scale
};

class CGlobalTerrain : public CGlobalObject {
public:
	//----------------------------object-----------------------------
	bool Begin(float fOneSpaceSize, float fOneSidSpaceNum, float fHeightScale);
	virtual bool End();
	//----------------------------object-----------------------------

	//--------------------------globalobject--------------------------
	//virtual void SetBufferInfo(void** ppMappedResources, int& nInstance, shared_ptr<CCamera> pCamera);
	virtual void SetGlobalBufferInfo(void** ppMappedResources);
	virtual void SetGlobalInfo(void* pData);
	//--------------------------globalobject--------------------------

protected:
	TERRAIN_GLOBAL_VALUE* m_pGlobalTerrainData;

public:
	CGlobalTerrain();
	virtual ~CGlobalTerrain();
};