#include "stdafx.h"
#include "InstanceBuffer.h"

bool CInstanceBuffer::Begin(UINT Object, UINT BufferStride, UINT Offset) {
	
		m_nObject = Object;
		m_BufferStride = BufferStride;
		m_ByteWidth = m_nObject * m_BufferStride;
		m_Offset = Offset;
		//create buffer
		m_pd3dBuffer = CreateInstanceBuffer(m_ByteWidth);
	
	return true;
}

bool CInstanceBuffer::End() {

	return true;
}

//instance buffer�� set/ clean �׷��� ���ص� ��
void CInstanceBuffer::SetShaderState() {

}
void CInstanceBuffer::CleanShaderState() {

}

void CInstanceBuffer::UpdateShaderState() {

}


//�ʱ� ���� ���� instanceBuffer����
ID3D11Buffer* CInstanceBuffer::CreateInstanceBuffer(UINT nByteWidth) {

	ID3D11Buffer *pd3dInstanceBuffer = NULL;
	D3D11_BUFFER_DESC d3dBufferDesc;
	ZeroMemory(&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));
	/*������ �ʱ�ȭ �����Ͱ� ������ ���� ���۷� �����Ѵ�. ��, ���߿� ������ �Ͽ� ������ ä��ų� �����Ѵ�.*/
	d3dBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	d3dBufferDesc.ByteWidth = nByteWidth;
	d3dBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	d3dBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_pd3dDevice->CreateBuffer(&d3dBufferDesc, NULL, &pd3dInstanceBuffer);
	return(pd3dInstanceBuffer);

}


CInstanceBuffer::CInstanceBuffer(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext) : CBuffer(pd3dDevice, pd3dDeviceContext) { }
CInstanceBuffer::~CInstanceBuffer() { };

