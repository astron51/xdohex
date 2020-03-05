//Custom Class Do not Temper

class CheatVariable
{
public :
	D3DDEVICE_CREATION_PARAMETERS D3DCP;
	DWORD BaseAddress;
	DWORD DirectXModulePE;
	DWORD GameModulePE;
	DWORD SigScanVTM;
	DWORD* VTable;
	DWORD dwVTable[5];
	LPDIRECT3DDEVICE9 g_Device;
	WNDPROC WinHandle;
	BOOL CheatValve[60];
	INT CheatValue[60];
	FLOAT CheatFloat[60];
	void* oEndScene;
};

//End of Custom Class

// Generated using ReClass 2016

//Result = +11 / B
class SDOEntry;
class XHWID;
class UEngine;
class UInterface;
class UInteraction;
class UNotePanel;
class UResult;
class UEntity;
class UPlayerList;

class SDOEntry
{
public:
	UEngine* GameEngineIndex; //0x0000 
	char pad_0x0004[0x5C]; //0x0004
	XHWID* WindowHandle; //0x0060 

}; //Size=0x0064

class XHWID
{
public:
	char pad_0x0000[0x8]; //0x0000
	__int32 ActiveScreen; //0x0008 

}; //Size=0x000C

class UEngine
{
public:
	char pad_0x0000[0x8]; //0x0000
	UInterface* Interface; //0x0008 

}; //Size=0x000C

class UInterface
{
public:
	char pad_0x0000[0x100]; //0x0000
	UInteraction* PlayerInteraction; //0x0100 
	char pad_0x0104[0x4]; //0x0104
	UResult* Result; //0x0108 
	UResult* ResultX; //0x010C 

}; //Size=0x0110

class UInteraction
{
public:
	UNotePanel* NotePanel; //0x0000 

}; //Size=0x0004

class UNotePanel
{
public:
	char pad_0x0000[0x30]; //0x0000
	float NoteMasterSpeed; //0x0030 
	char pad_0x0034[0x3C]; //0x0034
	float NoteSpeedFloat; //0x0070 
	char pad_0x0074[0x4]; //0x0074
	__int32 NoteSpeedInteger; //0x0078 
	__int32 NoteSpeedLongInteger; //0x007C 
	char pad_0x0080[0x34]; //0x0080
	__int32 NotePosition; //0x00B4 
	char pad_0x00B8[0x8]; //0x00B8
	__int32 Left; //0x00C0 
	__int32 Down; //0x00C4 
	__int32 Up; //0x00C8 
	__int32 Right; //0x00CC 
	char pad_0x00D0[0xC]; //0x00D0
	__int32 AllMissGoesHere; //0x00DC 
	char MusicData[18]; //0x645888 
	void* Unknown; //0x00F2 
	char pad_0x00F8[0x218]; //0x00F8
	__int32 Autoplay; //0x0310 
	char pad_0x0314[0x10]; //0x0314
	__int32 NoteLeftPosition; //0x0324 
	char pad_0x0328[0x4]; //0x0328
	__int32 NoteDownPosition; //0x032C 
	char pad_0x0330[0x4]; //0x0330
	__int32 NoteUpPosition; //0x0334 
	char pad_0x0338[0x4]; //0x0338
	__int32 NoteRightPosition; //0x033C 
	char pad_0x0340[0x3D4]; //0x0340

}; //Size=0x0714

class UResult
{
public:
	char pad_0x0000[0x14]; //0x0000
	__int32 ShowtimeGauge; //0x0014 
	char pad_0x0018[0x20]; //0x0018
	__int32 TotalPerfect; //0x0038 
	__int32 TotalCool; //0x003C 
	__int32 TotalBad; //0x0040 
	__int32 TotalMiss; //0x0044 
	__int32 CurrentCombo; //0x0048 
	__int32 HighestCombo; //0x004C 
	char pad_0x0050[0x4]; //0x0050
	__int32 Score; //0x0054 

}; //Size=0x0058

class UEntity
{
public:
	char pad_0x0000[0x2FC]; //0x0000
	UPlayerList* PlayerList; //0x02FC 

}; //Size=0x0300

class UPlayerList
{
public:
	char pad_0x0000[0x70]; //0x0000

}; //Size=0x0070

