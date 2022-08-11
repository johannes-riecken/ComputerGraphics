//
// Example of using freeglut and glew to get OpenGL 3.3/4.1
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
	Texture		tex;
	vec3		eye;

public:
	MeshWindow () : GlutRotateWindow ( 200, 200, 400, 400, "Textured 3D mesh" )
	{
		string	texName = "Textures/noise-3D.dds";

		if ( !tex.load3D ( texName.c_str () ) )
		{
			printf ( "Error loading texture %s\n", texName.c_str () );
			exit   ( 1 );
		}

		if ( !program.loadProgram ( "rotate-4.glsl" ) )
		{
			printf ( "Error building program: %s\n", program.getLog ().c_str () );
			exit   ( 2 );
		}

		program.bind ();
		program.setTexture ( "image", 0 );

		mesh = createKnot  ( 1, 4, 120, 30 );
		eye  = vec3 ( 5, 5, 5 );
	}

	void redisplay ()
	{
		glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		tex.bind ();
		program.bind ();
		program.setUniformMatrix ( "mv", getRotation () );

		mesh -> render ();

		program.unbind ();
		tex.unbind ();
	}

	void reshape ( int w, int h )
	{
		GlutWindow::reshape ( w, h );

		glViewport ( 0, 0, (GLsizei)w, (GLsizei)h );

		mat4 proj = perspective ( 60.0f, (float)w / (float)h, 0.5f, 20.0f ) * lookAt ( eye, vec3 :: zero, vec3 ( 0, 0, 1 ) );

		program.bind ();
		program.setUniformMatrix ( "proj", proj );
//		program.setUniformVector ( "eye",  eye );
		program.unbind ();
	}

    void mouseWheel ( int wheel, int dir, int x, int y )
	{
		eye += 0.5 * vec3 ( dir, dir, dir );

					// since eye value has changed
		reshape ( getWidth(), getHeight() );

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
