#pragma once
#include <Windows.h>
#include <iostream>

constexpr ULONG base_address = CTL_CODE( FILE_DEVICE_UNKNOWN , 0x997 , METHOD_BUFFERED , FILE_SPECIAL_ACCESS );


class driver
{
	HANDLE driver_handle_;

	struct comm_struct
	{
		std::uint32_t pid;
		PVOID outbase;
	};
	
public:
	
	driver ( ) : driver_handle_( nullptr )
	{
		this->driver_handle_ = CreateFileA( R"(\\.\MyDriver)" , GENERIC_READ , 0 , nullptr , OPEN_EXISTING , FILE_ATTRIBUTE_NORMAL , nullptr );

		if ( this->driver_handle_ == INVALID_HANDLE_VALUE )
			std::cout << "failed to open handle to driver" << std::endl;
	}

	auto get_baseaddress ( std::uint32_t pid ) -> std::uintptr_t
	{
		void* outbase = 0;
		comm_struct input_struct { pid, outbase };

		DeviceIoControl( this->driver_handle_ , base_address , &input_struct , sizeof( input_struct ) , &input_struct , sizeof( input_struct ) , nullptr , nullptr );

		return reinterpret_cast<std::uintptr_t>( outbase );
	}

	~driver()
	{
		CloseHandle( this->driver_handle_ );
	}
	
};