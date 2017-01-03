#include "stdafx.h"
#include "DebugTexture.h"

bool CDebugTexture::Begin()
{
	//m_objectID = OBJECT_DEBUG_TEXTURE;
	//CGameObject::Begin();
	return true;
}

void CDebugTexture::SetBufferInfo(void ** ppMappedResources, int & nInstance, shared_ptr<CCamera> pCamera) {
	//����ȯ
	VS_VB_DEBUG_TEXTURE_INSTANCE* pnInstances = (VS_VB_DEBUG_TEXTURE_INSTANCE*)ppMappedResources[0];

	//���� ����
	pnInstances[nInstance].m_xmf2LeftTop = m_xmf2LeftTop;
	pnInstances[nInstance].m_xmf2RightBottom = m_xmf2RightBottom;
}

void CDebugTexture::SetTextureInfo(XMFLOAT2 lefttop, XMFLOAT2 rightbottom) {
	/*
	sp.x/w, sp.y/h
	ep.x/w, ep.y/w 
	�ϸ�  0�� 1���� ���� ���� ���̰�
	�̰��� ��
	*2 -1 �ϸ� -1���� 1���� �� �� ȭ�� ��ǥ��� ��ȯ�� ��.
	
	���⼭ y���� �츮�� �����ϴ� �ǹ̰� �ݴ�ϱ� 
	*-1 ���ָ� 0��0 1�� -1 -1�� 1�� �� �ݴ밡 �ȴ�.
	*/
	lefttop.x = lefttop.x / m_rcClient.right;
	lefttop.y = lefttop.y / m_rcClient.bottom;
	rightbottom.x = rightbottom.x / m_rcClient.right;
	rightbottom.y = rightbottom.y / m_rcClient.bottom;

	lefttop.x = lefttop.x * 2 - 1;
	lefttop.y = lefttop.y * 2 - 1;
	rightbottom.x = rightbottom.x * 2 - 1;
	rightbottom.y = rightbottom.y * 2 - 1;

	lefttop.y *= -1;
	rightbottom.y *= -1;

	m_xmf2LeftTop = lefttop;
	m_xmf2RightBottom = rightbottom;
}

CDebugTexture::CDebugTexture() :CGameObject("debugtexture") {

}

CDebugTexture::~CDebugTexture(){

}
