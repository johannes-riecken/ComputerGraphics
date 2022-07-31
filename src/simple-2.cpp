//
// Example of using freeglut and glew to get OpenGL 3.3/4.1
//
// Author: Alex V. Boreskoff <steps3d@gmail.com>, <steps3d@narod.ru>
//

#include	"GlutWindow.h"
#include	"Program.h"
#include	"VertexArray.h"
#include	"VertexBuffer.h"
#include	"glUtilities.h"
#include	"mat4.h"

#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>

#define	NUM_VERTICES	3
#define	VERTEX_SIZE		(3*sizeof(float))

static const float vertices [] = 
{
	-1.0f, -1.0f, 0.0f,
	 0.0f,  1.0f, 0.0f,
	 1.0f, -1.0f, 0.0f
};

class	MyWindow : public GlutWindow
{
	Program			program;
	VertexArray		vao;
	VertexBuffer	buf;
	
public:
	MyWindow () : GlutWindow  ( 200, 200, 400, 400, "First GL 3.3 demo" )
	{
		if ( !program.loadProgram ( "simple.glsl" ) )
		{
			printf ( "Error building program: %s\n", program.getLog ().c_str () );
			
			exit ( 2 );
		}
		
		program.bind ();
		vao.create  ();
		vao.bind    ();
		buf.create  ();
		buf.bind    ( GL_ARRAY_BUFFER );
		buf.setData ( NUM_VERTICES * VERTEX_SIZE, vertices, GL_STATIC_DRAW );

		program.setAttrPtr ( "position", 3, VERTEX_SIZE, (void *) 0 );

		buf.unbind     ();
		vao.unbind     ();
	}

	virtual	void	redisplay ()
	{
		glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		vao.bind ();	
		glDrawArrays ( GL_TRIANGLES, 0, NUM_VERTICES );		
		vao.unbind ();
	}
	
	virtual	void	reshape ( int w, int h )
	{
		GlutWindow::reshape ( w, h );
		
		glViewport ( 0, 0, (GLsizei)w, (GLsizei)h );
	}
	
    virtual void	keyTyped ( unsigned char key, int modifiers, int x, int y )
	{
		if ( key == 27 || key == 'q' || key == 'Q' )	//	quit requested
			exit ( 0 );
	}
};

int main ( int argc, char * argv [] )
{
								// initialize glut
	GlutWindow::init( argc, argv );
	
	MyWindow	win;
	
	GlutWindow::run ();
	
	return 0;
}
