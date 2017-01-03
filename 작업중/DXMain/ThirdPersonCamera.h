#pragma once
#include "Camera.h"
#include "Player.h"


class CThirdPersonCamera :public CCamera {
public:
	//----------------------------dxobject-----------------------------
	virtual bool Begin();
	virtual bool End();

	virtual void SetShaderState();
	virtual void CleanShaderState() { }

	virtual void UpdateShaderState();//CCamera::UpdateShaderStateȣ�� �� �ؾ��Ѵ�. 
	//----------------------------dxobject-----------------------------
	
	
	//---------------------------------------ī�޶� ����---------------------------------
	//ȸ��
	virtual void Rotate(float x = 0.0f, float y = 0.0f, float z = 0.0f);
	//Ani ī�޶� �׼�

	//----------------------------------------���� �Ҹ� �Լ���------------------------
	//player����ٴϱ�
	virtual void Update(float fTimeElapsed);
	virtual void ProcessInput(float fTimeElapsed);
	//----------------------------------------���� �Ҹ� �Լ���------------------------
	//debug���� ī�޶� �׸��� 
	virtual void Render() {};
	//---------------------------------------ī�޶� ����---------------------------------

public:
	CThirdPersonCamera(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext);
	virtual ~CThirdPersonCamera();

};