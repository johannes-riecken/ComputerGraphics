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
#include	"Framebuffer.h"

#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>

class	MeshWindow : public GlutRotateWindow
{
	Program		program;
	BasicMesh * mesh;
	Texture		tex;
	BasicMesh * cube;
	Texture   * tex2;
	FrameBuffer fb;
	vec3		eye;

public:
	MeshWindow () : GlutRotateWindow ( 200, 200, 400, 400, "Render-to-texture" ), fb ( 512, 512, FrameBuffer::depth24 )
	{
		string	texName = "Textures/Fieldstone.dds";

		if ( !tex.load2D ( texName.c_str () ) )
		{
			printf ( "Error loading texture %s\n", texName.c_str () );
			exit   ( 1 );
		}

		if ( !program.loadProgram ( "rotate-8.glsl" ) )
		{
			printf ( "Error building program: %s\n", program.getLog ().c_str () );
			exit   ( 2 );
		}

		program.bind ();
		program.setTexture ( "image", 0 );

		fb.create ();
		fb.bind ();

		fb.attachColorTexture ( fb.createColorTexture () );
		fb.unbind ();

		if ( !fb.isOk () )
		{
			printf ( "Error with Framebuffer object\n" );
			exit   ( 3 );
		}

		mesh = createKnot ( 1, 4, 120, 30 );
		cube = createBox  ( vec3 ( -1, -1, -1 ), vec3 ( 2, 2, 2 ) );
		eye  = vec3 ( 2.5, 2.5, 2.5 );
	}

	void redisplay ()
	{
		displayBoxes ();

		glClearColor ( 0.5, 0.5, 0.5, 1 );
		glClear      ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		fb.getColorBuffer () -> bind ();
		program.bind ();
		program.setUniformMatrix ( "mv", getRotation () );

		cube -> render ();

		program.unbind ();
		fb.getColorBuffer () -> unbind ();
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

	void displayBoxes ()
	{
		fb.bind ();

		glViewport  ( 0, 0, fb.getWidth(), fb.getHeight() );
		glClearColor ( 0, 0, 0, 1 );
		glClear      ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		tex.bind  ( 0 );
		program.bind ();
		program.setUniformMatrix ( "mv", mat4::rotateX(getTime()) );
		program.setUniformVector ( "eye", vec3 ( 7, 7, 7 ) );

		mesh->render ();

		tex.unbind ();
		fb.unbind ();
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
