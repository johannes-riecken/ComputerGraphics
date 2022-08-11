//
// OpenCL +  OpenGL particle animation example
//
// Author: Alexey V. Boreskov <steps3d@gmail.com>, <steps3d@narod.ru>
//

#include	<CL/cl.h>
#include	<CL/cl_gl.h>

#include	"GlutRotateWindow.h"
#include	"Program.h"
#include	"glUtilities.h"
#include	"BasicMesh.h"
#include	"Texture.h"
#include	"randUtils.h"

#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>

#define	NUM_PARTICLES	(5*1024)

const char * oclErrorString (cl_int error)
{
    static const char* errorString[] = {
        "CL_SUCCESS",
        "CL_DEVICE_NOT_FOUND",
        "CL_DEVICE_NOT_AVAILABLE",
        "CL_COMPILER_NOT_AVAILABLE",
        "CL_MEM_OBJECT_ALLOCATION_FAILURE",
        "CL_OUT_OF_RESOURCES",
        "CL_OUT_OF_HOST_MEMORY",
        "CL_PROFILING_INFO_NOT_AVAILABLE",
        "CL_MEM_COPY_OVERLAP",
        "CL_IMAGE_FORMAT_MISMATCH",
        "CL_IMAGE_FORMAT_NOT_SUPPORTED",
        "CL_BUILD_PROGRAM_FAILURE",
        "CL_MAP_FAILURE",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "CL_INVALID_VALUE",
        "CL_INVALID_DEVICE_TYPE",
        "CL_INVALID_PLATFORM",
        "CL_INVALID_DEVICE",
        "CL_INVALID_CONTEXT",
        "CL_INVALID_QUEUE_PROPERTIES",
        "CL_INVALID_COMMAND_QUEUE",
        "CL_INVALID_HOST_PTR",
        "CL_INVALID_MEM_OBJECT",
        "CL_INVALID_IMAGE_FORMAT_DESCRIPTOR",
        "CL_INVALID_IMAGE_SIZE",
        "CL_INVALID_SAMPLER",
        "CL_INVALID_BINARY",
        "CL_INVALID_BUILD_OPTIONS",
        "CL_INVALID_PROGRAM",
        "CL_INVALID_PROGRAM_EXECUTABLE",
        "CL_INVALID_KERNEL_NAME",
        "CL_INVALID_KERNEL_DEFINITION",
        "CL_INVALID_KERNEL",
        "CL_INVALID_ARG_INDEX",
        "CL_INVALID_ARG_VALUE",
        "CL_INVALID_ARG_SIZE",
        "CL_INVALID_KERNEL_ARGS",
        "CL_INVALID_WORK_DIMENSION",
        "CL_INVALID_WORK_GROUP_SIZE",
        "CL_INVALID_WORK_ITEM_SIZE",
        "CL_INVALID_GLOBAL_OFFSET",
        "CL_INVALID_EVENT_WAIT_LIST",
        "CL_INVALID_EVENT",
        "CL_INVALID_OPERATION",
        "CL_INVALID_GL_OBJECT",
        "CL_INVALID_BUFFER_SIZE",
        "CL_INVALID_MIP_LEVEL",
        "CL_INVALID_GLOBAL_WORK_SIZE",
    };

    const int errorCount = sizeof(errorString) / sizeof(errorString[0]);

    const int index = -error;

    return (index >= 0 && index < errorCount) ? errorString[index] : "Unspecified Error";
}

class	MeshWindow : public GlutRotateWindow
{
	vec3         eye;
	vec3         p [NUM_PARTICLES];
	vec3         v [NUM_PARTICLES];
	Program      program;
	VertexArray  vao;
	VertexBuffer posBuf, velBuf;

						// OpenCL part
	cl_platform_id   platform;
	cl_device_id     device;
	cl_context       context;
	cl_command_queue queue;
	cl_mem           pBuf, vBuf;
	cl_program       prog;
	cl_kernel        kernel;

public:
	MeshWindow () : GlutRotateWindow ( 200, 200, 400, 400, "OpenCL particles" )
	{
		eye = vec3  ( 3, 3, 3 );

		if ( !program.loadProgram ( "opencl-particles.glsl" ) )
		{
			printf ( "Error loading shader: %s\n", program.getLog ().c_str () );
			exit   ( 1 );
		}

		initParticles ();

		posBuf.create  ();
		posBuf.bind    ( GL_ARRAY_BUFFER );
		posBuf.setData ( NUM_PARTICLES * sizeof ( vec3 ), p, GL_DYNAMIC_DRAW );
		posBuf.unbind  ();
		velBuf.create  ();
		velBuf.bind    ( GL_ARRAY_BUFFER );
		velBuf.setData ( NUM_PARTICLES * sizeof ( vec3 ), v, GL_DYNAMIC_DRAW );
		velBuf.unbind  ();

		initOpenCL ();

		cl_int err;

		pBuf  = clCreateFromGLBuffer( context, CL_MEM_READ_WRITE, posBuf.getId (), &err );

		if ( err != CL_SUCCESS )
			printf ( "Error\n" );

		vBuf  = clCreateFromGLBuffer( context, CL_MEM_READ_WRITE, velBuf.getId (), &err );

		if ( err != CL_SUCCESS )
			printf ( "Error\n" );

		vao.create ();
		vao.bind   ();
		posBuf.bind       ( GL_ARRAY_BUFFER );
		posBuf.setAttrPtr ( 0, 3, sizeof ( vec3 ), (void *) 0 );
		velBuf.bind       ( GL_ARRAY_BUFFER );
		velBuf.setAttrPtr ( 1, 3, sizeof ( vec3 ), (void *) 0 );
		vao.unbind     ();
	}

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

	void	initOpenCL ()
	{
		cl_int err = clGetPlatformIDs( 1, &platform, NULL );

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


#ifdef	_WIN32
	cl_context_properties props [] =
	{
	  CL_GL_CONTEXT_KHR,
		(cl_context_properties) wglGetCurrentContext(),
	  CL_WGL_HDC_KHR,
		(cl_context_properties) wglGetCurrentDC(),
	  CL_CONTEXT_PLATFORM,
		(cl_context_properties) platform,
	  0      // terminator
	};
#else
	cl_context_properties props [] =
	{
	  CL_GL_CONTEXT_KHR,
		(cl_context_properties) glXGetCurrentContext(),
	  CL_GLX_DISPLAY_KHR,
		(cl_context_properties) glXGetCurrentDisplay(),
	  CL_CONTEXT_PLATFORM,
		(cl_context_properties) platform,
	  0      // terminator
	};
#endif

		context = clCreateContext ( props, 1, &device, NULL, NULL, &err );

		if ( err != CL_SUCCESS )
		{
			printf ( "Error creating OpenCL context.\n" );

			exit ( 1 );
		}


		queue = clCreateCommandQueue ( context, device, NULL, &err );

		if ( err != CL_SUCCESS )
		{
			printf ( "Error creating OpenCL command queue.\n" );

			exit ( 1 );
		}

		const char * source = loadFile( "particles.cl" );

		if ( source == NULL )
		{
			printf ( "Error loading kernel source.\n" );

			exit ( 1 );
		}

		prog = clCreateProgramWithSource( context, 1, &source, NULL, &err );

		if ( err != CL_SUCCESS )
		{
			printf ( "Error creating OpenCL program.\n" );

			exit ( 1 );
		}

		if ( clBuildProgram( prog, 1, &device, NULL, NULL, NULL ) != CL_SUCCESS )
		{
			size_t	logSize;

			clGetProgramBuildInfo( prog, device, CL_PROGRAM_BUILD_LOG, 0, NULL, &logSize );

			char * str = new char [logSize + 1];

			clGetProgramBuildInfo( prog, device, CL_PROGRAM_BUILD_LOG, logSize, str, NULL );

			printf ( "Error building program:\n%s\n", str );

			delete str;

			exit ( 1 );
		}

		kernel = clCreateKernel( prog, "animate", &err );

		if ( err != CL_SUCCESS )
		{
			printf ( "Error creating OpenCL kernel.\n" );

			exit ( 1 );
		}
	}

	void	doOpenCL ( float dt )
	{
		int    n          = NUM_PARTICLES;
		size_t localSize  = 512;			// work-items per work-group
		size_t globalSize = n;				// total work-items
		cl_int err;

		glFinish ();

		clEnqueueAcquireGLObjects( queue, 1, &pBuf, 0, NULL, NULL );
		clEnqueueAcquireGLObjects( queue, 1, &vBuf, 0, NULL, NULL );

		clSetKernelArg( kernel, 0, sizeof (pBuf), &pBuf );
		clSetKernelArg( kernel, 1, sizeof (vBuf), &vBuf );
		clSetKernelArg( kernel, 2, sizeof (dt),   &dt   );
		clSetKernelArg( kernel, 3, sizeof (n),    &n    );

		err = clEnqueueNDRangeKernel ( queue, kernel, 1, NULL, &globalSize, &localSize, 0, NULL, NULL );

		if ( err != CL_SUCCESS )
		{
			printf ( "Error launcing OpenCL kernel (%d - %s).\n", err, oclErrorString (err) );

			exit ( 1 );
		}

		clEnqueueReleaseGLObjects( queue, 1, &pBuf, 0, NULL, NULL );
		clEnqueueReleaseGLObjects( queue, 1, &vBuf, 0, NULL, NULL );

		clFinish ( queue );
	}

	void	initParticles ()
	{
		for ( int i = 0; i < NUM_PARTICLES; i++ )
		{
			p [i] = vec3 ( randUniform ( -0.1, 0.1 ), randUniform ( -0.1, 0.1 ), randUniform ( -0.1, 0.1 ) );
			v [i] = vec3 ( randUniform ( -0.1, 0.1 ), randUniform ( -0.1, 0.1 ), randUniform ( -0.1, 0.1 ) );
		}
	}

	void redisplay ()
	{
		static float lastTime = 0;

		float t  = 0.001f * glutGet ( GLUT_ELAPSED_TIME );

		doOpenCL ( lastTime - t );

		glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		mat4	mv = getRotation ();

		program.bind ();
		program.setUniformMatrix ( "mv",  mv );
		vao.bind ();

		glDrawArrays ( GL_POINTS, 0, NUM_PARTICLES );

		vao.unbind ();
		program.unbind ();

		lastTime = t;
	}

	void reshape ( int w, int h )
	{
		glViewport ( 0, 0, (GLsizei)w, (GLsizei)h );

		mat4 proj = perspective ( 60.0f, (float)w / (float)h, 0.5f, 20.0f ) * lookAt ( eye, vec3 :: zero, vec3 ( 0, 1, 0 ) );

		program.bind ();
		program.setUniformMatrix ( "proj",    proj );
		program.unbind ();
	}
};

int main ( int argc, char * argv [] )
{
	GlutWindow::init( argc, argv );

	MeshWindow	win;

	GlutWindow::run ();

	return 0;
}

