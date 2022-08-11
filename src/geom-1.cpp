//
// Geometry shader example
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

#define	NUM_VERTICES	3
#define	VERTEX_SIZE		(4*sizeof(float))

static float vertices [NUM_VERTICES * 4] =
{
	0, -1, 0, 1,
	1, 0, 0, 2,
	0, 1, 1, 3
};

class	MeshWindow : public GlutRotateWindow
{
	Program      program;
	VertexArray  vao;
	VertexBuffer buf;
	vec3		 eye;

public:
	MeshWindow () : GlutRotateWindow ( 200, 200, 400, 400, "Billboards" )
	{
		if ( !program.loadProgram ( "geom-1.glsl" ) )
		{
			printf ( "Error loading shader: %s\n", program.getLog ().c_str () );
			exit ( 1 );
		}

		program.bind ();
		vao.create ();
		vao.bind   ();

		buf.create  ();
		buf.bind    ( GL_ARRAY_BUFFER );
		buf.setData ( NUM_VERTICES * VERTEX_SIZE, vertices, GL_STATIC_DRAW );

		program.setAttrPtr ( "posSize", 4, VERTEX_SIZE, (void *) 0 );

		buf.unbind  ();
		vao.unbind  ();
		program.unbind ();

		eye = vec3 ( 5, 5, 5 );
	}

	void redisplay ()
	{
		glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		mat4	mv = getRotation ();
		vec3	viewDir = vec3 ( 0, 0, 1 );
		vec3	sideDir = vec3 ( 0, 1, 0 );
		vec3	upDir   = vec3 ( 1, 0, 0 );

		program.bind ();
		program.setUniformMatrix ( "mv",    mv );
		program.setUniformVector ( "up",    upDir );
		program.setUniformVector ( "right", sideDir );

		vao.bind ();
		glDrawArrays ( GL_POINTS, 0, NUM_VERTICES );
		vao.unbind ();

		program.unbind ();
	}

	void reshape ( int w, int h )
	{
		glViewport ( 0, 0, (GLsizei)w, (GLsizei)h );

		mat4 proj = perspective ( 60.0f, (float)w / (float)h, 0.5f, 15.0f ) * lookAt ( eye, vec3 :: zero, vec3 ( 0, 0, 1 ) );

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
