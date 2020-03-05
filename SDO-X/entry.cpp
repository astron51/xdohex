#include <Windows.h>
// Entry Point should be standalone

DWORD WINAPI CheatRoutine(LPVOID lArg);

BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
	DisableThreadLibraryCalls(hModule);
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		CreateThread(NULL, NULL, &CheatRoutine, NULL, NULL, NULL);
		break;
	case DLL_PROCESS_DETACH:
		break;
	}
	return true;
}