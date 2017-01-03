
#include "stdafx.h"
#include "InputManager.h"

bool CInputManager::Begin(HWND hWnd) {
	m_hWnd = hWnd;
//	m_stHashTable.clear();
//
//	//Pad Connect
//	m_bConnect = false;
//
//	//Vibe State
//	m_fVibrateionFrame = 0.0f;
//	m_fVibrateDuringTime = GPAD_DURING_VIVERATE;
//	m_bVibrate = false;
//
//
//	m_bKeyState = new bool[YK_END];
//
	memset(&m_pKeyBuffers, 0, sizeof(m_pKeyBuffers));
	memset(&m_bDownCheck, 0, sizeof(m_bDownCheck));

	return true;
}

bool CInputManager::End() {
	return true;
}

void CInputManager::Update(float fTimeEleasped){
	//IsGamepadConnected(&m_bConnect);
	m_cxDelta = 0.0f, m_cyDelta = 0.0f;

	//if (m_bConnect)
	//	UpdateGamePoad(fTimeEleasped);
	//else
		UpdateKeyBoard();
	if(m_bCapture)
		UpdateMouse();
}
void CInputManager::UpdateKeyBoard(){
	GetKeyboardState(m_pKeyBuffers);
}

void CInputManager::UpdateMouse(){
	POINT ptCursorPos;

	//���콺�� ĸ�������� ���콺�� �󸶸�ŭ �̵��Ͽ��� ���� ����Ѵ�.
	if (GetCapture() == m_hWnd)
	{
		//���콺 Ŀ���� ȭ�鿡�� ���ش�(������ �ʰ� �Ѵ�).
		SetCursor(NULL);
		//���� ���콺 Ŀ���� ��ġ�� �����´�.
		GetCursorPos(&ptCursorPos);
		//���콺 ��ư�� ���� ä�� ���� ��ġ���� ���� ���콺 Ŀ���� ��ġ���� ������ ���� ���Ѵ�.
		m_cxDelta = (float)(ptCursorPos.x - m_ptOldCursorPos.x) / 3.0f;
		m_cyDelta = (float)(ptCursorPos.y - m_ptOldCursorPos.y) / 3.0f;
		SetCursorPos(m_ptOldCursorPos.x, m_ptOldCursorPos.y);
	}
}

//Key Up
bool CInputManager::KeyUp(eVK input){
	//if (IsGamepadConnected())
	//	return KeyPadUp(input);
	return KeyBoardUp(input);
}
bool CInputManager::KeyBoardUp(eVK input)
{
	if (m_pKeyBuffers[input] & 0xF0){
		m_bUpCheck[input] = true;
		return false;
	}
	else if (m_bUpCheck[input]){
		m_bUpCheck[input] = false;
		return true;
	}
	return false;
}
//Only KeyDown
bool CInputManager::OnlyKeyDown(eVK input)
{
	//if (IsGamepadConnected())
	//	return OnlyKeyPadDown(input);
	return OnlyKeyBoardDown(input);
}
bool CInputManager::OnlyKeyBoardDown(eVK input)
{
	if (m_pKeyBuffers[input] & 0xF0){
		return true;
	}
	return false;
}
BYTE CInputManager::AnyOneKeyDown(int n, eVK ...)
{
	if (false == m_bKeyBoardPressCheck) return false;

	va_list _ArgList;
	va_start(_ArgList, n);
	bool press = false;
	while (n--)
	{
		auto input = va_arg(_ArgList, eVK);
		press = OnlyKeyDown(input);
		if (press) return press;
	}
	return press;
}

//KeyDown
bool CInputManager::KeyDown(eVK input)
{
	//if (IsGamepadConnected())
	//	return KeyPadDown(input);
	return KeyBoardDown(input);
}
bool CInputManager::KeyBoardDown(eVK input){
	if (m_pKeyBuffers[input] & 0xF0){
		if (false == m_bDownCheck[input]){
			m_bDownCheck[input] = true;
			return true;
		}
	}
	else {
		m_bDownCheck[input] = false;
	}
	return false;
}

//mouse
void CInputManager::SetWheel(int wheel)
{
	if (wheel > WHEEL_NON)   m_sWheel = WHEEL_UP;
	else if (wheel < WHEEL_NON) m_sWheel = WHEEL_DOWN;
	else                  m_sWheel = WHEEL_NON;
}

void CInputManager::MouseWheel()
{
	if (GetAsyncKeyState(VK_MBUTTON) & 0x8000)
		std::cout << "0x800" << std::endl;

	if (GetAsyncKeyState(VK_MBUTTON) & 0xF0)
		std::cout << "0xF0" << std::endl;

	if (GetAsyncKeyState(VK_MBUTTON) & 0x00)
		std::cout << "0x00" << std::endl;

	if (GetAsyncKeyState(VK_MBUTTON) & 0x01)
		std::cout << "0x01" << std::endl;

	if (GetAsyncKeyState(VK_MBUTTON))
		std::cout << "VK_MBUTTON" << std::endl;
}

void CInputManager::SetMousePoint(int x, int y){
	pt.x = x; pt.y = y;
}

bool CInputManager::MouseLeftDown(){
	return m_bLeftCheck;
}

bool CInputManager::MouseRightDown(){
	return m_bRightCheck;
}

//^ ������ 1, �ٸ��� 0
bool CInputManager::MouseLeftUp(){
	return (!m_bLeftCheck);
}

bool CInputManager::MouseRightUp(){
	return (!m_bRightCheck);
}

void CInputManager::SetbCapture(bool b){
	if (b) SetCapture(m_hWnd);
	else ReleaseCapture();

	m_bCapture = b;
}

void CInputManager::SetMousePoint(POINT _pt){
	pt = _pt;
}

CInputManager::CInputManager() :CSingleTonBase<CInputManager>("inputmanagersingleton") {

}

CInputManager::~CInputManager() {

}
