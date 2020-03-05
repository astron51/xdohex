#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <ostream>
#include <sstream>
#include <fstream>
#include "GameStructure.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx9.h"
#include "ImGui/imgui_impl_win32.h"
using namespace std;

#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")

#define GameVersion 0x0
#define dwEndScene 0x00435696
#define SignatureBase "\x00\x00\x00\x00\x00\x00\x00\x00"
#define SignatureMask "xxxx??xxxxx????x????xxx"
#define Offset 0xB

BOOL CompareTwoByte(const BYTE* pData, const BYTE* bMask, const char* szMask)
{
	for (; *szMask; ++szMask, ++pData, ++bMask)
		if (*szMask == 'x' && *pData != *bMask)
			return false;
	return (*szMask) == NULL;
}
DWORD SignatureScan(DWORD dwAddress, DWORD dwLen, BYTE* bMask, const char* szMask)
{
	for (DWORD i = 0; i < dwLen; i++)
		if (CompareTwoByte((BYTE*)(dwAddress + i), bMask, szMask))
			return (DWORD)(dwAddress + i);
	return 0;
}
VOID MakeDevice(DWORD* dwVTable)
{
	LPDIRECT3D9 pD3d9;
	LPDIRECT3DDEVICE9 pD3DDevice;
	pD3d9 = Direct3DCreate9(D3D_SDK_VERSION);
	if (pD3d9 == NULL)
		return;
	D3DPRESENT_PARAMETERS pPresentParms;
	ZeroMemory(&pPresentParms, sizeof(pPresentParms));
	pPresentParms.Windowed = TRUE;
	pPresentParms.BackBufferFormat = D3DFMT_UNKNOWN;
	pPresentParms.SwapEffect = D3DSWAPEFFECT_DISCARD;
	if (FAILED(pD3d9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, GetDesktopWindow(), D3DCREATE_SOFTWARE_VERTEXPROCESSING, &pPresentParms, &pD3DDevice)))
		return;
	DWORD* dwTable = (DWORD*)pD3DDevice;
	dwTable = (DWORD*)dwTable[0];
	dwVTable[0] = dwTable[16];  //Reset
	dwVTable[1] = dwTable[17];  //Present
	dwVTable[2] = dwTable[42];  //EndScene
	dwVTable[3] = dwTable[82];  //DrawIndexedPrimitive
	dwVTable[4] = dwTable[100]; //SetStreamSource
	dwVTable[5] = dwTable[118]; //CreateQuery
}
VOID* DetourMake(BYTE* src, const BYTE* dst, const int len)
{
	int Pointer[] = { 0xE9, 0x90 };

	BYTE* jmp = (BYTE*)malloc(len + 5);
	DWORD dwback;
	VirtualProtect(src, len, PAGE_READWRITE, &dwback);
	memcpy(jmp, src, len);
	jmp += len;
	jmp[0] = Pointer[0];
	*(DWORD*)(jmp + 1) = (DWORD)(src + len - jmp) - 5;
	src[0] = Pointer[0];
	*(DWORD*)(src + 1) = (DWORD)(dst - src) - 5;
	for (INT i = 5; i < len; i++) src[i] = Pointer[1];
	VirtualProtect(src, len, dwback, &dwback);

	return(jmp - len);
}
void CodecaveInsertion(DWORD codecave, SIZE_T size, DWORD address)
{
	DWORD PreviousProtection;
	VirtualProtect((LPVOID)address, size, PAGE_EXECUTE_READWRITE, &PreviousProtection);
	*(BYTE*)(address) = 0xE9;
	*(DWORD*)(address + 1) = (codecave - address) - 5;
	if (size > 5)
		memset((void*)(address + 5), 0x90, size - 5);
	VirtualProtect((LPVOID)address, size, PreviousProtection, &PreviousProtection);
}
void* HookAddressCALL(const PBYTE src, const PBYTE dst, const int len)
{
	BYTE buf[5];
	DWORD protect;
	int offset = (int)dst - ((int)src + 5);
	buf[0] = (char)0xE8;
	*(DWORD*)(buf + 1) = offset;
	VirtualProtect(src, len, PAGE_EXECUTE_READWRITE, &protect);
	memcpy(src, buf, 5);
	for (int i = 5; i < len; i++)
		src[i] = 0x90;
	VirtualProtect(src, len, protect, NULL);

	return (void*)(src + len);
}
void* HookAddress(const PBYTE src, const PBYTE dst, const int len)//by default len shld be 5 bytes
{
	BYTE* jmp = (BYTE*)malloc(len + 5);
	DWORD dwBack;

	VirtualProtect(src, len, PAGE_EXECUTE_READWRITE, &dwBack);
	memcpy(jmp, src, len);
	jmp += len;
	jmp[0] = 0xE9;
	*(DWORD*)(jmp + 1) = (DWORD)(src + len - jmp) - 5;
	src[0] = 0xE9;
	*(DWORD*)(src + 1) = (DWORD)(dst - src) - 5;
	for (int i = 5; i < len; i++)
		src[i] = 0x90;
	VirtualProtect(src, len, dwBack, &dwBack);
	return (jmp - len);

}
void ModifyMemory(LPVOID source, SIZE_T len, LPVOID destination)
{
	DWORD previous;
	VirtualProtect(destination, len, PAGE_EXECUTE_READWRITE, &previous);
	memmove(destination, source, len);
	VirtualProtect(destination, len, previous, &previous);
}
class Log
{
public:
	Log() {}
	~Log()
	{
		if (LOG.is_open())
		{
			LOG << std::endl;
		}
	}

	template <typename T>
	Log& operator<<(const T& t)
	{
		if (LOG.is_open())
		{
			LOG << t;
		}
		return *this;
	}
private:
	static std::ofstream LOG;
};
typedef HRESULT(WINAPI* tEndScene)(LPDIRECT3DDEVICE9 pDevice);
IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
