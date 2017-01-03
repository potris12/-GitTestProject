#pragma once
//#include "Camera.h"
#include "SingleTon.h"
/*
rotaion ���� TwAddVarRW(bar, "Rotation", TW_TYPE_QUAT4F, &g_SpongeRotation, "opened=true axisz=-z group=Sponge");
TwAddVarRW(bar, "Camera distance", TW_TYPE_FLOAT, &g_CamDistance, "min=0 max=4 step=0.01 keyincr=PGUP keydecr=PGDOWN");0~4 ���� 0.01ũ��� �����ϴ� bar 
TwAddVarRW(bar, "Background", TW_TYPE_COLOR4F, &g_BackgroundColor, "colormode=hls");//color�����ֱ� �̷��� �ϸ� �ش� color�����͸� ������ ���� ���ϴ� �޴��� �ڵ����� ��Ÿ��
TwAddVarRW(bar, "Animation", TW_TYPE_BOOLCPP, &g_Animate, "group=Sponge key=a"); botton o/x bool type
*/
class CTwBarManager : public CSingleTonBase<CTwBarManager> {
public:
	bool Begin(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dDeviceContext, const char* def);
	bool End();

	bool OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	void Render();

	void AddBar(const char* barName);
	void SetParam(const char* barName, const char* paramName, TwParamValueType type, UINT inValueCnt, const void* inValue);
	//������ ���� ������� �̰ɷ� ���� ������ struct
	// get�Լ����� TwŸ�Կ� �°� ����->ui�� ó��->Set�Լ����� �� struct�� ����
	//client data�� �����̵� �� ���ְ�, Callback�Լ��� value�� ui�� �ٲٴ� ���̴�.
	void AddVarCB(const char* barName, const char* menuName, TwType type, TwSetVarCallback setCallback, TwGetVarCallback getCallback, void* clientData, const char* def);
	//�ڷ����� ������ �ִ°�� �̰ɷ�
	void AddVarRW(const char* barName, const char* menuName, TwType type, void* var, const char* def);

/*
	����� ���� Ʋ�ε� �̰� Tw�ڷ����� ���� �༮�̾���� ���ʹϾ��̳�. color��
*/
	//light dir float3�� ����ϱ� ���� ȭ��ǥ bar
	void AddDirBar(const char* barName, const char* groupName, const char* menuName, void* var);
	//rotation object float4�� ����ϱ� ���� ���׶�� bar
	void AddRotationBar(const char* barName, const char* groupName, const char* menuName, void* var);
	//color�� ����ϱ� ������ ���� ���� bar ���� ���� menu�� �ڵ����� ����
	void AddColorBar(const char* barName, const char* groupName, const char* menuName, void* var);
	//bool botton o/x�� ����ϱ� ������ ��ư ui
	void AddBoolBar(const char* barName, const char* groupName, const char* menuName, void* var);

	//light dir float3�� ����ϱ� ���� ȭ��ǥ bar
	void AddDirBar(const char* barName, const char* groupName, const char* menuName, CGameObject* pObj);
	//rotation object float4�� ����ϱ� ���� ���׶�� bar
	void AddRotationBar(const char* barName, const char* groupName, const char* menuName, CGameObject* pObj);


	void DeleteBar(const char* barName);
private:
	ID3D11Device* m_pd3dDevice;
	ID3D11DeviceContext* m_pd3dDeviceContext;

	map<string, TwBar*> m_mTwBar;
	using pairTwBar = pair<string, TwBar*>;

public:
	CTwBarManager() : CSingleTonBase<CTwBarManager>("twbarmanagersingleton") {}

};

//callback
void TW_CALL SetQuaternionToTwBar(const void *value, void * clientData);
void TW_CALL GetQuaternionToTwBar(void *value, void * clientData);
