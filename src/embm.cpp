//
// Environment mapped bump mapping (EMBM) example 
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
	Program		program;
	BasicMesh * mesh;
	Texture		bumpMap;
	Texture		cubeMap;
	vec3		eye;
	
public:
	MyWindow () : GlutRotateWindow ( 200, 200, 400, 400, "EMBM" )
	{
		bumpMap.load2D      ( "Textures/FieldStone-DOT3.tga" );
		cubeMap.loadCubemap ( "Textures/Snow.dds" );
		
		if ( !program.loadProgram ( "embm.glsl" ) )
		{
			printf ( "Error loading shader: %s\n", program.getLog ().c_str () );
			
			exit ( 1 );
		}
		
		program.bind ();
		program.setTexture ( "bumpMap", 0 );
		program.setTexture ( "cubeMap", 1 );
		program.unbind ();

		mesh = createKnot ( 1, 3, 120, 30 );
		eye  = vec3       ( 4, 4, 4 );
	}
	
	void redisplay ()
	{
		mat4	mv = getRotation ();
		mat3	nm = normalMatrix ( mv );
		
		glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		program.bind ();
		program.setUniformMatrix ( "mv",  mv );
		program.setUniformMatrix ( "nm",  nm );

		cubeMap.bind ( 1 );
		bumpMap.bind ( 0 );
		
		mesh -> render ();

		cubeMap.unbind ();
		bumpMap.unbind ();
		
		program.unbind ();
	}

	void reshape ( int w, int h )
	{
		GlutWindow::reshape ( w, h );
		
		glViewport ( 0, 0, (GLsizei)w, (GLsizei)h );
	   
		mat4 proj = perspective ( 60.0f, (float)w / (float)h, 0.5f, 20.0f ) * lookAt ( eye, vec3 :: zero, vec3 ( 0, 0, 1 ) );

		program.bind ();
		program.setUniformMatrix ( "proj", proj );
		program.setUniformVector ( "eye",   eye );
		program.unbind ();  
	}

    void mouseWheel ( int wheel, int dir, int x, int y )
	{
		eye += 0.5 * vec3 ( dir, dir, dir );
		
					// since eye value has changed
		reshape ( getWidth(), getHeight() );
		
		glutPostRedisplay ();
	}
	
	
	void	idle ()
	{
		program.bind ();
		program.setUniformVector ( "eye",   eye   );
		program.unbind ();

		glutPostRedisplay ();
	}
};

int main ( int argc, char * argv [] )
{
	GlutWindow::init( argc, argv );
	
	MyWindow	win;
	
	GlutWindow::run ();
	
	return 0;
}
