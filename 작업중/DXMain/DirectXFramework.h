#pragma once

#include "Scene.h"
#include "FreeCamera.h"
#include "ThirdPersonCamera.h"
#include "PostProcessingLayer.h"
#include "Timer.h"

#define TITLE_MAX_LENGTH 64
//-------------------------deferred lighting------------------
#define RENDER_TARGET_NUMBER 3
//-------------------------deferred lighting------------------


class CDirectXFramework {

public:
	//���� ��ü �� ������ �ʱ�ȭ.
	void Begin(HINSTANCE hInstance, HWND hWnd);
	void End();

	//-------------------------------begin----------------------------
	bool CreateD3D11Deivce();
	bool CreateRenderTargetView();

	using FLOAT4 = float[4];
	//-------------------------------begin----------------------------
	
public:
	//�׸� �׸���
	void Render();

	//��ü Animate, input����
	void Update(float fTimeElapsed);

	//�帧
	void FrameAdvance();

	//--------------------------------------input------------------------------------
	void ProcessInput(float fTimeElapsed);
	//�������� �޽���(Ű����, ���콺 �Է�)�� ó���ϴ� �Լ��̴�. 
	void OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	void OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	//--------------------------------------input------------------------------------

	//--------------------------------------scene-----------------------------------------
	void ChangeScene(CScene* pScene);
	void PopScene();
	void PushScene(CScene* pScene);
	//--------------------------------------scene-----------------------------------------

	RECT GetDisplaySize() { return m_rcClient; }
	shared_ptr<CCamera> GetCamera() { return m_pCamera; }
	ID3D11Device* GetDevice() { return m_pd3dDevice; }
	ID3D11DeviceContext* GetDeviceContext() { return m_pd3dDeviceContext; }
	ID3D11RenderTargetView* GetRenderTargetView() { return m_pd3dRenderTargetView; }
	ID3D11DepthStencilView* GetDepthStencilView() { return m_pd3dDepthStencilView; }
	ID3D11Texture2D* GetDepthStencilTexture() { return m_pd3dDepthStencilBuffer; }
	HWND GethWnd() { return m_hWnd; }

	//--------------------------------------deferred rendering func-----------------------------
	void SetForwardRenderTargets();
	void SetMainRenderTargetView();
	void SetRenderTargetViews(UINT nRenderTarget, ID3D11RenderTargetView** pd3dRTVs);
	void PostProcessing();

	void ClearDepthStencilView();
	void ClearRenderTargetView();
	void ReleaseForwardRenderTargets();
	//--------------------------------------deferred rendering func-----------------------------

private:
	
	//------------------------------------Scene-----------------------------------
	std::stack<CScene*>	m_stackScene;
	//scene ť
	int m_nScene{ 0 };
	//scene�� ����
	shared_ptr<CCamera> m_pCamera{ nullptr };//ī�޶�� framework�� ������ �ִ°� �´ٰ� ������
	//------------------------------------Scene-----------------------------------

	ID3D11Device			*	m_pd3dDevice{ nullptr };
	IDXGISwapChain			*	m_pdxgiSwapChain{ nullptr };

	ID3D11RenderTargetView	*	m_pd3dRenderTargetView{ nullptr };
	ID3D11DeviceContext		*	m_pd3dDeviceContext{ nullptr };

	HINSTANCE					m_hInstance{ NULL };
	HWND						m_hWnd{ NULL };
	RECT						m_rcClient;

	// Rendering �߰� �κ� : Depth-Stencil �� ���� ������ �������� ť�갡 ��µ��� �ʴ´�.
	ID3D11Texture2D				*m_pd3dDepthStencilBuffer{ nullptr };
	ID3D11DepthStencilView		*m_pd3dDepthStencilView{ nullptr };

	//str 
	_TCHAR									m_pszBuffer[50];

//---------------------------deferred rendering-----------------------
	ID3D11Texture2D			 *m_pd3dtxtColorSpecInt;//0
	ID3D11Texture2D			 *m_pd3dtxtNormal;//1
	ID3D11Texture2D			 *m_pd3dtxtSpecPow;//2


	ID3D11ShaderResourceView *m_pd3dsrvColorSpecInt;//0
	ID3D11ShaderResourceView *m_pd3dsrvNormal;//1
	ID3D11ShaderResourceView *m_pd3dsrvSpecPow;//2

	ID3D11RenderTargetView   *m_pd3drtvColorSpecInt;//0
	ID3D11RenderTargetView   *m_pd3drtvNormal;//1
	ID3D11RenderTargetView   *m_pd3drtvSpecPow;//2

	vector<shared_ptr<CTexture>> m_vObjectLayerResultTexture;
	vector<shared_ptr<CTexture>> m_vLightLayerResultTexture;

	//--------------------------light render target----------------
	ID3D11Texture2D			 *m_pd3dtxtLight;
	ID3D11ShaderResourceView *m_pd3dsrvLight;
	ID3D11RenderTargetView   *m_pd3drtvLight;
	//--------------------------light render target----------------
	//-------------------------layer-------------------------
	CPostProcessingLayer* m_pPostProcessingLayer{ nullptr };
	//-------------------------layer-------------------------
	void ObjectRender();
	void LightRender();

//---------------------------deferred rendering-----------------------
public :	
	CDirectXFramework() = default;
	~CDirectXFramework();


	// 2D code
private:

	//ID2D1Device2					*	m_pd2dDevice		{ nullptr }	;
	//ID2D1Factory3					*	m_pd2dFactory		{ nullptr }	;
	//ID2D1DeviceContext2				*	m_pd2dDeviceContext	{ nullptr }	;
	//IDWriteFactory3					*	m_pdwFactory		{ nullptr }	;
	//IWICImagingFactory2				*	m_wicFactory		{ nullptr }	;
	//
	//// 3D SwapChain���� RenderTarget�� ��� ���� BackBuffer �Դϴ�.
	//ID2D1Bitmap1					*	m_pd2dBmpBackBuffer { nullptr }	;
	//
	//wstring								m_strText			{         }	;
	//ID2D1SolidColorBrush			*	m_pd2dsbrText		{ nullptr }	;
	//IDWriteTextLayout3				*	m_pdwTextLayout		{ nullptr }	;
	//IDWriteTextFormat2				*	m_pdwTextFormat		{ nullptr }	;
	//// ���������� Drawing ���¸� �����մϴ�.
	//ID2D1DrawingStateBlock1			*	m_pd2dStateBlock	{ nullptr }	;
	//
	//void ReleaseD2DResources()
	//{
	//	if (m_pd2dDevice) m_pd2dDevice->Release();
	//	if (m_pd2dFactory) m_pd2dFactory->Release();
	//	if (m_pd2dDeviceContext) m_pd2dDeviceContext->Release();
	//	if (m_pdwFactory) m_pdwFactory->Release();
	//	if (m_wicFactory) m_wicFactory->Release();
	//	if (m_pd2dBmpBackBuffer) m_pd2dBmpBackBuffer->Release();
	//	if (m_pd2dsbrText) m_pd2dsbrText->Release();
	//	if (m_pdwTextLayout) m_pdwTextLayout->Release();
	//	if (m_pdwTextFormat) m_pdwTextFormat->Release();
	//	if (m_pd2dStateBlock) m_pd2dStateBlock->Release();
	//}

public:

	//bool CreateD2D1Device(IDXGIDevice3* pdxgiDevice)
	//{
	//	if(pdxgiDevice) pdxgiDevice->AddRef();
	//	else return (false);
	//
	//	HRESULT hResult = S_OK;
	//
	//	// Direct2D ���ҽ��� �ʱ�ȭ�մϴ�.
	//	D2D1_FACTORY_OPTIONS options;
	//	ZeroMemory(&options, sizeof(D2D1_FACTORY_OPTIONS));
	//
	//#if defined(_DEBUG)
	//	// ������Ʈ�� ����� ���� ���� ��� SDK ���̾ ���� Direct2D ������� ����մϴ�.
	//	options.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
	//#endif
	//
	//	// Direct2D ���͸��� �ʱ�ȭ�մϴ�.
	//	if(FAILED(hResult = D2D1CreateFactory(	  D2D1_FACTORY_TYPE_SINGLE_THREADED
	//											, __uuidof(ID2D1Factory3)
	//											, &options
	//											, reinterpret_cast<LPVOID*>(&m_pd2dFactory)
	//	))) goto ReleaseDXGI;
	//
	//	// DirectWrite ���͸��� �ʱ�ȭ�մϴ�.
	//	if (FAILED(hResult = DWriteCreateFactory(	  DWRITE_FACTORY_TYPE_SHARED
	//												, __uuidof(IDWriteFactory3)
	//												, reinterpret_cast<IUnknown**>(&m_pdwFactory)
	//	))) goto ReleaseDXGI;
	//
	//	// COM Library�� �ʱ�ȭ�մϴ�.
	//	CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
	//
	//	// WIC(Windows Imaging Component) ���͸��� �ʱ�ȭ�մϴ�.
	//	if (FAILED(hResult = CoCreateInstance(	  CLSID_WICImagingFactory
	//											, nullptr
	//											, CLSCTX_INPROC_SERVER
	//											, IID_PPV_ARGS(&m_wicFactory)
	//	))) goto ReleaseDXGI;
	//
	//	m_pd2dFactory->CreateDevice(pdxgiDevice, &m_pd2dDevice);
	//
	//	// Direct2D DC�� �����մϴ�.
	//	m_pd2dDevice->CreateDeviceContext(
	//		  D2D1_DEVICE_CONTEXT_OPTIONS_NONE
	//		, &m_pd2dDeviceContext
	//	);
	//
	//ReleaseDXGI:
	//	pdxgiDevice->Release();
	//	return (!FAILED(hResult));
	//}
	//
	//bool CreateD2DBackBuffer()
	//{
	//	// ȭ���� �ػ󵵸� ����ϴ�.
	//	float fdpiX, fdpiY;
	//	m_pd2dFactory->GetDesktopDpi(&fdpiX, &fdpiY);
	//	
	//	HRESULT hResult = S_OK;
	//
	//	// ���� ü�� �� ���ۿ� ����� Direct2D ���
	//	// ��Ʈ���� ����� �̸� ���� ������� �����մϴ�.
	//	D2D1_BITMAP_PROPERTIES1 bitmapProperties =
	//		BitmapProperties1(	  D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW
	//				, PixelFormat(DXGI_FORMAT_R16G16B16A16_FLOAT, D2D1_ALPHA_MODE_PREMULTIPLIED)
	//				, fdpiX
	//				, fdpiY
	//		);
	//
	//	IDXGISurface2 *dxgiBackBuffer;
	//	if (FAILED(hResult = m_pdxgiSwapChain->GetBuffer(0, IID_PPV_ARGS(&dxgiBackBuffer)))) return false;
	//	if (FAILED(hResult = m_pd2dDeviceContext->CreateBitmapFromDxgiSurface(	  dxgiBackBuffer
	//																			, &bitmapProperties
	//																			, &m_pd2dBmpBackBuffer
	//	))) return false;
	//
	//	m_pd2dDeviceContext->SetTarget(m_pd2dBmpBackBuffer);
	//	m_pd2dDeviceContext->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_GRAYSCALE);
	//
	//	CreateDwriteDevice();
	//
	//	return true;
	//}
	//
	//void CreateDwriteDevice()
	//{
	//
	//	m_pdwFactory->CreateTextFormat(	  L"D2Coding"
	//									, nullptr
	//									, DWRITE_FONT_WEIGHT_LIGHT
	//									, DWRITE_FONT_STYLE_NORMAL
	//									, DWRITE_FONT_STRETCH_NORMAL
	//									, 32.0f
	//									, L"ko-kr"
	//									, reinterpret_cast<IDWriteTextFormat**>(&m_pdwTextFormat)
	//	);
	//	m_pdwTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
	//	m_pd2dFactory->CreateDrawingStateBlock(&m_pd2dStateBlock);
	//	m_pd2dDeviceContext->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &m_pd2dsbrText);
	//}
	//
	//void Update2D()
	//{
	//	m_strText = L"Test Text";
	//
	//	if (m_pdwTextLayout) m_pdwTextLayout->Release();
	//
	//	m_pdwFactory->CreateTextLayout(   m_strText.c_str()
	//									, m_strText.length()
	//									, m_pdwTextFormat
	//									, 240.f
	//									, 50.f
	//									, reinterpret_cast<IDWriteTextLayout**>(&m_pdwTextLayout)
	//	);
	//}
	//
	//void Render2D() 
	//{
	////	m_pd2dDeviceContext->SaveDrawingState(m_pd2dStateBlock);
	//
	//	m_pd2dDeviceContext->BeginDraw();
	//	{
	//		Matrix3x2F screenTranslation { Matrix3x2F::Identity() };
	//		m_pd2dDeviceContext->SetTransform(screenTranslation);
	//
	//		m_pdwTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);
	//		m_pd2dDeviceContext->DrawTextLayout(	  Point2F(0.f, 0.f)
	//												, m_pdwTextLayout
	//												, m_pd2dsbrText
	//		);
	//
	//	}
	//	m_pd2dDeviceContext->EndDraw();
	//
	////	m_pd2dDeviceContext->RestoreDrawingState(m_pd2dStateBlock);
	//}

};
