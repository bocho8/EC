#include <ntifs.h>

void debug_print(PCSTR text)
{
	KdPrintEx((DPLFLTR_IHVDRIVER_ID, DPFLTR_INFO_LEVEL, text));
}