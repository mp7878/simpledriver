#pragma once
#include <ntifs.h>

#define DBG_LOG(fmt, ...) DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL, "[simpledriver][" __FUNCTION__ "] " fmt "\n", ##__VA_ARGS__)

extern "C" __declspec( dllimport ) PVOID NTAPI PsGetProcessSectionBaseAddress( PEPROCESS Process );

inline UNICODE_STRING create_unicode( const wchar_t* text )
{
	UNICODE_STRING return_string {};
	RtlInitUnicodeString( &return_string , text );
	return return_string;
}