#pragma once
#include "DXObject.h"
#include "Sampler.h"
#include "ConstantBuffer.h"

class CTexture : public DXObject {
public:
	//texture array
	bool Begin(UINT nTextures, _TCHAR(*ppstrFilePaths)[128], shared_ptr<CSampler> pSampler, UINT Slot = 0, UINT BindFlag = BIND_PS, shared_ptr<CConstantBuffer> pConstantBuffer = nullptr);
	bool Begin(UINT nTextures, ID3D11Texture2D **ppd3dTextures, shared_ptr<CSampler> pSampler, UINT Slot, UINT BindFlag, shared_ptr<CConstantBuffer> pConstantBuffer);
	//texture 1��
	bool Begin(_TCHAR(pstrFilePath)[128], shared_ptr<CSampler> pSampler, UINT Slot = 0, UINT BindFlag = BIND_PS, shared_ptr<CConstantBuffer> pConstantBuffer = nullptr);
	//�ϼ��� srv set
	bool Begin(ID3D11ShaderResourceView* pShaderResourceView, shared_ptr<CSampler> pSampler, UINT Slot = 0, UINT BindFlag = BIND_PS, shared_ptr<CConstantBuffer> pConstantBuffer = nullptr);
	
	virtual bool End();

	virtual void SetShaderState();
	virtual void CleanShaderState();

	virtual void UpdateShaderState();

	//setter
	void SetSampler(shared_ptr<CSampler> pSampler);
	void SetConstantBuffer(shared_ptr<CConstantBuffer> pConstantBuffer);
	//getter
	ID3D11ShaderResourceView* GetShaderResourceView() { return m_pd3dsrvTexture; }
	shared_ptr<CSampler> GetvSampler() { return m_pSampler; }

	ID3D11ShaderResourceView *CreateTexture2DArraySRV(_TCHAR(*ppstrFilePaths)[128], UINT nTextures);
	ID3D11ShaderResourceView *CreateTexture2DArraySRV(ID3D11Texture2D **ppd3dTextures, UINT nTextures);
protected:	
	//texture 1��
	ID3D11ShaderResourceView*		m_pd3dsrvTexture{ nullptr };
	UINT							m_TextureStartSlot{ 0 };
	UINT							m_BindFlag{ 0 };

	//�� �ȿ� �������� dx��ü���� ������
	//smapler 1�� texture�� 1���ϱ�
	shared_ptr<CSampler>		m_pSampler{ nullptr };

	//buffer ������
	shared_ptr<CConstantBuffer>	m_pConstantBuffer{ nullptr };
	
public:
	CTexture(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext);
	virtual ~CTexture();

};