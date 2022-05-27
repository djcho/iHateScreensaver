// Ihatescreensaver.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <Windows.h>
#include <string>

int CALLBACK WinMain(
	_In_ HINSTANCE hInstance,
	_In_ HINSTANCE hPrevInstance,
	_In_ LPSTR     lpCmdLine,
	_In_ int       nCmdShow)
{

	HANDLE mutexHandle = ::CreateMutex(FALSE, 0, _T("ihateScreenSaver!!"));
	if (GetLastError() == ERROR_ALREADY_EXISTS)
		exit(0);


	HKEY regKey = NULL;
	std::wstring notifyRegKey = L"Control Panel\\Desktop";
	LRESULT result = RegOpenKeyExW(HKEY_CURRENT_USER, notifyRegKey.c_str(), 0, STANDARD_RIGHTS_READ | KEY_QUERY_VALUE | KEY_NOTIFY | KEY_WRITE, &regKey);
	if (result != ERROR_SUCCESS)
		return 0;

	char inputDataBuffer[] = "0";
	result = RegSetValueEx(regKey, L"ScreenSaveTimeOut", 0, REG_SZ, (const BYTE*)&inputDataBuffer, strlen(inputDataBuffer)* sizeof(char));
	if (result != ERROR_SUCCESS)
	{
		RegCloseKey(regKey);
		return 0;
	}

	result = RegSetValueEx(regKey, L"ScreenSaveActive", 0, REG_SZ, (const BYTE*)&inputDataBuffer, strlen(inputDataBuffer) * sizeof(char));
	if (result != ERROR_SUCCESS)
	{
		RegCloseKey(regKey);
		return 0;
	}

	HANDLE eventHandle = CreateEvent(NULL, FALSE, FALSE, NULL);
	while (true)
	{
		DWORD regFilter = REG_NOTIFY_CHANGE_NAME | REG_NOTIFY_CHANGE_ATTRIBUTES | REG_NOTIFY_CHANGE_LAST_SET | REG_NOTIFY_CHANGE_SECURITY;
		result = RegNotifyChangeKeyValue(regKey, true, regFilter, eventHandle, true);

		if (result != ERROR_SUCCESS)
		{
			CloseHandle(eventHandle);
			wprintf(L"RegNotifyChangeKeyValue fail\n");
			return 0;
		}

		// Wait for an event to occur.
		switch (WaitForSingleObject(eventHandle, INFINITE)) // 변경이 감지될 때까지 대기
		{
		case WAIT_TIMEOUT:
			break;
		case WAIT_FAILED:
			wprintf(L"WaitForSingleObject fail. errorcode: %d\n", GetLastError());
			break;
		case WAIT_OBJECT_0:
			wprintf(L"Registry changed.\n");
			result = RegSetValueEx(regKey, L"ScreenSaveTimeOut", 0, REG_SZ, (const BYTE*)&inputDataBuffer, strlen(inputDataBuffer) * sizeof(char));
			if (result != ERROR_SUCCESS)
			{
				RegCloseKey(regKey);
				return 0;
			}
			result = RegSetValueEx(regKey, L"ScreenSaveActive", 0, REG_SZ, (const BYTE*)&inputDataBuffer, strlen(inputDataBuffer) * sizeof(char));
			if (result != ERROR_SUCCESS)
			{
				RegCloseKey(regKey);
				return 0;
			}
			break;
		default:
			break;
		}
	}
	CloseHandle(eventHandle);
}
