#include "stdafx.h"

CheatVariable curSession;
std::ofstream Log::LOG("SDO-X.CheatLog");

void RenderMenu()
{
	if (!curSession.CheatValve[48])return;
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	if (ImGui::Begin(u8"SDO - X Test Built", 0, ImGuiWindowFlags_NoCollapse))
	{
		ImGui::Text(u8"現在のフレーム/秒 %.1f", ImGui::GetIO().Framerate);
		ImGui::Text(u8"注意 ：これは私有財産です.");
	}
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}

void ACC_Logging(const char* x)
{
	Log() << x;
}

void CheatAlter(SDOEntry* GameEnginePtr) {
	__try
	{
		GameEnginePtr->WindowHandle->ActiveScreen = 1;
		if (GameEnginePtr->GameEngineIndex->Interface)
		{
			if (GameEnginePtr->GameEngineIndex->Interface->Result)
			{
				if (GameEnginePtr->GameEngineIndex->Interface->Result->ShowtimeGauge < 1120)
					GameEnginePtr->GameEngineIndex->Interface->Result->ShowtimeGauge += 200;
			}
			else
			{
				RaiseException(EXCEPTION_ACCESS_VIOLATION, 0, 0, 0);
			}
			if (GameEnginePtr->GameEngineIndex->Interface->PlayerInteraction)
			{
				if (GameEnginePtr->GameEngineIndex->Interface->PlayerInteraction->NotePanel)
				{
					if (GameEnginePtr->GameEngineIndex->Interface->PlayerInteraction->NotePanel->Autoplay == 0)
						GameEnginePtr->GameEngineIndex->Interface->PlayerInteraction->NotePanel->Autoplay = TRUE;
				}
				else
				{
					RaiseException(EXCEPTION_ACCESS_VIOLATION, 0, 0, 0);
				}
			}
			else
			{
				RaiseException(EXCEPTION_ACCESS_VIOLATION, 0, 0, 0);
			}
		}
		else
		{
			RaiseException(EXCEPTION_ACCESS_VIOLATION, 0, 0, 0);
		}
	}
	__except (GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION ?
		EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH)
	{
		ACC_Logging("Access Violation");
	}
}

HRESULT WINAPI EndScene(LPDIRECT3DDEVICE9 pDevice)
{
	if (!pDevice)
		return ((tEndScene)curSession.oEndScene)(pDevice);

	if (!curSession.g_Device)
	{
		curSession.g_Device = pDevice;
		return ((tEndScene)curSession.oEndScene)(pDevice);
	}

	pDevice->GetCreationParameters(&curSession.D3DCP);

	if (!curSession.CheatValve[50])
	{
		ImGui::CreateContext();
		curSession.CheatValve[49] = TRUE;
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		ImGuiStyle& style = ImGui::GetStyle();
		ImVec4* colors = ImGui::GetStyle().Colors;
		io.DeltaTime = 1.0f / 60.0f;
		io.FontGlobalScale = 0.90f;
		io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\msyh.ttc", 15.0f, NULL, io.Fonts->GetGlyphRangesChineseFull());
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
		io.ConfigWindowsMoveFromTitleBarOnly = TRUE;
		ImGui_ImplWin32_Init(curSession.D3DCP.hFocusWindow);
		ImGui_ImplDX9_Init(pDevice);
		ImGui::StyleColorsDark();
		ImGui::SetColorEditOptions(ImGuiColorEditFlags_Float | ImGuiColorEditFlags_HDR | ImGuiColorEditFlags_PickerHueWheel);
		style.WindowTitleAlign.x = 0.50f; style.WindowTitleAlign.y = 0.50f;
		style.FrameRounding = 12.0f; style.WindowRounding = 12.0f;
		curSession.CheatValve[50] = TRUE;
	}

	RenderMenu();

	if (curSession.CheatValve[49] && GetAsyncKeyState(VK_END) & 1)
	{
		curSession.CheatValve[48] = !curSession.CheatValve[48];
	}

	return ((tEndScene)curSession.oEndScene)(pDevice);
}

LRESULT CALLBACK hWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (curSession.CheatValve[50])
	{
		switch (uMsg)
		{
		case WM_ACTIVATEAPP:
			curSession.CheatValve[49] = (bool)wParam;
			break;
		case WM_SIZE:
			ImGui_ImplDX9_InvalidateDeviceObjects();
			ImGui_ImplDX9_CreateDeviceObjects();
			break;
		}
		ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam);
	}

	return CallWindowProc(curSession.WinHandle, hwnd, uMsg, wParam, lParam);
}

DWORD WINAPI CheatRoutine(LPVOID lArg)
{
	//Initialization Stages
	do { Log() << "Attemped to search for DirectX9"; Sleep(1000); curSession.DirectXModulePE = (DWORD)GetModuleHandle("d3d9.dll"); } while (!curSession.DirectXModulePE);
	Log() << "DirectX9 : " << std::hex << curSession.DirectXModulePE;
	do { Log() << "Attemped to search for GameModule PE"; Sleep(1000); curSession.GameModulePE = (DWORD)GetModuleHandle("sdo.bin"); } while (!curSession.GameModulePE);
	Log() << "GameModule : " << std::hex << curSession.GameModulePE;
	do { Log() << "Attemped to search for Game Engine"; Sleep(1000); curSession.BaseAddress = SignatureScan(curSession.GameModulePE, 0x128000, (PBYTE)SignatureBase, SignatureMask) + 0xB;} while (!curSession.BaseAddress);
	Log() << "EngineAddress : " << std::hex << curSession.BaseAddress;

	//Initialization Completed begin entry for ImGui Interface , COMMENT OUT FOR FUTURE USAGE
	/*curSession.SigScanVTM = SignatureScan(curSession.DirectXModulePE, 0x128000, (PBYTE)"\xC7\x06\x00\x00\x00\x00\x89\x86\x00\x00\x00\x00\x89\x86", "xx????xx????xx");
	memcpy(&curSession.VTable, (void*)(curSession.SigScanVTM + 2), 4); MakeDevice(curSession.dwVTable);
	curSession.oEndScene = (void*)curSession.VTable[42]; HookAddressCALL((PBYTE)dwEndScene, (PBYTE)&EndScene, 6);
	do { Sleep(100); } while (curSession.g_Device == NULL);
	curSession.WinHandle = (WNDPROC)SetWindowLongPtr(curSession.D3DCP.hFocusWindow, GWL_WNDPROC, (LONG)(LONG_PTR)hWndProc);*/

	//ImGui initialized and EndScene hooked , begin loop

	DWORD BaseAddress = *(DWORD*)curSession.BaseAddress;
	while (TRUE)
	{
		CheatAlter((SDOEntry*)BaseAddress);
		Sleep(1);
	}
	return 0;
}
