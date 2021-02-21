#pragma once
#include <cstdint>

constexpr ULONG base_address = CTL_CODE( FILE_DEVICE_UNKNOWN , 0x997 , METHOD_BUFFERED , FILE_SPECIAL_ACCESS );

struct comm_struct
{
	std::uint32_t pid;
	PVOID outbase;
};
