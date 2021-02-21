#include "driver_interface.hpp"
#include <memory>

int main ( )
{
	auto drv = std::make_unique<driver>( );

	std::cout << std::hex << "Current process base address 0x" << drv->get_baseaddress( GetCurrentProcessId( ) ) << std::endl;
	
	Sleep( 3000 );
}