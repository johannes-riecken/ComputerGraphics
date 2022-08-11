#include	<stdio.h>
#include	<stdlib.h>
#include	<math.h>
#include	<CL/cl.h>

#define 	N	1024

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

	float * a = new float [N*N];
	float * b = new float [N*N];
	float * c = new float [N*N];
	int     n = N;
	int		numBytes = N*N*sizeof(float);

	for ( int i = 0; i < N; i++ )
		for ( int j = 0; j < N; j++ )
		{
			int	k = i + j*N;

			a [k] = i == j ? 2 : 0;
			b [k] = i + j;
		}

	cl_mem aBuf = clCreateBuffer( context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,  numBytes, a, &err );
	cl_mem bBuf = clCreateBuffer( context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,  numBytes, b, &err );
	cl_mem cBuf = clCreateBuffer( context, CL_MEM_WRITE_ONLY, numBytes, NULL, &err );

	const char * source = loadFile( "matrixMul.cl" );

	if ( source == NULL )
	{
		printf ( "Error loading kernel source.\n" );

		exit ( 1 );
	}

	cl_program program = clCreateProgramWithSource( context, 1, &source, NULL, &err );

	if ( err != CL_SUCCESS )
	{
		printf ( "Error creating OpenCL program.\n" );

		exit ( 1 );
	}

	if ( clBuildProgram( program, 1, &device, NULL, NULL, NULL ) != CL_SUCCESS )
	{
		size_t	logSize;

		clGetProgramBuildInfo( program, device, CL_PROGRAM_BUILD_LOG, 0, NULL, &logSize );

		char * str = new char [logSize + 1];

		clGetProgramBuildInfo( program, device, CL_PROGRAM_BUILD_LOG, logSize, str, NULL );

		printf ( "Error building program:\n%s\n", str );

		delete str;

		exit ( 1 );
	}

	cl_kernel kernel = clCreateKernel( program, "matrixMul", &err );

	if ( err != CL_SUCCESS )
	{
		printf ( "Error creating OpenCL kernel.\n" );

		exit ( 1 );
	}

	clSetKernelArg( kernel, 0, sizeof(aBuf), &aBuf );
	clSetKernelArg( kernel, 1, sizeof(bBuf), &bBuf );
	clSetKernelArg( kernel, 2, sizeof(cBuf), &cBuf );
	clSetKernelArg( kernel, 3, sizeof(n),      &n );

	size_t localSize [] = { 16, 16 };	// work-items per work-group
	size_t globalSize  [] = { N, N };	// total work-items

	err = clEnqueueNDRangeKernel( queue, kernel, 2, NULL, globalSize, localSize, 0, NULL, NULL );

	if ( err != CL_SUCCESS )
	{
		printf ( "Error launcing OpenCL kernel.\n" );

		exit ( 1 );
	}

	err = clEnqueueReadBuffer( queue, cBuf, CL_TRUE, 0, numBytes, c, 0, NULL, NULL );

	if ( err != CL_SUCCESS )
	{
		printf ( "Error getting array back.\n" );

		exit ( 1 );
	}

	for ( int i = 0; i < N; i++ )
		for ( int j = 0; j < N; j++ )
		{
			int	  k = i + j*N;
			float r = (i+j) * 2.0f;

			if ( fabs ( c [k] - r ) > 0.001f )
				printf ( "Error\n" );
		}


	delete a;
	delete b;
	delete c;

	free ( (void *)source );

	clReleaseKernel( kernel );
	clReleaseCommandQueue( queue );
	clReleaseContext( context );
	clReleaseMemObject( aBuf );
	clReleaseMemObject( bBuf );
	clReleaseMemObject( cBuf );

	return 0;
}