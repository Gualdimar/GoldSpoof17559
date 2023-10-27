#include <xtl.h>
#include "ppcasm.h"

// struct for the kernel version
typedef struct _XBOX_KRNL_VERSION {
	WORD Major;
	WORD Minor;
	WORD Build;
	WORD Qfe;
} XBOX_KRNL_VERSION, *PXBOX_KRNL_VERSION;

// kernel imports that aren't normally exposed
EXTERN_C { 
	VOID DbgPrint(const char* s, ...);
	extern PXBOX_KRNL_VERSION XboxKrnlVersion;
}

/*VOID *ResolveFunction(CHAR* ModuleName, DWORD Ordinal)
{
	HMODULE mHandle = GetModuleHandle(ModuleName);
	return (mHandle == NULL) ? NULL : GetProcAddress(mHandle, (LPCSTR)Ordinal);
}

typedef void (*XNOTIFYQUEUEUI)(uint32_t type, uint32_t userIndex, uint64_t areas, const wchar_t *displayText, void *pContextData);
XNOTIFYQUEUEUI XNotifyQueueUI = static_cast<XNOTIFYQUEUEUI>(ResolveFunction("xam.xex", 656));*/
 
// dll entrypoint
BOOL APIENTRY DllMain(HANDLE hInstDLL, DWORD dwReason, LPVOID lpReserved) {
	HANDLE hKernel = NULL;
	PDWORD pdwUsbdAuthFunction = NULL;
	if (dwReason == DLL_PROCESS_ATTACH) {
		
		// only patch if 17559, due to hardcoded function address
		if (XboxKrnlVersion->Build == 17559) {
			DbgPrint("GoldSpoof | Patching kernel %i\n", XboxKrnlVersion->Build);
			POKE_32(0x816DACE0, 0x38600006);	//XamUserGetMembershipTierFromXUID
			POKE_32(0x816DAC84, 0x38600006);	//XamUserGetMembershipTier
			POKE_32(0x81A3CD60, 0x38600001);
			
			POKE_32(0x816DD688, 0x39600001);	//XamUserCheckPrivilege
			POKE_32(0x816DD6E8, 0x39600001);	//XamUserCheckPrivilege
			POKE_32(0x816DD6F4, 0x39600001);	//XamUserCheckPrivilege
			POKE_32(0x816DD6FC, 0x39600001);	//XamUserCheckPrivilege
			
			/* BOOL bIsDevkit = *(DWORD*)0x8E038610 & 0x8000 ? FALSE : TRUE; // Simple devkit check
			if (!bIsDevkit) // We don't need a stupid notify on Devkit. Smh...
				XNotifyQueueUI(14, 255, 2, L"You are golden!", NULL); */
		} else {
			DbgPrint("GoldSpoof | NOT patching: kernel is %i\n", XboxKrnlVersion->Build);
		}
		
		*(WORD*)((DWORD)hInstDLL + 64) = 1;
		return FALSE;
	}

	DbgPrint("GoldSpoof | Unload");
	return TRUE;
}