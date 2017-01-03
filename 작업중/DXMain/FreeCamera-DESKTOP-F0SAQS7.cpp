#pragma once
#include "stdafx.h"
#include "FreeCamera.h"

#include "stdafx.h"
#include "Camera.h"

//DI
bool CFreeCamera::Begin() {

	m_pGBufferUnpackingBuffer = CreateBuffer(sizeof(GBUFFER_UNPACKING_DATA), 1, nullptr, D3D11_BIND_CONSTANT_BUFFER, D3D11_USAGE_DEFAULT, 0);
	
	return CCamera::Begin();
}
bool CFreeCamera::End() {
	
	
	return CCamera::End();
}

void CFreeCamera::Rotate(float x, float y, float z) {
	XMMATRIX xmmtxRotate;
	if (x != 0.0f)
	{
		//ī�޶��� ���� x-���� �������� ȸ���ϴ� ����� �����Ѵ�. ���� �����̴� �����̴�.
		xmmtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&m_xmf3Right), (float)XMConvertToRadians(x));
		//ī�޶��� ���� x-��, y-��, z-���� ȸ���Ѵ�.
		XMStoreFloat3(&m_xmf3Right, XMVector3TransformNormal(XMLoadFloat3(&m_xmf3Right), xmmtxRotate));
		XMStoreFloat3(&m_xmf3Up, XMVector3TransformNormal(XMLoadFloat3(&m_xmf3Up), xmmtxRotate));
		XMStoreFloat3(&m_xmf3Look, XMVector3TransformNormal(XMLoadFloat3(&m_xmf3Look), xmmtxRotate));
	}
	if (y != 0.0f)
	{
		//�÷��̾��� ���� y-���� �������� ȸ���ϴ� ����� �����Ѵ�.
		xmmtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&m_xmf3Up), (float)XMConvertToRadians(y));
		//ī�޶��� ���� x-��, y-��, z-���� ȸ���Ѵ�.
		XMStoreFloat3(&m_xmf3Right, XMVector3TransformNormal(XMLoadFloat3(&m_xmf3Right), xmmtxRotate));
		XMStoreFloat3(&m_xmf3Up, XMVector3TransformNormal(XMLoadFloat3(&m_xmf3Up), xmmtxRotate));
		XMStoreFloat3(&m_xmf3Look, XMVector3TransformNormal(XMLoadFloat3(&m_xmf3Look), xmmtxRotate));
	}
	if (z != 0.0f)
	{
		//�÷��̾��� ���� z-���� �������� ȸ���ϴ� ����� �����Ѵ�.
		xmmtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&m_xmf3Look), (float)XMConvertToRadians(z));
		//ī�޶��� ���� x-��, y-��, z-���� ȸ���Ѵ�.
		XMStoreFloat3(&m_xmf3Right, XMVector3TransformNormal(XMLoadFloat3(&m_xmf3Right), xmmtxRotate));
		XMStoreFloat3(&m_xmf3Up, XMVector3TransformNormal(XMLoadFloat3(&m_xmf3Up), xmmtxRotate));
		XMStoreFloat3(&m_xmf3Look, XMVector3TransformNormal(XMLoadFloat3(&m_xmf3Look), xmmtxRotate));
	}
}

void CFreeCamera::Update(float fTimeElapsed) {


	UpdateShaderState();
}

//������� ����
void CFreeCamera::UpdateShaderState() {
	//���� ����
	XMMATRIX xmMtxProjection = (XMLoadFloat4x4(&m_xmf4x4Projection));
	UpdateViewMtx();

	XMMATRIX xmMtxView = XMLoadFloat4x4(&m_xmf4x4View);

	XMStoreFloat4x4(&m_stCameraBufferData.m_xmf4x4ViewProjection, XMMatrixTranspose(XMMatrixMultiply(xmMtxView, xmMtxProjection)));
	m_stCameraBufferData.m_CameraPos = XMFLOAT4(m_xmf3Pos.x, m_xmf3Pos.y, m_xmf3Pos.z, 1.0f);
	//���� ����

	//������� ������Ʈ
	m_pd3dDeviceContext->UpdateSubresource(m_pViewProjectionBuffer, 0, NULL, &m_stCameraBufferData, 0, 0);

	//--------------------------------deferred lighting--------------------------------
	XMFLOAT4 xmf4PerspectiveValues;
	xmf4PerspectiveValues.x = m_xmf4x4Projection._11;
	xmf4PerspectiveValues.y = m_xmf4x4Projection._22;
	xmf4PerspectiveValues.z = m_xmf4x4Projection._33;
	xmf4PerspectiveValues.w = m_xmf4x4Projection._43;

	XMFLOAT4X4 xmf4x4World;
	XMStoreFloat4x4(&xmf4x4World, XMMatrixTranspose(GetWorldMtx()));

	GBUFFER_UNPACKING_DATA data{ xmf4PerspectiveValues , xmf4x4World };
	m_pd3dDeviceContext->UpdateSubresource(m_pGBufferUnpackingBuffer, 0, NULL, &data, 0, 0);
	//--------------------------------deferred lighting--------------------------------
}

void CFreeCamera::SetShaderState() {
	//������� set
	m_pd3dDeviceContext->VSSetConstantBuffers(VS_CB_CAMERA, 1, &m_pViewProjectionBuffer);
	m_pd3dDeviceContext->DSSetConstantBuffers(DS_CB_CAMERA, 1, &m_pViewProjectionBuffer);
	m_pd3dDeviceContext->GSSetConstantBuffers(GS_CB_CAMERA, 1, &m_pViewProjectionBuffer);
}

void CFreeCamera::ProcessInput(float fTimeElapsed, UCHAR* pKeyBuffer, float cxDelta, float cyDelta) {
	DWORD dwDirection = 0;
	//UINT iMoveState;

	
	if (pKeyBuffer[VK_W] & 0xF0)		dwDirection |= DIR_FORWARD;
	if (pKeyBuffer[VK_S] & 0xF0)		dwDirection |= DIR_BACKWARD;
	if (pKeyBuffer[VK_A] & 0xF0)		dwDirection |= DIR_LEFT;
	if (pKeyBuffer[VK_D] & 0xF0)		dwDirection |= DIR_RIGHT;
	if (pKeyBuffer[VK_E] & 0xF0)		dwDirection |= DIR_UP;
	if (pKeyBuffer[VK_Q] & 0xF0)		dwDirection |= DIR_DOWN;

	//if (pKeyBuffer[VK_SHIFT] & 0xF0)	iMoveState = RUN;
	//else								iMoveState = WALK;

	

#define SPEED 100

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
		if (pKeyBuffer[VK_RBUTTON] & 0xF0)
			Rotate(cyDelta, 0.0f, -cxDelta);
		else
			Rotate(cyDelta, cxDelta, 0.0f);
	}

}

CFreeCamera::CFreeCamera(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext) : CCamera(pd3dDevice, pd3dDeviceContext) {
	
}
CFreeCamera::~CFreeCamera() {

}
