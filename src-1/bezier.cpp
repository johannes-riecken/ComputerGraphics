//
// Bezier cubic patch tessellation for OpenGL 4.1
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

#define	PATCH_SIZE		16							// 16 control points 
#define	VERTEX_SIZE		(3*sizeof(float))
#define	NUM_VERTICES	(PATCH_SIZE)

static const float vertices [PATCH_SIZE * 3] = 
{
   -2.0f, -2.0f, 0.0f, 			// P00*
   -1.0f, -2.0f, 1.0f, 			// P10
	1.0f, -1.0f, 2.0f,			// P20
	2.0f, -2.0f, 0.0f,			// P30*
   -2.0f, -1.0f, 1.0f,			// P01
   -1.0f, -1.0f, 1.0f,			// P11
	2.0f,  0.0f, 1.0f,			// P21
	2.0f, -1.0f, 2.0f,			// P31
   -3.0f,  0.0f, 1.0f,			// P02
   -1.0f, -1.5f, 1.0f,			// P12
	0.0f,  0.0f, 0.0f,			// P22
	1.0f,  1.0f, 1.0f,			// P32
   -2.0f,  2.0f, 0.0f,			// P03*
   -1.5f,  3.0f, 2.0f,			// P13
	1.0f,  3.0f, 2.0f,			// P23
	2.0f,  2.0f, 0.0f			// P33*
};

class	MeshWindow : public GlutRotateWindow
{
	int			 inner;
	int			 outer;
	vec3		 eye;
	VertexBuffer buf;
	VertexArray  vao;
	Program      program;
	
public:
	MeshWindow () : GlutRotateWindow ( 200, 200, 400, 400, 	"Bezier tesselation demo" )
	{
		if ( !program.loadProgram ( "bezier.glsl" ) )
		{
			printf ( "Error loading shader: %s\n", program.getLog ().c_str () );
			exit   ( 1 );
		}

		inner = 2;
		outer = 2;
		eye   = vec3 ( 3, 3, 4 );
		
		program.bind ();

		vao.create ();
		vao.bind   ();

		buf.create  ();
		buf.bind    ( GL_ARRAY_BUFFER );
		buf.setData ( NUM_VERTICES * VERTEX_SIZE, vertices, GL_STATIC_DRAW );
		program.setAttrPtr ( "position", 3, VERTEX_SIZE, (void *) 0 );
		vao.unbind ();
		program.unbind ();

		updateCaption ();

		printf ( "Use + and to change inner tessellation level, * and / to change outer level\n" );
	}
	
	void updateCaption ()
	{
		char str [128];

		sprintf ( str, "GL 4.1 Bezier tesselation - Inner Level: %2d, Outer Level: %2d", inner, outer );

		glutSetWindowTitle ( str );
	}

	void redisplay ()
	{
		glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		mat4	mv = getRotation ();
		
		program.bind ();
		program.setUniformMatrix ( "mv", mv );
		program.setUniformInt    ( "inner", inner );
		program.setUniformInt    ( "outer", outer );
		program.setPatchSize     ( PATCH_SIZE );

		vao.bind ();
		glPolygonMode ( GL_FRONT_AND_BACK, GL_LINE );
		glDrawArrays  ( GL_PATCHES, 0, NUM_VERTICES );
		vao.unbind ();
		program.unbind ();
	}

	void reshape ( int w, int h )
	{
		glViewport ( 0, 0, (GLsizei)w, (GLsizei)h );
	   
		mat4 proj = perspective ( 60.0f, (float)w / (float)h, 0.5f, 20.0f ) * lookAt ( eye, vec3 :: zero, vec3 ( 0, 0, 1 ) );

		program.bind ();
		program.setUniformMatrix ( "proj", proj );
		program.unbind ();  
	}


	void keyTyped ( unsigned char key, int modifiers, int x, int y )
	{
		if ( key == 27 || key == 'q' || key == 'Q' )	//	quit requested
			exit ( 0 );
			
		if ( key == '+' )
			inner++;
		else
		if ( key == '-' && inner > 1 )
			inner--;
			
		if ( key == '*' )
			outer++;
		else
		if ( key == '/' && outer > 1)
			outer--;
			
		if ( key == '+' || key == '-' || key == '/' || key == '*' )
			updateCaption ();

		glutPostRedisplay ();
	}
};

int main ( int argc, char * argv [] )
{
	GlutWindow::init( argc, argv, 4, 1 );
	
	MeshWindow	win;
	
	GlutWindow::run ();
	
	return 0;
}
