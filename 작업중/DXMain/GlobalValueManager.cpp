
#include "stdafx.h"
#include "GlobalValueManager.h"

bool CGlobalValueManager::Begin(HINSTANCE hInst, HWND hWnd) {
	SethInst(hInst);
	SethWnd(hWnd);
	GetClientRect(m_hWnd, &m_rcClient);

	//create swapchain, device, device context
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

	}


	return true;
}

bool CGlobalValueManager::End() {
	// Release ��ü
	if (m_pd3dDevice) m_pd3dDevice->Release();
	if (m_pdxgiSwapChain) m_pdxgiSwapChain->Release();
	if (m_pd3dDeviceContext) m_pd3dDeviceContext->Release();

	return true;
}

CGlobalValueManager::CGlobalValueManager() :CSingleTonBase<CGlobalValueManager>("globalvaluemanagersingleton") {

}

CGlobalValueManager::~CGlobalValueManager() {

}
