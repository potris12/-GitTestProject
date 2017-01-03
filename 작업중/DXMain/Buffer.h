#pragma once
#include "DXObject.h"


class CBuffer : public DXObject {
public:
	bool Begin() { return true; };
	virtual bool End() { return true; };

	virtual void SetShaderState() {};
	virtual void CleanShaderState() {};

	virtual void UpdateShaderState() {};

	//map
	void* Map();
	void Unmap();
	//unmap

	//getter
	UINT GetBufferStride() { return m_BufferStride; }
	UINT GetnObject() { return m_nObject; }
	UINT GetOffset() { return m_Offset; }
	ID3D11Buffer* GetBuffer() { return m_pd3dBuffer; }

private:

protected:

	//buffer 1��
	ID3D11Buffer*					m_pd3dBuffer{ nullptr };

	//map data 1��
	D3D11_MAPPED_SUBRESOURCE		m_MappedData{ nullptr };
	void*							m_pData{ nullptr };

	//buffe info 1��
	UINT							m_nObject{ 0 };
	UINT							m_BufferStride{ 0 };
	UINT							m_ByteWidth{ 0 };
	UINT							m_Offset{ 0 };
public:
	CBuffer(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext);
	virtual ~CBuffer();

};

/*
//���۰� �־�����
if (m_ppBuffers) {
//���� ���۵� release

//���ο� ���� �迭 ����
new_ppBuffers = new shared_ptr<CBuffer>[m_nBuffers];
//���� �� ����
int i = 0;
for (i; i < m_nBuffers; ++i) {
new_ppBuffers[i] = m_ppBuffers[i];
}
//���� ���� �� ����
int j;
for (int j = 0; j < nBuffers; ++j) {
new_ppBuffers[m_nBuffers + j] = ppBuffers[j];
}
//���� ���� �����
delete[] m_ppBuffers;

//�� ����
m_ppBuffers = new_ppBuffers;
m_nBuffers = new_nBuffers;
}
else {
//�� ����
m_ppBuffers = ppBuffers;
m_nBuffers = nBuffers;
}
*/