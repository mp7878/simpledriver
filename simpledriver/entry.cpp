#include "nt.hpp"
#include "comm.hpp"

auto PopulateIO( PDEVICE_OBJECT device_object , PIRP irp ) -> NTSTATUS
{
	UNREFERENCED_PARAMETER( device_object );

	IoCompleteRequest( irp , IO_NO_INCREMENT );
	return irp->IoStatus.Status;
}

auto DriverControl( DEVICE_OBJECT* device_object , PIRP irp ) -> NTSTATUS
{
	UNREFERENCED_PARAMETER( device_object );

	const auto stack_location = IoGetCurrentIrpStackLocation( irp );

	auto stack = IoGetCurrentIrpStackLocation( irp );
	auto buffer = static_cast<comm_struct*>( irp->AssociatedIrp.SystemBuffer );


	if ( stack )
	{
		const auto ctl_code = stack->Parameters.DeviceIoControl.IoControlCode;

		if ( ctl_code == base_address && buffer->pid )
		{
			PEPROCESS target_proc {};
			PsLookupProcessByProcessId( (HANDLE)buffer->pid , &target_proc );
			buffer->outbase = PsGetProcessSectionBaseAddress( target_proc );
		}		
	}

	IoCompleteRequest( irp , IO_NO_INCREMENT );
	return STATUS_SUCCESS;
}

auto DriverUnload( PDRIVER_OBJECT driver_object ) -> NTSTATUS
{
	auto dos_device = create_unicode( L"\\DosDevices\\MyDriver" );
	IoDeleteSymbolicLink( &dos_device );
	IoDeleteDevice( driver_object->DeviceObject );
	return STATUS_SUCCESS;
}

extern "C" auto DriverEntry ( PDRIVER_OBJECT driver_object , PUNICODE_STRING reg_path ) -> NTSTATUS
{
	UNREFERENCED_PARAMETER( reg_path );

	DBG_LOG( "Driver loaded" );
	
	DEVICE_OBJECT* device_object {};
	auto dos_device = create_unicode( L"\\DosDevices\\MyDriver" );
	auto device = create_unicode( L"\\Device\\MyDriver" );
	

	if ( !NT_SUCCESS( IoCreateDevice( driver_object , 0 , &device , FILE_DEVICE_UNKNOWN , FILE_DEVICE_SECURE_OPEN , false , &device_object ) ) )
		return STATUS_FAILED_DRIVER_ENTRY;

	if ( !NT_SUCCESS( IoCreateSymbolicLink( &dos_device , &device ) ) )
		return STATUS_FAILED_DRIVER_ENTRY;

	driver_object->MajorFunction[ IRP_MJ_DEVICE_CONTROL ] = DriverControl;
	driver_object->DriverUnload = reinterpret_cast<PDRIVER_UNLOAD>( DriverUnload );
	driver_object->MajorFunction[ IRP_MJ_CREATE ] = PopulateIO;
	driver_object->MajorFunction[ IRP_MJ_CLOSE ] = PopulateIO;
	
	return STATUS_SUCCESS;
	
}
