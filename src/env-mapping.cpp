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
	MeshWindow () : GlutRotateWindow ( 200, 200, 400, 400, "Reflecting torus" )
	{
		string	texName = "Textures/Snow.dds";

		if ( !tex.loadCubemap ( texName.c_str () ) )
		{
			printf ( "Error loading texture %s\n", texName.c_str () );
			exit   ( 1 );
		}

		if ( !program.loadProgram ( "rotate-6.glsl" ) )
		{
			printf ( "Error building program: %s\n", program.getLog ().c_str () );
			exit   ( 2 );
		}

		program.bind ();
		program.setTexture ( "image", 0 );

		mesh = createTorus ( 2, 4, 30, 30  );
		eye  = vec3 ( 7, 7, 7 );
	}

	void redisplay ()
	{
		glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		mat4	mv = getRotation ();
		mat3	nm = normalMatrix ( mv );

		tex.bind ();
		program.bind ();
		program.setUniformMatrix ( "mv",  mv );
		program.setUniformMatrix ( "nm",  nm );

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

};

int main ( int argc, char * argv [] )
{
	GlutWindow::init( argc, argv );

	MeshWindow	win;

	GlutWindow::run ();

	return 0;
}
