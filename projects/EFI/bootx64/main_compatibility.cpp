#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/DevicePathLib.h>
#include <Library/PrintLib.h>
#include <Protocol/SimpleFileSystem.h>
#include <Protocol/LoadedImage.h>
#include <IndustryStandard/PeImage.h>
#include <Guid/GlobalVariable.h>
#include <Protocol/UsbFunctionIo.h>
#include <Protocol/Smbios.h>
#include <intrin.h>
#include "globals.h"

extern "C"
{
	//
	// EFI common variables
	//
	EFI_SYSTEM_TABLE     *gST = 0;
	EFI_RUNTIME_SERVICES *gRT = 0;
	EFI_BOOT_SERVICES    *gBS = 0;

	//
	// EFI image variables
	//
	QWORD EfiBaseAddress        = 0;
	QWORD EfiBaseSize           = 0;
	DWORD GlobalStatusVariable  = 0;
	QWORD ntoskrnl_fbase        = 0;
	QWORD ntoskrnl_fsize        = 0;

	//
	// EFI global variables
	//
	EFI_GET_MEMORY_MAP oGetMemoryMap;
	EFI_ALLOCATE_PAGES oAllocatePages;
	EFI_EXIT_BOOT_SERVICES oExitBootServices;
	EFI_STATUS EFIAPI ExitBootServicesHook(EFI_HANDLE ImageHandle, UINTN MapKey);
	EFI_STATUS EFIAPI AllocatePagesHook(EFI_ALLOCATE_TYPE Type, EFI_MEMORY_TYPE MemoryType, UINTN Pages, EFI_PHYSICAL_ADDRESS *Memory);
	EFI_STATUS EFIAPI GetMemoryMapHook(UINTN *MemoryMapSize, EFI_MEMORY_DESCRIPTOR* MemoryMap, UINTN* MapKey, UINTN* DescriptorSize, UINT32* DescriptorVersion);
}

#define Print(Text) \
{ \
unsigned short private_text[] = Text; \
gST->ConOut->OutputString(gST->ConOut, private_text); \
} \

EFI_GUID gEfiLoadedImageProtocolGuid    = { 0x5B1B31A1, 0x9562, 0x11D2, { 0x8E, 0x3F, 0x00, 0xA0, 0xC9, 0x69, 0x72, 0x3B }};

inline void PressAnyKey()
{
	EFI_STATUS         Status;
	EFI_EVENT          WaitList;
	EFI_INPUT_KEY      Key;
	UINTN              Index;
	Print(FILENAME L" " L"\n\r");
	Print(FILENAME L" " L" /$$$$$$$$  /$$$$$$        /$$        /$$$$$$   /$$$$$$  /$$$$$$$  /$$$$$$$$ /$$$$$$$                  \n\r");
	Print(FILENAME L" " L"| $$_____/ /$$__  $$      | $$       /$$__  $$ /$$__  $$| $$__  $$| $$_____/| $$__  $$                 \n\r");
	Print(FILENAME L" " L"| $$      | $$  |__/      | $$      | $$  | $$| $$  | $$| $$  | $$| $$      | $$  | $$                 \n\r");
	Print(FILENAME L" " L"| $$$$$   | $$            | $$      | $$  | $$| $$$$$$$$| $$  | $$| $$$$$   | $$  | $$                 \n\r");
	Print(FILENAME L" " L"| $$__/   | $$            | $$      | $$  | $$| $$__  $$| $$  | $$| $$__/   | $$  | $$                 \n\r");
	Print(FILENAME L" " L"| $$      | $$    $$      | $$      | $$  | $$| $$  | $$| $$  | $$| $$      | $$  | $$                 \n\r");
	Print(FILENAME L" " L"| $$$$$$$$|  $$$$$$/      | $$$$$$$$|  $$$$$$/| $$  | $$| $$$$$$$/| $$$$$$$$| $$$$$$$/                 \n\r");
	Print(FILENAME L" " L"|________/ |______/       |________/ |______/ |__/  |__/|_______/ |________/|_______/                  \n\r");
	Print(FILENAME L" " L"                                                                                                       \n\r");
	Print(FILENAME L" " L"                                                                                                       \n\r");
	Print(FILENAME L" " L"                                                                                                       \n\r");
	Print(FILENAME L" " L" /$$$$$$$  /$$$$$$$  /$$$$$$$$  /$$$$$$   /$$$$$$        /$$$$$$$$  /$$     /$$                        \n\r");
	Print(FILENAME L" " L"| $$__  $$| $$__  $$| $$_____/ /$$__  $$ /$$__  $$      | $$_____//$$$$   /$$$$                        \n\r");
	Print(FILENAME L" " L"| $$  | $$| $$  | $$| $$      | $$  |__/| $$  |__/      | $$     |_  $$  |_  $$                        \n\r");
	Print(FILENAME L" " L"| $$$$$$$/| $$$$$$$/| $$$$$   |  $$$$$$ |  $$$$$$       | $$$$$    | $$    | $$                        \n\r");
	Print(FILENAME L" " L"| $$____/ | $$__  $$| $$__/    |____  $$ |____  $$      | $$__/    | $$    | $$                        \n\r");
	Print(FILENAME L" " L"| $$      | $$  | $$| $$       /$$  | $$ /$$  | $$      | $$       | $$    | $$                        \n\r");
	Print(FILENAME L" " L"| $$      | $$  | $$| $$$$$$$$|  $$$$$$/|  $$$$$$/      | $$      /$$$$$$ /$$$$$$                      \n\r");
	Print(FILENAME L" " L"|__/      |__/  |__/|________/ |______/  |______/       |__/     |______/|______/                      \n\r");
	Print(FILENAME L" " L"                                                                                                       \n\r");
	Print(FILENAME L" " L"                                                                                                       \n\r");
	Print(FILENAME L" " L"                                                                                                       \n\r");
	Print(FILENAME L" " L" /$$$$$$$$ /$$$$$$         /$$$$$$   /$$$$$$  /$$   /$$ /$$$$$$$$ /$$$$$$ /$$   /$$ /$$   /$$ /$$$$$$$$\n\r");
	Print(FILENAME L" " L"|__  $$__//$$__  $$       /$$__  $$ /$$__  $$| $$$ | $$|__  $$__/|_  $$_/| $$$ | $$| $$  | $$| $$_____/\n\r");
	Print(FILENAME L" " L"   | $$  | $$  | $$      | $$  |__/| $$  | $$| $$$$| $$   | $$     | $$  | $$$$| $$| $$  | $$| $$      \n\r");
	Print(FILENAME L" " L"   | $$  | $$  | $$      | $$      | $$  | $$| $$ $$ $$   | $$     | $$  | $$ $$ $$| $$  | $$| $$$$$   \n\r");
	Print(FILENAME L" " L"   | $$  | $$  | $$      | $$      | $$  | $$| $$  $$$$   | $$     | $$  | $$  $$$$| $$  | $$| $$__/   \n\r");
	Print(FILENAME L" " L"   | $$  | $$  | $$      | $$    $$| $$  | $$| $$|  $$$   | $$     | $$  | $$|  $$$| $$  | $$| $$      \n\r");
	Print(FILENAME L" " L"   | $$  |  $$$$$$/      |  $$$$$$/|  $$$$$$/| $$ |  $$   | $$    /$$$$$$| $$ |  $$|  $$$$$$/| $$$$$$$$\n\r");
	Print(FILENAME L" " L"   |__/   |______/        |______/  |______/ |__/  |__/   |__/   |______/|__/  |__/ |______/ |________/\n\r");
	do {
		WaitList = gST->ConIn->WaitForKey;
		Status = gBS->WaitForEvent(1, &WaitList, &Index);
		gST->ConIn->ReadKeyStroke(gST->ConIn, &Key);
		if (Key.ScanCode == SCAN_F11)
			break;
	} while ( 1 );
	gST->ConOut->ClearScreen(gST->ConOut);
	gST->ConOut->SetAttribute(gST->ConOut, EFI_WHITE | EFI_BACKGROUND_BLACK);
}

extern "C" EFI_STATUS EFIAPI EfiMain(IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE *SystemTable)
{
	gRT = SystemTable->RuntimeServices;
	gBS = SystemTable->BootServices;
	gST = SystemTable;


	EFI_LOADED_IMAGE_PROTOCOL *current_image;
	

	//
	// Get current image information
	//
	if (EFI_ERROR(gBS->HandleProtocol(ImageHandle, &gEfiLoadedImageProtocolGuid, (void**)&current_image)))
	{
		Print(FILENAME L" Failed to start " SERVICE_NAME L" service.");
		return 0;
	}


	UINTN page_count = EFI_SIZE_TO_PAGES (current_image->ImageSize);
	VOID  *rwx       = 0;


	//
	// allocate space for SwapMemory
	//
	if (EFI_ERROR(gBS->AllocatePages(AllocateAnyPages, EfiBootServicesCode, page_count, (EFI_PHYSICAL_ADDRESS*)&rwx)))
	{
		Print(FILENAME L" Failed to start " SERVICE_NAME L" service.");
		return 0;
	}

	//
	// swap our context to new memory region
	//
	SwapMemory( (QWORD)current_image->ImageBase, (QWORD)current_image->ImageSize, (QWORD)rwx );


	//
	// clear old image from memory
	//
	for (QWORD i = current_image->ImageSize; i--;)
	{
		((unsigned char*)current_image->ImageBase)[i] = 0;
	}


	//
	// allocate space for ntoskrnl file
	//
	if (EFI_ERROR(gBS->AllocatePages(AllocateAnyPages, EfiBootServicesData, EFI_SIZE_TO_PAGES(SIZE_32MB), &ntoskrnl_fbase)))
	{
		Print(FILENAME L" Failed to start " SERVICE_NAME L" service.");
		return 0;
	}


	//
	// save our new EFI address information
	//
	EfiBaseAddress  = (QWORD)rwx;
	EfiBaseSize     = (QWORD)EFI_PAGES_TO_SIZE(page_count);


	//
	// GetMemoryMapHook: Reserve RAM space
	//
	oGetMemoryMap = gBS->GetMemoryMap;
	gBS->GetMemoryMap = GetMemoryMapHook;


	//
	// ExitBootServices: Output EFI status
	//
	oExitBootServices = gBS->ExitBootServices;
	gBS->ExitBootServices = ExitBootServicesHook;
	

	//
	// AllocatePages: winload/ntoskrnl.exe hooks
	//
	oAllocatePages = gBS->AllocatePages;
	gBS->AllocatePages = AllocatePagesHook;

	gST->ConOut->ClearScreen(gST->ConOut);
	gST->ConOut->SetAttribute(gST->ConOut, EFI_WHITE | EFI_BACKGROUND_BLACK);

	Print(FILENAME L" " SERVICE_NAME L" is now started");
	gST->ConOut->SetCursorPosition(gST->ConOut, 0, 1);
	PressAnyKey();
	return EFI_SUCCESS;
}

namespace km
{
	BOOLEAN initialize(QWORD ntoskrnl, QWORD fbase, QWORD fsize);
}

extern "C" EFI_STATUS EFIAPI ExitBootServicesHook(EFI_HANDLE ImageHandle, UINTN MapKey)
{
	gBS->ExitBootServices = oExitBootServices;

	gST->ConOut->ClearScreen(gST->ConOut);
	gST->ConOut->SetAttribute(gST->ConOut, EFI_WHITE | EFI_BACKGROUND_BLACK);

	if (GlobalStatusVariable)
	{
		Print(FILENAME L" Success -> " SERVICE_NAME L" service is running.");
	}
	else
	{
		Print(FILENAME L" Failure -> Unsupported OS.");
	}
	
	gST->ConOut->SetCursorPosition(gST->ConOut, 0, 1);
	PressAnyKey();

	if (!GlobalStatusVariable)
	{
		return 0;
	}

	return gBS->ExitBootServices(ImageHandle, MapKey);
}

unsigned __int64 __fastcall RtlFindExportedRoutineByName(QWORD BaseAddress, const char *ExportName)
{
	if (!strcmp_imp(ExportName, "NtImageInfo"))
	{
		pe_resolve_imports(BaseAddress, EfiBaseAddress);
		pe_clear_headers(EfiBaseAddress);

		__int64 (__fastcall *BlMmMapPhysicalAddressEx)(__int64 *a1, __int64 a2, unsigned __int64 a3, unsigned int a4, char a5);
		*(QWORD*)&BlMmMapPhysicalAddressEx = GetExportByName(get_caller_base((QWORD)_ReturnAddress()), "BlMmMapPhysicalAddressEx");
		
		QWORD EfiBaseVirtualAddress = EfiBaseAddress;	
		EFI_STATUS status = BlMmMapPhysicalAddressEx((__int64*)&EfiBaseVirtualAddress,
							EfiBaseVirtualAddress,
							EfiBaseSize,
							/*0x24000*/ 0x424000 /*0x1044008*/,
							0);
		
		if (!EFI_ERROR(status))
		{
			SwapMemory2(EfiBaseAddress, EfiBaseVirtualAddress);
			GlobalStatusVariable = km::initialize(BaseAddress, ntoskrnl_fbase, ntoskrnl_fsize);
			SwapMemory2(EfiBaseVirtualAddress, EfiBaseAddress);
		}
	}
	return GetExportByName(BaseAddress, ExportName);
}

typedef unsigned short WORD;
inline QWORD get_section_headers(QWORD nt_header)
{
	WORD machine = *(WORD*)(nt_header + 0x4);
	QWORD section_header = machine == 0x8664 ?
		nt_header + 0x0108 :
		nt_header + 0x00F8;
	return section_header;
}

inline QWORD get_nt_header(QWORD image)
{
	QWORD nt_header = (QWORD)*(DWORD*)(image + 0x03C) + image;
	if (nt_header == image)
	{
		return 0;
	}
	return nt_header;
}

static QWORD RtlImageNtHeaderExHook(DWORD Flags, VOID* Base, QWORD Size, OUT QWORD *OutHeaders)
{
	*OutHeaders = get_nt_header((QWORD)Base);
	//
	// file copied already
	//
	if (ntoskrnl_fsize != 0)
	{
		return 0;
	}

	//
	// make sure RtlImageNtHeaderEx is not called for headers only
	//
	if (Size <= 0x1000)
	{
		return 0;
	}

	DWORD es = 0;
	QWORD nt = *OutHeaders;
	QWORD sh = get_section_headers(nt);
	for (WORD i = 0; i < *(WORD*)(nt + 6); i++)
	{
		QWORD section = sh + ((QWORD)i * 40);
		unsigned char name[8];
		MemCopy(name, (void*)(section), 8);
		if (!strcmp_imp((const char*)name, "MINIEX"))
		{
			es = *(DWORD*)(section + 0x0C);
			break;
		}
	}

	if (es < 0x100000)
	{
		return 0;
	}

	MemCopy((void*)ntoskrnl_fbase, (void *)Base, Size);
	ntoskrnl_fsize = Size;

	return 0;
}

extern "C" EFI_STATUS EFIAPI AllocatePagesHook(EFI_ALLOCATE_TYPE Type, EFI_MEMORY_TYPE MemoryType, UINTN Pages, EFI_PHYSICAL_ADDRESS *Memory)
{
	QWORD return_address = (QWORD)_ReturnAddress();
	if (*(DWORD*)(return_address) == 0x48001F0F)
	{
		QWORD winload = get_caller_base(return_address);
		QWORD target_routine = GetExportByName(winload, "RtlFindExportedRoutineByName");
		if (target_routine)
		{
			*(QWORD*)(target_routine + 0x00) = 0x25FF;
			*(QWORD*)(target_routine + 0x06) = (QWORD)RtlFindExportedRoutineByName;

			//
			// hook routine for cloning ntoskrnl.exe file (Raw Version)
			//
			target_routine = GetExportByName(winload, "RtlImageNtHeaderEx");
			if (target_routine == 0)
			{
				target_routine = FindPattern(winload, (unsigned char *)"\x45\x33\xD2\x4D\x8B\xD8\x4D\x85\xC9", (unsigned char*)"xxxxxxxxx");
			}
			*(QWORD*)(target_routine + 0x00) = 0x25FF;
			*(QWORD*)(target_routine + 0x06) = (QWORD)RtlImageNtHeaderExHook;

			gBS->AllocatePages = oAllocatePages;
		}
	}
	return oAllocatePages(Type, MemoryType, Pages, Memory);
}

extern "C" EFI_STATUS EFIAPI GetMemoryMapHook(UINTN *MemoryMapSize, EFI_MEMORY_DESCRIPTOR* MemoryMap, UINTN* MapKey, UINTN* DescriptorSize, UINT32* DescriptorVersion)
{
	EFI_STATUS status = oGetMemoryMap(MemoryMapSize, MemoryMap, MapKey, DescriptorSize, DescriptorVersion);
	if (!EFI_ERROR(status))
	{
		VOID *map = MemoryMap;
		UINTN map_size = *MemoryMapSize;
		UINTN descriptor_size = *DescriptorSize;
		UINTN descriptor_count = map_size / descriptor_size;

		for (UINT32 i = 0; i < descriptor_count; i++)
		{
			EFI_MEMORY_DESCRIPTOR *entry = (EFI_MEMORY_DESCRIPTOR*)((char *)map + (i*descriptor_size));
			if (EfiBaseAddress >= entry->PhysicalStart && EfiBaseAddress <= (entry->PhysicalStart + EFI_PAGES_TO_SIZE(entry->NumberOfPages)))
			{
				entry->Type = EfiRuntimeServicesCode;
			}
		}
	}
	return status;
}

