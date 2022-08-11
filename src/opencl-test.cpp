#include	<stdio.h>
#include	<stdlib.h>
#include	<CL/cl.h>

int main ( int argc, char * argv [] )
{
	cl_platform_id platform;
	cl_device_id   device;
	cl_int	       err = clGetPlatformIDs( 1, &platform, NULL );

	if ( err != CL_SUCCESS )
	{
		printf ( "No OpenCL platform found.\n" );

		exit ( 1 );
	}

	char	buf1 [1024];
	char	buf2 [1024];

	clGetPlatformInfo ( platform, CL_PLATFORM_NAME,   sizeof(buf1), buf1, NULL );
	clGetPlatformInfo ( platform, CL_PLATFORM_VENDOR, sizeof(buf2), buf2, NULL );

	printf ( "Platform  : %s\nVendor    :%s\n", buf1, buf2 );

	err = clGetDeviceIDs( platform, CL_DEVICE_TYPE_GPU, 1, &device, NULL );

	if ( err != CL_SUCCESS )
	{
		printf ( "No OpenCL device found.\n" );

		exit ( 1 );
	}

	cl_ulong	sizGlobal, sizConst, sizLocal;

	clGetDeviceInfo( device, CL_DEVICE_NAME, sizeof(buf1), buf1, NULL );
	clGetDeviceInfo( device, CL_DEVICE_GLOBAL_MEM_SIZE, sizeof(sizGlobal), &sizGlobal, NULL );
	clGetDeviceInfo( device, CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE, sizeof(sizConst), &sizConst, NULL );
	clGetDeviceInfo( device, CL_DEVICE_LOCAL_MEM_SIZE, sizeof(sizLocal), &sizGlobal, NULL );

	printf ( "Name      : %s\nGlobal mem: %d\nConstant  :%d\nLocal     : %d\n", buf1, sizGlobal, sizConst, sizLocal );

	return 0;
}