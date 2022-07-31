//
// Simple unlit and untextures torus for OpenGL 3.3/4.1
//
// Author: Alex V. Boreskoff <steps3d@gmail.com>, <steps3d@narod.ru>
//

#include	"GlutRotateWindow.h"
#include	"Program.h"
#include	"glUtilities.h"
#include	"BasicMesh.h"
#include	"Texture.h"

#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>

class	MeshWindow : public GlutRotateWindow
{
	Program		program;
	BasicMesh * mesh;
	vec3		eye;
	vec3        light;
	
public:
	MeshWindow () : GlutRotateWindow ( 200, 200, 400, 400, "Simple torus" )
	{
		if ( !program.loadProgram ( "unlit.glsl" ) )
		{
			printf ( "Error building program: %s\n", program.getLog ().c_str () );
			exit   ( 2 );
		}
		
		program.bind ();
		
		mesh  = createKnot  ( 1, 4, 120, 30 );
		eye   = vec3 ( 4, 4, 4 );
		light = vec3 ( 5, 5, 5 );
	}

	void redisplay ()
	{
		mat4	mv = getRotation ();
		mat3	nm = normalMatrix ( mv );
		
		glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		program.bind ();
		program.setUniformMatrix ( "mv", mv );

		mesh -> render ();
		
		program.unbind ();
	}

	void reshape ( int w, int h )
	{
		GlutWindow::reshape ( w, h );
		
		glViewport ( 0, 0, (GLsizei)w, (GLsizei)h );
	   
		mat4 proj = perspective ( 60.0f, (float)w / (float)h, 0.5f, 20.0f ) * lookAt ( eye, vec3 :: zero, vec3 ( 0, 0, 1 ) );

		program.bind ();
		program.setUniformMatrix ( "proj", proj );
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
		float	angle = 4 * getTime ();
		
		glutPostRedisplay ();
	}
};

int main ( int argc, char * argv [] )
{
	GlutWindow::init( argc, argv );
	
	MeshWindow	win;
	
	GlutWindow::run ();
	
	return 0;
}
