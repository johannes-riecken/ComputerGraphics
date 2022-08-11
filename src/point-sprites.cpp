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
#include	"randUtils.h"

#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>

#define	NUM_VERTICES 1000
#define	VERTEX_SIZE	 3*sizeof(float)

class	MeshWindow : public GlutRotateWindow
{
	Program      program;
	VertexArray  vao;
	VertexBuffer buf;
	vec3         vertices [NUM_VERTICES];
	Texture		 tex;
	vec3		 eye;

public:
	MeshWindow () : GlutRotateWindow ( 200, 200, 400, 400, "Point sprites" )
	{
		string	texName = "Textures/Fire.bmp";

		if ( !tex.load2D ( texName.c_str () ) )
		{
			printf ( "Error loading texture %s\n", texName.c_str () );
			exit   ( 1 );
		}

		if ( !program.loadProgram ( "sprite.glsl" ) )
		{
			printf ( "Error building program: %s\n", program.getLog ().c_str () );
			exit   ( 2 );
		}

		program.bind ();
		program.setTexture ( "image", 0 );

		eye  = vec3 ( 7, 7, 7 );

		for ( int i = 0; i < NUM_VERTICES; i++ )
			vertices [i] = vec3 ( randUniform (), randUniform (), randUniform () ) * 10.0 - vec3 ( 5.0 );

		vao.create  ();
		vao.bind    ();
		buf.create  ();
		buf.bind    ( GL_ARRAY_BUFFER );
		buf.setData ( NUM_VERTICES * VERTEX_SIZE, vertices, GL_STATIC_DRAW );

		program.setAttrPtr ( 0, 3, VERTEX_SIZE, (void *) 0 );

		buf.unbind     ();
		vao.unbind     ();
	}

	void redisplay ()
	{
		glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		mat4	mv = getRotation ();

		tex.bind ();
		program.bind ();
		program.setUniformMatrix ( "mv",  mv );
		vao.bind ();

		glEnable     ( GL_BLEND );
		glDisable    ( GL_DEPTH_TEST );
		glBlendFunc  ( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
		glDrawArrays ( GL_POINTS, 0, NUM_VERTICES );
		glDisable    ( GL_BLEND );
		vao.unbind ();

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
