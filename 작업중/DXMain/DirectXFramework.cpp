#include "stdafx.h"
#include "DirectXFramework.h"

static bool testBotton{ false };
static bool testBotton2{ false };
static XMFLOAT4 xmf4Xolor;

void CDirectXFramework::Begin(HINSTANCE hInstance, HWND hWnd)
{

	m_hInstance = hInstance;
	m_hWnd = hWnd;
	GetClientRect(m_hWnd, &m_rcClient);

	_tcscpy_s(m_pszBuffer, _T("DXMAIN ("));
	
	if (false == CreateD3D11Deivce()) return;

	//device ���� ���� �ؾ���
	//singleton Init 
	RESOURCEMGR->Begin(m_pd3dDevice, m_pd3dDeviceContext);
	RCSELLER->Begin(m_pd3dDevice, m_pd3dDeviceContext);
	DEBUGER->Begin(m_pd3dDevice, m_pd3dDeviceContext);
	INPUTMGR->Begin(m_hWnd);
	TWBARMGR->Begin(m_pd3dDevice, m_pd3dDeviceContext, 
		" GLOBAL help='test ui death' ");
	//singleton Init
	//post processing layer
	m_pPostProcessingLayer = new CPostProcessingLayer(m_pd3dDevice, m_pd3dDeviceContext);
	m_pPostProcessingLayer->Begin();
	//post processing layer
	//device ���� ���� �ؾ���

	// render target�� depth-stencil buffer ����/ deferred texture ����
	if (!CreateRenderTargetView()) {
		MessageBox(m_hWnd, TEXT("RenderTarget�̳� Depth-Stencil ���� ������ �����߽��ϴ�. ���α׷��� �����մϴ�."), TEXT("���α׷� ���� ����"), MB_OK);
		return;
	}

	//camera
	m_pCamera = make_shared<CFreeCamera>(m_pd3dDevice, m_pd3dDeviceContext);
	//------------------------------------------ī�޶� ����--------------------------------------
	m_pCamera->Begin();
	m_pCamera->GenerateProjectionMatrix(
		// FOV Y �� : Ŭ ���� �ָ����� �� �� �ִ�.
		60.0f * XM_PI / 180.0f
		// ��Ⱦ��
		, float(m_rcClient.right) / float(m_rcClient.bottom)
		// �ּ� �Ÿ�
		, 0.01f
		// �ִ� �Ÿ�
		, 10000.0f);

	//��ġ ����
	//viewprojection��� ����
	XMVECTOR eye = { 0.0f, 200.0f, 00.0f, 0.0f };
	XMVECTOR at = { 500.0f, 0.0f, 500.0f, 0.0f };
	XMVECTOR up = { 0.0f, 1.0f, 0.0f, 0.0f };

	m_pCamera->SetLookAt(eye, at, up);
	// RS�� ����Ʈ ����
	m_pCamera->SetViewport(0, 0, m_rcClient.right, m_rcClient.bottom, 0.0f, 1.0f);
	//------------------------------------------ī�޶� ����--------------------------------------


//	//ui
	int barSize[2] = { 224, 320 };
	TWBARMGR->SetParam("TweakBarTest", "size", TW_PARAM_INT32, 2, barSize);

	TWBARMGR->AddBoolBar("TweakBarTest", "Button", "TestBool", &testBotton);
	TWBARMGR->AddBoolBar("TweakBarTest", "Botton", "TestBool2", &testBotton2);
	TWBARMGR->AddColorBar("TweakBarTest", "Botton", "TestColor", &xmf4Xolor);
	//TwDefine(" GLOBAL help='This example shows how to integrate AntTweakBar into a DirectX11 application.' "); // Message added to the help bar.
	//int barSize[2] = { 224, 320 };
	//TwSetParam(bar, NULL, "size", TW_PARAM_INT32, 2, barSize);
	
}

void CDirectXFramework::End() {
	// Release ��ü
	if (m_pd3dDevice) m_pd3dDevice->Release();
	if (m_pdxgiSwapChain) m_pdxgiSwapChain->Release();
	if (m_pd3dRenderTargetView) m_pd3dRenderTargetView->Release();
	if (m_pd3dDeviceContext) m_pd3dDeviceContext->Release();

	// Rendering �߰� �κ� : Depth-Stencil �� ���� ������ �������� ť�갡 ��µ��� �ʴ´�.
	if (m_pd3dDepthStencilBuffer) m_pd3dDepthStencilBuffer->Release();
	if (m_pd3dDepthStencilView) m_pd3dDepthStencilView->Release();

	//camera end
	if (m_pCamera){
		m_pCamera->End();
	}

	//scene end
	for (int i = 0; i < m_nScene; ++i) {
		m_stackScene.top()->End();
		m_stackScene.pop();
	}
	
	ReleaseForwardRenderTargets();

	//layer
	if (m_pPostProcessingLayer) {
		m_pPostProcessingLayer->End();
		delete m_pPostProcessingLayer;
	}

	//singleton End
	RESOURCEMGR->End();
	RCSELLER->End();
	DEBUGER->End();
	INPUTMGR->End();
	//singleton End

//	//ui
	TWBARMGR->End();
}

bool CDirectXFramework::CreateD3D11Deivce()
{
	// Swap Chain Description ����ü
	DXGI_SWAP_CHAIN_DESC dxgiSwapChainDesc;
#pragma region [DXGI_SWAP_CHAIN_DESC �ʱ�ȭ]
	// ����ü ����
	::ZeroMemory(&dxgiSwapChainDesc, sizeof(dxgiSwapChainDesc));
	// BufferCount : �ĸ� ������ ���� ����
	dxgiSwapChainDesc.BufferCount = 2;

	// BufferDesc : �ĸ� ������ ���÷��� ������ ����
	{
		// Width : ������ ���� ũ�⸦ �ȼ��� ����
		dxgiSwapChainDesc.BufferDesc.Width = m_rcClient.right;
		// Height : ������ ���� ũ�⸦ �ȼ��� ����
		dxgiSwapChainDesc.BufferDesc.Height = m_rcClient.bottom;
		// Format : �ĸ� ���� �ȼ� ����
		/// DirectX 11-1(Chap 01)-Device, p.49 ����
		dxgiSwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
		// RefreshRate : ȭ�� ���� ������ Hz ������ ����
		{
			// Denominator : �и�
			dxgiSwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
			// Numerator : ����
			dxgiSwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
		}
		// ScanlineOrdering : scan line �׸��� ��� ����(�⺻ 0)
		//	DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED        (0) : ��ĵ ���� ������ �������� ����	
		//	DXGI_MODE_SCANLINE_ORDER_PROGRESSIVE        (1) : ���α׷��ú� ��ĵ ���� ���� ����
		//	DXGI_MODE_SCANLINE_ORDER_UPPER_FIELD_FIRST  (2) : ���� �ʵ�� �̹��� ����
		//	DXGI_MODE_SCANLINE_ORDER_LOWER_FIELD_FIRST  (3) : ���� �ʵ�� �̹��� ����
		dxgiSwapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	}

	// BufferUsage : �ĸ� ���ۿ� ���� ǥ�� ��� ��İ� CPU�� ���� ��� ����
	//	DXGI_USAGE_SHADER_INPUT				: shader �� �Է����� ���
	//	DXGI_USAGE_RENDER_TARGET_OUTPUT		: render target���� ���
	//	DXGI_USAGE_BACK_BUFFER         		: back buffer �� ���
	//	DXGI_USAGE_SHARED              		: ���� ����
	//	DXGI_USAGE_READ_ONLY           		: �б� ����
	//	DXGI_USAGE_DISCARD_ON_PRESENT  		: DXGI ���� ���� ���(����ڰ� ������� ����)
	//	DXGI_USAGE_UNORDERED_ACCESS    		: ������ȭ ����
	dxgiSwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	// OutputWindow : ��� �������� �ڵ��� ����(�ݵ�� ��ȿ�ؾ� ��)
	dxgiSwapChainDesc.OutputWindow = m_hWnd;

	// SampleDesc : ���� ���ø��� ǰ���� ����
	// CheckMultisampleQualityLevels �Լ��� ����Ͽ� ���� ���ø� ���� ���θ� Ȯ���� �ڿ� �� ����
	{
		// Count : �ȼ� �� ���� ����
		//	1  : ���� ���ø��� ���� ����
		//	2~ : �ش� ����ŭ�� ���� ���ø�
		dxgiSwapChainDesc.SampleDesc.Count = 1;
		// Quality : ǰ�� ����
		// 0 : ���� ���ø��� ���� ����
		dxgiSwapChainDesc.SampleDesc.Quality = 0;
	}

	// Windowed : ������ ��� �Ǵ� ��ü ȭ�� ��� ���� ~ TRUE  | â ��� 
	//												   ~ FALSE | ��ü ȭ��
	dxgiSwapChainDesc.Windowed = TRUE;

	// Flags : Swap Chain ���ۿ� ���� ���� ������ ����
	//	DXGI_SWAP_CHAIN_FLAG_NONPREROTATED		(1) : ��ü ȭ�� ��忡�� ���� ������ ������ ȭ������ �ű� �� �ڵ� ȸ������ ����
	//	DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH	(2) : ���� ���α׷��� ���÷��� ��带 ������ �� ����
	//	DXGI_SWAP_CHAIN_FLAG_GDI_COMPATIBLE		(4) : ���� ���α׷��� GDI �� ������ �� �� ����. �ĸ� ���ۿ� GetDC() ��� ����
	dxgiSwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH/*2*/;

	// SwapEffect : Swaping�� ó���ϴ� ���û����� ����(�⺻ : 0)
	//	DXGI_SWAP_EFFECT_DISCARD		(0) : ���� ������ ���
	//	DXGI_SWAP_EFFECT_SEQUENTIAL		(1) : ���� ����
	// DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL	(2) : Flip ���� ����
	dxgiSwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

#pragma endregion


	// �����ϴ� �ϵ���� �׷��� ����̹��� �����մϴ�.
	D3D_DRIVER_TYPE d3dDriverTypes[]
	{
		// �ϵ���� ����̹�, Direct3D�� ����� �ִ��� �ϵ����� ���� 
		// �ϵ���������� �������� �ʴ� ����� ����Ʈ��� ���� ����
		D3D_DRIVER_TYPE_HARDWARE
		// DirectX SDK���� �����ϴ� ������ ����Ʈ���� ����̹�
		// Ư�� ���� 9_1���� Ư�� ���� 10.1���� ����
		// - ������ ����Ʈ���� ����̹�(�����Ͷ�����)�� �ʿ����
		// - ����Ʈ���� ����̹��̸鼭 �׷��� �ϵ������ �ִ� ������ ����
		//   Direct3D 11�� �������� �ʴ� �׷��� �ϵ������� ���
		, D3D_DRIVER_TYPE_WARP
		// ���� ����̹�(��� Direct3D Ư���� �����ϴ� ����Ʈ���� ����̹�)
		// DirectX SDK���� �ӵ����ٴ� ��Ȯ���� ���� ����
		// CPU�� ����Ͽ� �����Ͷ����� ���� 
		// Direct3D 11�� ��� Ư�� �������� �� ����̹��� ����� �� ����
		// ���� ���α׷��� �׽�Ʈ�� ������� ���� �ַ� ���
		, D3D_DRIVER_TYPE_REFERENCE
	};

	// �� �迭�� �� ���� ���α׷����� �����ϴ� DirectX �ϵ���� ��� ���� ������ �����մϴ�.
	// ������ �����ؾ� �մϴ�.
	// ������ ���� ���α׷��� �ʿ��� �ּ� ��� ������ �����ؾ� �մϴ�.
	// ������ �������� ���� ��� ��� ���� ���α׷��� 9.1�� �����ϴ� ������ ���ֵ˴ϴ�.
	D3D_FEATURE_LEVEL d3dFeatureLevels[]
	{
		D3D_FEATURE_LEVEL_11_1
		, D3D_FEATURE_LEVEL_11_0
		, D3D_FEATURE_LEVEL_10_1
		, D3D_FEATURE_LEVEL_10_0
	};
	// ���� ������ ��� �ϵ���� ��� ���� ���� ���� ���մϴ�.
	auto szFeatureLevel = static_cast<UINT>(GetArraySize(d3dFeatureLevels));

	// ���������� ���� ��ġ�� ��� ������ �����ϱ� ���� �����Դϴ�.
	D3D_FEATURE_LEVEL nd3dFeatureLevel = D3D_FEATURE_LEVEL_11_1;

	// Result Handle �Դϴ�. ��ġ�� ���������� ���������� �˻��մϴ�.
	HRESULT hResult = S_OK;

	// �� �÷��״� API �⺻���� �ٸ� �� ä�� ������ ǥ�鿡 ���� ������
	// �߰��մϴ�. Direct2D���� ȣȯ���� ���� �ʿ��մϴ�.
	UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#if defined(_DEBUG)

	auto IsSDKLayerAvailable = []() -> bool {
		return SUCCEEDED(D3D11CreateDevice(nullptr
			, D3D_DRIVER_TYPE_NULL			// ���� �ϵ���� ��ġ�� ���� �ʿ䰡 �����ϴ�.
			, 0
			, D3D11_CREATE_DEVICE_DEBUG		// SDK ���̾ Ȯ���ϼ���.
			, nullptr						// ��� ��� ������ ����˴ϴ�.
			, 0
			, D3D11_SDK_VERSION				// Windows ����� ���� ��� �׻� �� ���� D3D11_SDK_VERSION���� �����մϴ�.
			, nullptr						// D3D ��ġ ������ ������ �ʿ䰡 �����ϴ�.
			, nullptr						// ��� ������ �� �ʿ䰡 �����ϴ�.
			, nullptr						// D3D ��ġ ���ؽ�Ʈ ������ ������ �ʿ䰡 �����ϴ�.
			));
	};

	// SDK ���̾� ������ Ȯ���ϼ���.
	if (IsSDKLayerAvailable())
	{
		// ������Ʈ�� ����� ���� ���� ��쿡�� �� �÷��װ� �ִ� SDK ���̾ ���� ������� ����Ͻʽÿ�.
		creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
	}
#endif

	for (D3D_DRIVER_TYPE &p : d3dDriverTypes)
	{
		if (SUCCEEDED(hResult = D3D11CreateDevice(NULL						// �⺻ ����͸� ����Ϸ��� nullptr�� �����մϴ�.
			, p							// �ϵ���� �׷��� ����̹��� ����Ͽ� ��ġ�� ����ϴ�.
			, 0							// ����̹��� D3D_DRIVER_TYPE_SOFTWARE�� �ƴ� ��� 0�̾�� �մϴ�.
			, creationFlags				// ����� �� Direct2D ȣȯ�� �÷��׸� �����մϴ�.
			, d3dFeatureLevels			// �� ���� ���α׷��� ������ �� �ִ� ��� ���� ����Դϴ�.
			, szFeatureLevel			// �� ����� ũ���Դϴ�.
			, D3D11_SDK_VERSION			// Windows ����� ���� ��� �׻� �� ���� D3D11_SDK_VERSION���� �����մϴ�.
			, &m_pd3dDevice				// ������� Direct3D ��ġ�� ��ȯ�մϴ�.
			, &nd3dFeatureLevel			// ������� ��ġ�� ��� ������ ��ȯ�մϴ�.
			, &m_pd3dDeviceContext		// ��ġ ���� ���ؽ�Ʈ�� ��ȯ�մϴ�.
			)
			)
			)
			break;
	}

	// ��� ���� ������ ����̹����� ������ �����ϸ� ���α׷��� �����մϴ�.
	if (!m_pd3dDevice)
	{
		MessageBox(m_hWnd, TEXT("���� ������ �׷��� ����� �����ϴ�."), TEXT("���α׷� ���� ����"), MB_OK);
		return(false);
	}

	// DXGI Device �� �޽��ϴ�.
	//IDXGIDevice3 *pdxgiDevice = NULL;
	//if (FAILED(hResult = m_pd3dDevice->QueryInterface(__uuidof(IDXGIDevice3), (LPVOID*)&pdxgiDevice)))
	IDXGIDevice *pdxgiDevice = NULL;
	if (FAILED(hResult = m_pd3dDevice->QueryInterface(__uuidof(IDXGIDevice), (LPVOID*)&pdxgiDevice)))
	{
		MessageBox(m_hWnd, TEXT("DXGI Device ��ü�� ��ȯ���� ���߽��ϴ�. ���α׷��� �����մϴ�."), TEXT("���α׷� ���� ����"), MB_OK);
		return(false);
	}
	// DXGI Factory �ν��Ͻ��� DXGIFactory���� �޽��ϴ�.
	//IDXGIFactory *pdxgiFactory = NULL;
	IDXGIFactory *pdxgiFactory = NULL;

	UINT udxgiFlag = 0;
#ifdef _DEBUG
	udxgiFlag |= DXGI_CREATE_FACTORY_DEBUG;
#endif
	//if (FAILED(hResult = CreateDXGIFactory3(udxgiFlag, __uuidof(IDXGIFactory3), (LPVOID*)&pdxgiFactory)))
	if (FAILED(hResult = CreateDXGIFactory(/*udxgiFlag,*/ __uuidof(IDXGIFactory), (LPVOID*)&pdxgiFactory)))
	{
		MessageBox(m_hWnd, TEXT("DXGIFactory������ ��ü ������ �����߽��ϴ�. ���α׷��� �����մϴ�."), TEXT("���α׷� ���� ����"), MB_OK);
		return(false);
	}
	//  SwapChain �� �����մϴ�
	if (FAILED(hResult = pdxgiFactory->CreateSwapChain(pdxgiDevice, &dxgiSwapChainDesc, &m_pdxgiSwapChain)))
	{
		MessageBox(m_hWnd, TEXT("SwapChain �ν��Ͻ� ������ �����߽��ϴ�. ���α׷��� �����մϴ�."), TEXT("���α׷� ���� ����"), MB_OK);
		return(false);
	}

	//	// Direct2D : Direct2D �ν��Ͻ��� �����մϴ�.
	//	if (!CreateD2D1Device(pdxgiDevice))
	//	{
	//		MessageBox(m_hWnd, TEXT("Direct2D �ν��Ͻ� ������ �����߽��ϴ�. ���α׷��� �����մϴ�."), TEXT("���α׷� ���� ����"), MB_OK);
	//		return(false);
	//	}

	// �Ҵ���� COM ��ü�� ��ȯ�մϴ�.
	if (pdxgiDevice) pdxgiDevice->Release();
	if (pdxgiFactory) pdxgiFactory->Release();
	return true;
}

bool CDirectXFramework::CreateRenderTargetView(){

	// Result Handle �Դϴ�. ��ġ�� ���������� ���������� �˻��մϴ�.
	HRESULT hResult = S_OK;

	// render target �� �����ϱ� ���� back buffer �� SwapChain ���� ��û�մϴ�.
	ID3D11Texture2D *pd3dBackBuffer{ nullptr };

	if (FAILED(hResult = m_pdxgiSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID *)&pd3dBackBuffer))) return(false);

	// ��ȯ���� ���۸� ����Ͽ� render target view �� �����մϴ�.
	if (FAILED(hResult = m_pd3dDevice->CreateRenderTargetView(pd3dBackBuffer, NULL, &m_pd3dRenderTargetView))) return(false);

	// back buffer �� ��ȯ�մϴ�.
	if (pd3dBackBuffer) pd3dBackBuffer->Release();


	// Rendering �߰� �κ� : Depth-Stencil �� ���� ������ �������� ť�갡 ��µ��� �ʴ´�.
	{
		// depth stencil "texture" �� �����մϴ�.
		D3D11_TEXTURE2D_DESC d3dDepthStencilBufferDesc;

		// �޸𸮴� 0���� �ʱ�ȭ�մϴ�.
		ZeroMemory(&d3dDepthStencilBufferDesc, sizeof(D3D11_TEXTURE2D_DESC));



		// Width : texture �� �ʺ��Դϴ�.
		d3dDepthStencilBufferDesc.Width = m_rcClient.right;
		// Height : texture �� �����Դϴ�.
		d3dDepthStencilBufferDesc.Height = m_rcClient.bottom;
		if (m_rcClient.right == 0 & m_rcClient.bottom == 0) {
			// Width : texture �� �ʺ��Դϴ�.
			d3dDepthStencilBufferDesc.Width += 1;
			// Height : texture �� �����Դϴ�.
			d3dDepthStencilBufferDesc.Height += 1;
		}
		// MipLevels : texture �ִ� MipMap Level ��. 
		//				���� ���ø� �ؽ�ó : 1
		//				�ִ� �Ӹ� ���� : 0
		d3dDepthStencilBufferDesc.MipLevels = 1;
		// ArraySize :texture �迭�� texture ����. (�迭�� �ƴϸ� 1)
		d3dDepthStencilBufferDesc.ArraySize = 1;
		// Format : texture �ȼ� ����
		d3dDepthStencilBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

		// SampleDesc : ���� ���ø��� ǰ���� ����
		// CheckMultisampleQualityLevels �Լ��� ����Ͽ� ���� ���ø� ���� ���θ� Ȯ���� �ڿ� �� ����
		{

			// Count : �ȼ� �� ���� ����
			//	1  : ���� ���ø��� ���� ����
			//	2~ : �ش� ����ŭ�� ���� ���ø�
			d3dDepthStencilBufferDesc.SampleDesc.Count = 1;
			// Quality : ǰ�� ����
			// 0 : ���� ���ø��� ���� ����
			d3dDepthStencilBufferDesc.SampleDesc.Quality = 0;
		}

		// Usage : texture �� �а� ���� ����� ���� ����
		d3dDepthStencilBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		// BindFlags : ���������� �ܰ� ��� ������ ������ ����
		d3dDepthStencilBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		// MiscFlags : ���ҽ��� ���� �߰� ���� ����. ������� ������ 0.
		d3dDepthStencilBufferDesc.MiscFlags = 0;
		// CPUAccessFlags : CPU�� ���۸� ����� �� �ִ� ����. ������� ������ 0.
		d3dDepthStencilBufferDesc.CPUAccessFlags = 0;

		//	�⺻ ����-���ٽ� ������ ���� ���۸� �������� �Ѵ�.
		if (FAILED(hResult = m_pd3dDevice->CreateTexture2D(&d3dDepthStencilBufferDesc, NULL, &m_pd3dDepthStencilBuffer))) return(false);

		// Create the depth stencil view 
		D3D11_DEPTH_STENCIL_VIEW_DESC d3dDepthStencilViewDesc;
		ZeroMemory(&d3dDepthStencilViewDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
		d3dDepthStencilViewDesc.Format = d3dDepthStencilBufferDesc.Format;

		d3dDepthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		d3dDepthStencilViewDesc.Texture2D.MipSlice = 0;

		if (FAILED(hResult = m_pd3dDevice->CreateDepthStencilView(m_pd3dDepthStencilBuffer, &d3dDepthStencilViewDesc, &m_pd3dDepthStencilView))) return(false);


	}
	{//create framework texture
		ReleaseForwardRenderTargets();
		//----------------------------------------Resource Desc-----------------------------------------//
		D3D11_SHADER_RESOURCE_VIEW_DESC d3dSRVDesc;
		::ZeroMemory(&d3dSRVDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
		d3dSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		d3dSRVDesc.Texture2D.MipLevels = 1;
		//d3dSRVDesc.Format = DXGI_FORMAT_R32_FLOAT;
		//----------------------------------------Resource Desc-----------------------------------------//
		//----------------------------------------TextUre Desc-----------------------------------------//
		D3D11_TEXTURE2D_DESC d3dTexture2DDesc;
		::ZeroMemory(&d3dTexture2DDesc, sizeof(D3D11_TEXTURE2D_DESC));
		d3dTexture2DDesc.Width = m_rcClient.right;
		d3dTexture2DDesc.Height = m_rcClient.bottom;
		d3dTexture2DDesc.MipLevels = 1;
		d3dTexture2DDesc.ArraySize = 1;
		d3dTexture2DDesc.SampleDesc.Count = 1;
		d3dTexture2DDesc.SampleDesc.Quality = 0;
		d3dTexture2DDesc.Usage = D3D11_USAGE_DEFAULT;
		d3dTexture2DDesc.CPUAccessFlags = 0;
		d3dTexture2DDesc.MiscFlags = 0;
		d3dTexture2DDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		//d3dTexture2DDesc.Format = DXGI_FORMAT_R32_TYPELESS;
		//----------------------------------------TextUre Desc-----------------------------------------//
		//----------------------------------------Render Desc-----------------------------------------//
		D3D11_RENDER_TARGET_VIEW_DESC d3dRTVDesc;
		d3dRTVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		d3dRTVDesc.Texture2D.MipSlice = 0;
		//----------------------------------------TextUre Desc-----------------------------------------//
		d3dTexture2DDesc.Format = d3dSRVDesc.Format = d3dRTVDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;

		//--------------------------------------Scene0 RTV Create-----------------------------------------//
		m_pd3dDevice->CreateTexture2D(&d3dTexture2DDesc, nullptr, &m_pd3dtxtColorSpecInt);
		m_pd3dDevice->CreateShaderResourceView(m_pd3dtxtColorSpecInt, &d3dSRVDesc, &m_pd3dsrvColorSpecInt);
		m_pd3dDevice->CreateRenderTargetView(m_pd3dtxtColorSpecInt, &d3dRTVDesc, &m_pd3drtvColorSpecInt);
		//--------------------------------------Scene0 RTV Create-----------------------------------------//

		//--------------------------------------Scene1 RTV Create-----------------------------------------//
		m_pd3dDevice->CreateTexture2D(&d3dTexture2DDesc, nullptr, &m_pd3dtxtNormal);
		m_pd3dDevice->CreateRenderTargetView(m_pd3dtxtNormal, &d3dRTVDesc, &m_pd3drtvNormal);
		m_pd3dDevice->CreateShaderResourceView(m_pd3dtxtNormal, &d3dSRVDesc, &m_pd3dsrvNormal);
		//--------------------------------------Scene1 RTV Create-----------------------------------------//

		//--------------------------------------Scene2 RTV Create-----------------------------------------//
		m_pd3dDevice->CreateTexture2D(&d3dTexture2DDesc, nullptr, &m_pd3dtxtSpecPow);
		m_pd3dDevice->CreateRenderTargetView(m_pd3dtxtSpecPow, &d3dRTVDesc, &m_pd3drtvSpecPow);
		m_pd3dDevice->CreateShaderResourceView(m_pd3dtxtSpecPow, &d3dSRVDesc, &m_pd3dsrvSpecPow);
		//--------------------------------------Scene2 RTV Create-----------------------------------------//

		//�ڱ� texture set -> sampler set����

		//---------------------make texture---------------------
		//texture set to light rendercontainer
		ID3D11ShaderResourceView *pd3dSRV = { m_pd3dsrvColorSpecInt };
		UINT Slot = { 0 };
		UINT BindFlag = { BIND_PS };
		//make sampler
		shared_ptr<CSampler> pSampler = make_shared<CSampler>(m_pd3dDevice, m_pd3dDeviceContext);
		pSampler->Begin();
		shared_ptr<CTexture> pTexture = make_shared<CTexture>(m_pd3dDevice, m_pd3dDeviceContext);
		pTexture->Begin(pd3dSRV, pSampler, Slot, BindFlag);
		m_vObjectLayerResultTexture.push_back(pTexture);

		pd3dSRV = { m_pd3dsrvNormal };
		Slot = { 1 };
		BindFlag = { BIND_PS };
		pTexture = make_shared<CTexture>(m_pd3dDevice, m_pd3dDeviceContext);
		pTexture->Begin(pd3dSRV, pSampler, Slot, BindFlag);
		m_vObjectLayerResultTexture.push_back(pTexture);

		pd3dSRV = { m_pd3dsrvSpecPow };
		Slot = { 2 };
		BindFlag = { BIND_PS };
		pTexture = make_shared<CTexture>(m_pd3dDevice, m_pd3dDeviceContext);
		pTexture->Begin(pd3dSRV, pSampler, Slot, BindFlag);
		m_vObjectLayerResultTexture.push_back(pTexture);
		//---------------------make texture---------------------


		//light texture����
		m_pd3dDevice->CreateTexture2D(&d3dTexture2DDesc, nullptr, &m_pd3dtxtLight);
		m_pd3dDevice->CreateRenderTargetView(m_pd3dtxtLight, &d3dRTVDesc, &m_pd3drtvLight);
		m_pd3dDevice->CreateShaderResourceView(m_pd3dtxtLight, &d3dSRVDesc, &m_pd3dsrvLight);

		pTexture = make_shared<CTexture>(m_pd3dDevice, m_pd3dDeviceContext);
		//make texture
		UINT LightTexSlot = { 0 };
		UINT LightTexBindFlag = { BIND_PS | BIND_CS };
		//make sampler
		shared_ptr<CSampler> pLightTexSampler = make_shared<CSampler>(m_pd3dDevice, m_pd3dDeviceContext);
		UINT LightTexSamplerBindFlag = { BIND_PS | BIND_CS };
		UINT LightTexSamplerSlot = { 0 };
		pLightTexSampler->Begin(LightTexSamplerSlot, LightTexSamplerBindFlag);
		pTexture->Begin(m_pd3dsrvLight, pLightTexSampler, LightTexSlot, LightTexBindFlag);
		m_vLightLayerResultTexture.push_back(pTexture);
		//light texture����

		//first pass data set
		m_pPostProcessingLayer->SetFirstPassData(m_rcClient.right, m_rcClient.bottom);
		m_pPostProcessingLayer->SetBloomThreshold(2.0f);
		float fMiddleGrey = 0.0025f;
		float fWhite = 1.5f;
		float fBloomScale = 0.1f;
		m_pPostProcessingLayer->SetFinalPassData(fMiddleGrey, fWhite, fBloomScale);
		//post processing layer
	}

	//	DXGI_FRAME_STATISTICS p;
	//	m_pdxgiSwapChain->GetFrameStatistics(&p);


	// Direct2D : RenderTarget���� 2DBackBuffer�� ȹ���մϴ�.
	//return(CreateD2DBackBuffer());

	return true;
}

void CDirectXFramework::FrameAdvance()
{
	TIMEMGR->Tick();

	Update(TIMEMGR->GetTimeElapsed());
	//Update2D();

	Render();
	//Render2D();
	
	m_pdxgiSwapChain->Present(0, 0);

	TIMEMGR->GetFrameRate(m_pszBuffer + 8, 37);
	::SetWindowText(m_hWnd, m_pszBuffer);
}


void CDirectXFramework::Render(){
	//scene
	ObjectRender();
	LightRender();
	//scene
	
	PostProcessing();
	//-----------------------------���� �� ����--------------------------------------
	

	if (INPUTMGR->GetDebugMode()) {
	//if(testBotton){
		DEBUGER->AddTexture(XMFLOAT2(100, 100), XMFLOAT2(250, 250), m_pd3dsrvColorSpecInt);
		DEBUGER->AddTexture(XMFLOAT2(100, 250), XMFLOAT2(250, 400), m_pd3dsrvNormal);
		DEBUGER->AddTexture(XMFLOAT2(100, 400), XMFLOAT2(250, 550), m_pd3dsrvLight);
		//DEBUGER->AddTexture(XMFLOAT2(250, 100), XMFLOAT2(500, 350), m_pd3dsrvSpecPow);
		//�̰� �� ���⼭ �������.

		DEBUGER->RenderTexture();
		DEBUGER->RenderText();
		//DEBUGER->ClearDebuger();
	}
	else {
		DEBUGER->ClearDebuger();
	}
	
//	//ui
	TWBARMGR->Render();
}

void CDirectXFramework::Update(float fTimeElapsed){
	//postprocessinglayer ������ set
	m_pPostProcessingLayer->SetAdaptation(fTimeElapsed);

	m_pCamera->Update(fTimeElapsed);
	ProcessInput(fTimeElapsed);

	//-----------------------------���� �� ����--------------------------------------
	m_stackScene.top()->Animate(fTimeElapsed);
	//-----------------------------���� �� ����--------------------------------------

	INPUTMGR->SetWheel(WHEEL_NON);

	if(INPUTMGR->KeyDown(VK_0) ){
		TWBARMGR->DeleteBar("TweakBarTest2");
	}
	
}

void CDirectXFramework::ProcessInput(float fTimeElapsed) {
	INPUTMGR->Update(fTimeElapsed);

	//-----------------------------���� �� ����--------------------------------------
	m_stackScene.top()->ProcessInput(fTimeElapsed);
	//-----------------------------���� �� ����--------------------------------------
	
}
void CDirectXFramework::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	m_stackScene.top()->OnProcessingMouseMessage(hWnd, nMessageID, wParam, lParam);
	
	switch (nMessageID)
	{

	case WM_LBUTTONDOWN:
		INPUTMGR->SetbCapture(true);
		INPUTMGR->SetOldCursorPos();
		INPUTMGR->SetMouseLeft(true);
		break;

	case WM_RBUTTONDOWN:
		INPUTMGR->SetbCapture(true);
		INPUTMGR->SetOldCursorPos();
		INPUTMGR->SetMouseRight(true);
		break;

	case WM_LBUTTONUP:
	{
		//static bool showCusor = true;
		//showCusor = showCusor ? false : true;
		//ShowCursor(showCusor);
		INPUTMGR->SetMouseLeft(false);
		INPUTMGR->SetbCapture(false);
		break;
	}

	case WM_RBUTTONUP:
		INPUTMGR->SetMouseRight(false);
		INPUTMGR->SetbCapture(false);
		break;
	
	case WM_MOUSEMOVE:
		INPUTMGR->SetMousePoint((int)LOWORD(lParam), (int)HIWORD(lParam));
		break;
	case WM_MOUSEWHEEL:
		((short)HIWORD(wParam) < WHEEL_NON) ?
			INPUTMGR->SetWheel(WHEEL_DOWN) :
			INPUTMGR->SetWheel(WHEEL_UP);
		break;

	default:
		break;
	}
}


void CDirectXFramework::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam){
	m_stackScene.top()->OnProcessingKeyboardMessage(hWnd, nMessageID, wParam, lParam);

	switch (nMessageID)
	{
	case WM_KEYUP:
		switch (wParam)
		{
		case VK_ESCAPE:
			::PostQuitMessage(0);
			break;
		}
	}
}

LRESULT CALLBACK CDirectXFramework::OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
//	//ui
	if (TWBARMGR->OnProcessingWindowMessage(hWnd, nMessageID, wParam, lParam)) return 0;

	switch (nMessageID)
	{
		/*�������� ũ�Ⱑ ����� ��(����� ��Alt+Enter�� ��ü ȭ�� ���� ������ ���� ��ȯ�� ��) ���� ü���� �ĸ���� ũ�⸦ �����ϰ� �ĸ���ۿ� ���� ���� Ÿ�� �並 �ٽ� �����Ѵ�. */
	case WM_SIZE:
	{

		m_rcClient.right = LOWORD(lParam);
		m_rcClient.bottom = HIWORD(lParam);

		DEBUGER->ResizeDisplay(m_rcClient);

		m_pd3dDeviceContext->OMSetRenderTargets(0, NULL, NULL);

		if (m_pd3dRenderTargetView) m_pd3dRenderTargetView->Release();
		if (m_pd3dDepthStencilBuffer) m_pd3dDepthStencilBuffer->Release();
		if (m_pd3dDepthStencilView) m_pd3dDepthStencilView->Release();

		if(FAILED(m_pdxgiSwapChain->ResizeBuffers(1, 0, 0, DXGI_FORMAT_R16G16B16A16_FLOAT, 0)))
			MessageBox(nullptr, TEXT(""), TEXT(""), MB_OK);

		//resize rtv size, deferred texture size
		CreateRenderTargetView();

		m_pCamera->SetViewport(0, 0, m_rcClient.right, m_rcClient.bottom, 0.0f, 1.0f);
		break;
	}

	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
	case WM_MOUSEMOVE:
		OnProcessingMouseMessage(hWnd, nMessageID, wParam, lParam);
		break;
	case WM_KEYDOWN:
	case WM_KEYUP:
		OnProcessingKeyboardMessage(hWnd, nMessageID, wParam, lParam);
		break;
	}
	return(0);
}

//--------------------------------------scene-----------------------------------------
void CDirectXFramework::ChangeScene(CScene* pScene) {
	//���� �ƿ� �ٲ� ������ ���� pop�ϰ� 
	if (m_nScene != 0) {
		//������ Scene�� �����ϸ�

		PopScene();
		//������ scene�� ������

		PushScene(pScene);
		//���ο� scene�� ���� ����
	}
	else {
		//�ƴϸ� ó�� �ִ� Scene�̸� �׳�

		PushScene(pScene);
		//���ο� scene�� ���� ����
	}
}
void CDirectXFramework::PopScene() {
	m_stackScene.top()->End();
	delete m_stackScene.top();

	m_stackScene.pop();
	//scene�� ������

	if (m_stackScene.top())	m_stackScene.top()->Begin();
	//top�� ������ begin
}
void CDirectXFramework::PushScene(CScene* pScene) {
	pScene->Begin();
	m_stackScene.push(pScene);
	//���ο� scene�� ���� ����
	++m_nScene;
}
//--------------------------------------scene-----------------------------------------
void CDirectXFramework::SetForwardRenderTargets() {
	ID3D11RenderTargetView *pd3dRTVs[RENDER_TARGET_NUMBER] = { m_pd3drtvColorSpecInt, m_pd3drtvNormal, m_pd3drtvSpecPow };
	float fClearColor[4] = { xmf4Xolor.x, xmf4Xolor.y, xmf4Xolor.z, xmf4Xolor.w };
	if (m_pd3drtvColorSpecInt) m_pd3dDeviceContext->ClearRenderTargetView(m_pd3drtvColorSpecInt, fClearColor);
	if (m_pd3drtvNormal) m_pd3dDeviceContext->ClearRenderTargetView(m_pd3drtvNormal, fClearColor);
	if (m_pd3drtvSpecPow) m_pd3dDeviceContext->ClearRenderTargetView(m_pd3drtvSpecPow, fClearColor);

	SetRenderTargetViews(RENDER_TARGET_NUMBER, pd3dRTVs);

	//multi thread render �ڵ�
	/*
	m_pFrameWork->SetMainRenderTargetView();

	multi thread rendering
	for (auto &pRenderThreadInfo : m_vRenderingThreadInfo)
	{
	pRenderThreadInfo.m_pd3dDeferredContext->OMSetRenderTargets(MULITE_RENDER_NUMBER, (ID3D11RenderTargetView **)&pd3dRTVs, m_pd3dDepthStencilView);
	::SetEvent(pRenderThreadInfo.m_hRenderingBeginEvent);
	}
	*/
}


//--------------------------------------deferred rendering func-----------------------------
void CDirectXFramework::SetMainRenderTargetView() {
	// OM�� RenderTarget �缳��
	m_pd3dDeviceContext->OMSetRenderTargets(1, &m_pd3dRenderTargetView, m_pd3dDepthStencilView);
}
void CDirectXFramework::SetRenderTargetViews(UINT nRenderTarget, ID3D11RenderTargetView** pd3dRTVs) {
	m_pd3dDeviceContext->OMSetRenderTargets(nRenderTarget, pd3dRTVs, m_pd3dDepthStencilView);
}
void CDirectXFramework::ObjectRender(){
	//set
	//-----------------------------ī�޶� ���� set------------------------------------
	m_pCamera->SetShaderState();
	//-----------------------------ī�޶� ���� set------------------------------------
	//-----------------------------���� �� ����--------------------------------------
	ClearDepthStencilView();
	SetForwardRenderTargets();

	m_stackScene.top()->ObjectRender();

}
void CDirectXFramework::LightRender() {

	//light render
	ClearDepthStencilView();
	//set light rtv
	SetRenderTargetViews(1, &m_pd3drtvLight);
	//m_pFrameWork->SetMainRenderTargetView();
	for (auto texture : m_vObjectLayerResultTexture) {
		texture->SetShaderState();
	}
	m_stackScene.top()->LightRender();
	for (auto texture : m_vObjectLayerResultTexture) {
		texture->CleanShaderState();
	}
	//light render
}
void CDirectXFramework::PostProcessing() {
	//postprocessing
	//clear depthStencilview�� ���� ����
	ClearDepthStencilView();

	//��¥ rtv set!
	//m_pFrameWork->ClearRenderTargetView();
	SetMainRenderTargetView();

	for (auto texture : m_vLightLayerResultTexture) {
		texture->SetShaderState();
	}
	
	//rtv�� Ǯ��ũ�� ��ο� 
	m_pPostProcessingLayer->Render(m_pCamera);

	for (auto texture : m_vLightLayerResultTexture) {
		texture->CleanShaderState();
	}
}
void CDirectXFramework::ClearDepthStencilView() {
	// Rendering �߰� �κ� : Depth-Stencil �� ���� ������ �������� ť�갡 ��µ��� �ʴ´�.
	m_pd3dDeviceContext->ClearDepthStencilView(m_pd3dDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}
void CDirectXFramework::ClearRenderTargetView() {
	m_pd3dDeviceContext->ClearRenderTargetView(m_pd3dRenderTargetView, FLOAT4{ 0.0f, 0.5f, 0.8f, 1.0f });
}

void CDirectXFramework::ReleaseForwardRenderTargets() {
	//texture end
	m_vObjectLayerResultTexture.clear();
	m_vLightLayerResultTexture.clear();

	if (m_pd3dtxtColorSpecInt) m_pd3dtxtColorSpecInt->Release();//0
	m_pd3dtxtColorSpecInt = nullptr;

	if (m_pd3dtxtNormal) m_pd3dtxtNormal->Release();//1
	m_pd3dtxtNormal = nullptr;

	if (m_pd3dtxtSpecPow) m_pd3dtxtSpecPow->Release();//2
	m_pd3dtxtSpecPow = nullptr;

	if (m_pd3dsrvColorSpecInt) m_pd3dsrvColorSpecInt->Release();//0
	m_pd3dsrvColorSpecInt = nullptr;

	if (m_pd3dsrvNormal) m_pd3dsrvNormal->Release();//1
	m_pd3dsrvNormal = nullptr;

	if (m_pd3dsrvSpecPow) m_pd3dsrvSpecPow->Release();//2
	m_pd3dsrvSpecPow = nullptr;

	if (m_pd3drtvColorSpecInt) m_pd3drtvColorSpecInt->Release();//0
	m_pd3drtvColorSpecInt = nullptr;

	if (m_pd3drtvNormal) m_pd3drtvNormal->Release();//1
	m_pd3drtvNormal = nullptr;

	if (m_pd3drtvSpecPow) m_pd3drtvSpecPow->Release();//2
	m_pd3drtvSpecPow = nullptr;

	if (m_pd3drtvLight) m_pd3drtvLight->Release();
	m_pd3drtvLight = nullptr;

}
//--------------------------------------deferred rendering func-----------------------------

CDirectXFramework::~CDirectXFramework(){

}
