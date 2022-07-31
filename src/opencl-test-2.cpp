#include	<stdio.h>
#include	<stdlib.h>
#include	<CL/cl.h>

#define 	N	(1024*1024)

const char * loadFile ( const char * fileName )
{
    printf ( "Loading %s\n", fileName );

	FILE * file = fopen ( fileName, "rb" );
	
	if ( file == NULL )
	{
		printf ( "Error opening %s\n", fileName );
		
        return NULL;
	}

	fseek ( file, 0, SEEK_END );
	
	size_t	size = ftell ( file );
	
	if ( size < 1 )
	{
		fclose ( file );
		printf ( "Error loading file %s\n", fileName );
		
		return NULL;
	}
	
	char * buf = (char *) malloc ( size + 1 );
	
	fseek ( file, 0, SEEK_SET );
	
	if ( fread ( buf, 1, size, file ) != size )
	{
		fclose ( file );
		free   ( buf  );
		printf ( "Error loading file %s\n", fileName );
		
		return NULL;
	}

	fclose ( file );
	
	buf [size] = '\0';
	
	return buf;
}

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
		
	err = clGetDeviceIDs( platform, CL_DEVICE_TYPE_GPU, 1, &device, NULL );
	
	if ( err != CL_SUCCESS )
	{
		printf ( "No OpenCL device found.\n" );
		
		exit ( 1 );
	}
		
	cl_context context = clCreateContext ( NULL, 1, &device, NULL, NULL, &err );
	
	if ( err != CL_SUCCESS )
	{
		printf ( "Error creating OpenCL context.\n" );
		
		exit ( 1 );
	}
	
	cl_command_queue queue = clCreateCommandQueue ( context, device, NULL, &err );
	
	if ( err != CL_SUCCESS )
	{
		printf ( "Error creating OpenCL command queue.\n" );
		
		exit ( 1 );
	}

	float * buf = new float [N];
	int     n   = N;
	
	cl_mem outBuf = clCreateBuffer ( context, CL_MEM_WRITE_ONLY, N * sizeof ( float ), NULL, &err );
	
	if ( err != CL_SUCCESS )
	{
		printf ( "Error creating OpenCL memory buffer.\n" );
		
		exit ( 1 );
	}

	const char * source = loadFile ( "test.cl" );
	
	if ( source == NULL )
	{
		printf ( "Error loading kernel source.\n" );
		
		exit ( 1 );
	}

	cl_program program = clCreateProgramWithSource(  context, 1, &source, NULL, &err );
	
	if ( err != CL_SUCCESS )
	{
		printf ( "Error creating OpenCL program.\n" );
		
		exit ( 1 );
	}

	if ( clBuildProgram( program, 1, &device, NULL, NULL, NULL ) != CL_SUCCESS )
	{
		size_t	logSize;
		
		clGetProgramBuildInfo ( program, device, CL_PROGRAM_BUILD_LOG, 0, NULL, &logSize );
		
		char * str = new char [logSize + 1];
		
		clGetProgramBuildInfo ( program, device, CL_PROGRAM_BUILD_LOG, logSize, str, NULL );
		
		printf ( "Error building program:\n%s\n", str );
		
		delete str;
		
		exit ( 1 );
	}
	
	cl_kernel kernel = clCreateKernel( program, "test", &err );
	
	if ( err != CL_SUCCESS )
	{
		printf ( "Error creating OpenCL kernel.\n" );
		
		exit ( 1 );
	}

	clSetKernelArg( kernel, 0, sizeof(outBuf), &outBuf );
	clSetKernelArg( kernel, 1, sizeof(n),      &n );
	
	size_t localSize  = 512;			// work-items per work-group
	size_t globalSize = N;				// total work-items
	
	err = clEnqueueNDRangeKernel( queue, kernel, 1, NULL, &globalSize, &localSize, 0, NULL, NULL );
	
	if ( err != CL_SUCCESS )
	{
		printf ( "Error launcing OpenCL kernel.\n" );
		
		exit ( 1 );
	}

	err = clEnqueueReadBuffer( queue, outBuf, CL_TRUE, 0, N*sizeof(float), buf, 0, NULL, NULL );
	
	if ( err != CL_SUCCESS )
	{
		printf ( "Error getting array back.\n" );
		
		exit ( 1 );
	}

	printf ( "%f %f\n", buf [0], buf[1024] );
	
	delete buf;
	free ( (void *)source );
	
	clReleaseKernel( kernel );
	clReleaseCommandQueue( queue );
	clReleaseContext( context );
	clReleaseMemObject( outBuf );
	
	return 0;
}