#include <Uefi.h>

EFI_STATUS EFIAPI efi_main(EFI_HANDLE image, EFI_SYSTEM_TABLE *systable)
{
    EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *stdout = systable->ConOut;
    stdout->ClearScreen(stdout);
    stdout->OutputString(stdout, u"Hello, world!\n");
    return RETURN_SUCCESS;
}