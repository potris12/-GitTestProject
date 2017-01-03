#pragma once
#include "stdafx.h"
#include "ThirdPersonCamera.h"

//DI
bool CThirdPersonCamera::Begin() {


	return CCamera::Begin();
}
bool CThirdPersonCamera::End() {


	return CCamera::End();
}

void CThirdPersonCamera::Rotate(float x, float y, float z) {
	XMMATRIX mtxRotate;
	XMFLOAT3 xmPosition;
	if (m_pPlayer && (x != 0.0f))
	{
		mtxRotate = XMMatrixRotationAxis(m_pPlayer->GetControlObject()->GetRight(), XMConvertToRadians(x));
		XMStoreFloat3(&m_xmf3Right, XMVector3TransformNormal(XMLoadFloat3(&m_xmf3Right), mtxRotate));
		XMStoreFloat3(&m_xmf3Up, XMVector3TransformNormal(XMLoadFloat3(&m_xmf3Up), mtxRotate));
		XMStoreFloat3(&m_xmf3Look, XMVector3TransformNormal(XMLoadFloat3(&m_xmf3Look), mtxRotate));

		XMStoreFloat3(&xmPosition, m_pPlayer->GetControlObject()->GetPosition());
		m_xmf3Pos.x -= xmPosition.x;
		m_xmf3Pos.y -= xmPosition.y;
		m_xmf3Pos.z -= xmPosition.z;
		XMStoreFloat3(&m_xmf3Pos, XMVector3TransformCoord(XMLoadFloat3(&m_xmf3Pos), mtxRotate));
		m_xmf3Pos.x += xmPosition.x;
		m_xmf3Pos.y += xmPosition.y;
		m_xmf3Pos.z += xmPosition.z;
	}
	if (m_pPlayer && (y != 0.0f))
	{
		mtxRotate = XMMatrixRotationAxis(m_pPlayer->GetControlObject()->GetUp(), XMConvertToRadians(y));
		XMStoreFloat3(&m_xmf3Right, XMVector3TransformNormal(XMLoadFloat3(&m_xmf3Right), mtxRotate));
		XMStoreFloat3(&m_xmf3Up, XMVector3TransformNormal(XMLoadFloat3(&m_xmf3Up), mtxRotate));
		XMStoreFloat3(&m_xmf3Look, XMVector3TransformNormal(XMLoadFloat3(&m_xmf3Look), mtxRotate));

		XMStoreFloat3(&xmPosition, m_pPlayer->GetControlObject()->GetPosition());
		m_xmf3Pos.x -= xmPosition.x;
		m_xmf3Pos.y -= xmPosition.y;
		m_xmf3Pos.z -= xmPosition.z;
		XMStoreFloat3(&m_xmf3Pos, XMVector3TransformCoord(XMLoadFloat3(&m_xmf3Pos), mtxRotate));
		m_xmf3Pos.x += xmPosition.x;
		m_xmf3Pos.y += xmPosition.y;
		m_xmf3Pos.z += xmPosition.z;
	}
	if (m_pPlayer && (z != 0.0f))
	{
		mtxRotate = XMMatrixRotationAxis(m_pPlayer->GetControlObject()->GetLook(), XMConvertToRadians(z));
		XMStoreFloat3(&m_xmf3Right, XMVector3TransformNormal(XMLoadFloat3(&m_xmf3Right), mtxRotate));
		XMStoreFloat3(&m_xmf3Up, XMVector3TransformNormal(XMLoadFloat3(&m_xmf3Up), mtxRotate));
		XMStoreFloat3(&m_xmf3Look, XMVector3TransformNormal(XMLoadFloat3(&m_xmf3Look), mtxRotate));

		XMStoreFloat3(&xmPosition, m_pPlayer->GetControlObject()->GetPosition());
		m_xmf3Pos.x -= xmPosition.x;
		m_xmf3Pos.y -= xmPosition.y;
		m_xmf3Pos.z -= xmPosition.z;
		XMStoreFloat3(&m_xmf3Pos, XMVector3TransformCoord(XMLoadFloat3(&m_xmf3Pos), mtxRotate));
		m_xmf3Pos.x += xmPosition.x;
		m_xmf3Pos.y += xmPosition.y;
		m_xmf3Pos.z += xmPosition.z;
	}
}

void CThirdPersonCamera::Update(float fTimeElapsed) {


	UpdateShaderState();
}

//������� ����
void CThirdPersonCamera::UpdateShaderState() {
	//���� ����
	XMMATRIX xmMtxProjection = (XMLoadFloat4x4(&m_xmf4x4Projection));
	UpdateViewMtx();
	XMMATRIX xmMtxView = XMLoadFloat4x4(&m_xmf4x4View);

	XMStoreFloat4x4(&m_stCameraBufferData.m_xmf4x4ViewProjection, XMMatrixTranspose(XMMatrixMultiply(xmMtxView, xmMtxProjection)));
	//���� ����

	//������� ������Ʈ
	m_pd3dDeviceContext->UpdateSubresource(m_pViewProjectionBuffer, 0, NULL, &m_stCameraBufferData, 0, 0);
}

void CThirdPersonCamera::SetShaderState() {
	//������� set
	m_pd3dDeviceContext->VSSetConstantBuffers(VS_CB_CAMERA, 1, &m_pViewProjectionBuffer);
}

void CThirdPersonCamera::ProcessInput(float fTimeElapsed) {
	DWORD dwDirection = 0;
	//UINT iMoveState;
	float cxDelta = INPUTMGR->GetcxDelta();
	float cyDelta = INPUTMGR->GetcyDelta();


	if (INPUTMGR->OnlyKeyBoardDown(VK_W))		dwDirection |= DIR_FORWARD;
	if (INPUTMGR->OnlyKeyBoardDown(VK_S))		dwDirection |= DIR_BACKWARD;
	if (INPUTMGR->OnlyKeyBoardDown(VK_A))		dwDirection |= DIR_LEFT;
	if (INPUTMGR->OnlyKeyBoardDown(VK_D))		dwDirection |= DIR_RIGHT;
	if (INPUTMGR->OnlyKeyBoardDown(VK_E))		dwDirection |= DIR_UP;
	if (INPUTMGR->OnlyKeyBoardDown(VK_Q))		dwDirection |= DIR_DOWN;

	//if (pKeyBuffer[VK_SHIFT] & 0xF0)	iMoveState = RUN;
	//else								iMoveState = WALK;

#define SPEED 500

	XMVECTOR xmvShift = XMVectorSet(0.0f, 0.0f, 0.0f, 0.f);

	if (dwDirection)
	{
		if (dwDirection & DIR_FORWARD)		xmvShift += XMLoadFloat3(&m_xmf3Look);
		if (dwDirection & DIR_BACKWARD)		xmvShift -= XMLoadFloat3(&m_xmf3Look);
		if (dwDirection & DIR_RIGHT)		xmvShift += XMLoadFloat3(&m_xmf3Right);
		if (dwDirection & DIR_LEFT)			xmvShift -= XMLoadFloat3(&m_xmf3Right);
		if (dwDirection & DIR_UP)			xmvShift += XMLoadFloat3(&m_xmf3Up);
		if (dwDirection & DIR_DOWN)			xmvShift -= XMLoadFloat3(&m_xmf3Up);

		XMVector3Normalize(xmvShift);

		//pos�� shift*speed*ftimeElapsed ������ = �ӵ���ŭ �̵�
		XMStoreFloat3(&m_xmf3Pos, XMLoadFloat3(&m_xmf3Pos) + ((xmvShift * SPEED) * fTimeElapsed));

	}

	if (cxDelta || cyDelta)
	{
		/*cxDelta�� y-���� ȸ���� ��Ÿ���� cyDelta�� x-���� ȸ���� ��Ÿ����. ������ ���콺 ��ư�� ������ ��� cxDelta�� z-���� ȸ���� ��Ÿ����.*/
		if (INPUTMGR->MouseRightDown())
			Rotate(cyDelta, 0.0f, -cxDelta);
		else
			Rotate(cyDelta, cxDelta, 0.0f);
	}

}

CThirdPersonCamera::CThirdPersonCamera(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext) : CCamera(pd3dDevice, pd3dDeviceContext) {

}
CThirdPersonCamera::~CThirdPersonCamera() {

}
