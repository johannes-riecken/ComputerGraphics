//
// Refraction example 
//
// Author: Alexey V. Boreskov <steps3d@gmail.com>, <steps3d@narod.ru>
//

//
// Refraction example 
//
// Author: Alexey V. Boreskov <steps3d@gmail.com>, <steps3d@narod.ru>
//

#include	"GlutRotateWindow.h"
#include	"Program.h"
#include	"glUtilities.h"
#include	"BasicMesh.h"
#include	"Texture.h"

#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>

class	MyWindow : public GlutRotateWindow
{
	Program	    program, program2;
	BasicMesh * mesh;
	BasicMesh * cube;
	Texture		cubeMap;
	vec3		eye;

public:
	MyWindow () : GlutRotateWindow ( 200, 200, 400, 400, "Refraction" )
	{
		cubeMap.loadCubemap ( "Textures/Snow.dds" );
		
		if ( !program.loadProgram ( "glass-2.glsl" ) )
		{
			printf ( "Error loading shader: %s\n", program.getLog ().c_str () );
			
			exit ( 1 );
		}
		
		program.bind ();
		program.setTexture ( "cubeMap", 0 );
		program.unbind ();

		if ( !program2.loadProgram ( "cube-box.glsl" ) )
		{
			printf ( "Error loading shader: %s\n", program2.getLog ().c_str () );
			
			exit ( 1 );
		}
		
		program2.bind ();
		program2.setTexture ( "cubeMap", 0 );
		program2.unbind ();
		
		mesh = createTorus ( 1.5, 3, 30, 30 );
		cube = createBox   ( vec3 ( -7, -7, -7 ), vec3 ( 14, 14, 14 ) );
		eye  = vec3       ( 4, 4, 4 );
	}

	
	void redisplay ()
	{
		mat4	mv = getRotation ();
		mat3	nm = normalMatrix ( mv );

		glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		cubeMap.bind ( 0 );
		
		program2.bind ();
		program2.setUniformMatrix ( "mv",  mat4::identity () );

		cube -> render ();
		
		program2.unbind ();
		
		program.bind ();
		program.setUniformMatrix ( "mv",  mv );
		program.setUniformMatrix ( "nm",  nm );

		mesh -> render ();

		cubeMap.unbind ();	
		program.unbind ();
	}

	void reshape ( int w, int h )
	{
		GlutWindow::reshape ( w, h );
		
		glViewport ( 0, 0, (GLsizei)w, (GLsizei)h );
	   
		mat4 proj = perspective ( 60.0f, (float)w / (float)h, 0.5f, 20.0f ) * lookAt ( eye, vec3 :: zero, vec3 ( 0, 0, 1 ) );

		program.bind ();
		program.setUniformMatrix ( "proj", proj );
		program.setUniformVector ( "eye",  eye );
		program.unbind ();  

		program2.bind ();
		program2.setUniformMatrix ( "proj", proj );
		program2.setUniformVector ( "eye",  eye );
		program2.unbind ();  
	}
};

int main ( int argc, char * argv [] )
{
	GlutWindow::init( argc, argv );
	
	MyWindow	win;
	
	GlutWindow::run ();
	
	return 0;
}
