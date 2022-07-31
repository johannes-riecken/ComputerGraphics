//
// Example of using freeglut and glew to get OpenGL 3.3/4.1
//
// Author: Alex V. Boreskoff <steps3d@gmail.com>, <steps3d@narod.ru>
//

#include	"GlutWindow.h"
#include	"Program.h"
#include	"glUtilities.h"
#include	"mat4.h"
#include	"VertexArray.h"
#include	"VertexBuffer.h"

#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>

#define	NUM_VERTICES	3
#define	VERTEX_SIZE		(6*sizeof(float))

static const float vertices [] = 
{       /* Coords: */  /* Color: */ 
	-1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
	 0.0f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
	 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f
};

class	MyWindow : public GlutWindow
{
	int				mouseOldX;
	int    			mouseOldY;
	vec3			rot;
	Program			program;
	VertexArray		vao;
	VertexBuffer	buf;
	
public:
	MyWindow () : GlutWindow ( 200, 200, 400, 400, "Rotating triangle" )
	{
		mouseOldX = 0;
		mouseOldY = 0;
		rot       = vec3 ( 0.0f );

		if ( !program.loadProgram ( "rotate.glsl" ) )
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
		program.setAttrPtr ( "color",    3, VERTEX_SIZE, (void *)(3 * sizeof ( float ) ) );

		buf.unbind     ();
		vao.unbind     ();
	}

	void redisplay ()
	{
		glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		mat4	mv = mat4 :: rotateZ ( toRadians ( -rot.z ) ) * mat4 :: rotateY ( toRadians ( rot.y ) ) * mat4 :: rotateX ( toRadians ( rot.x ) );
		
		program.bind ();
		program.setUniformMatrix ( "mv", mv );

		vao.bind ();
		glDrawArrays ( GL_TRIANGLES, 0, NUM_VERTICES );
		vao.unbind ();
		program.unbind ();
	}

	void reshape ( int w, int h )
	{
		vec3	eye ( 3, 3, 3 );
		
		GlutWindow::reshape ( w, h );
		
		glViewport ( 0, 0, (GLsizei)w, (GLsizei)h );
	   
		mat4 proj = perspective ( 60.0f, (float)w / (float)h, 0.5f, 15.0f ) * lookAt ( eye, vec3 :: zero, vec3 ( 0, 0, 1 ) );

		program.bind ();
		program.setUniformMatrix ( "proj", proj );
		program.unbind ();  
	}

    virtual void	mouseMotion ( int x, int y )
	{
		rot.x -= ((mouseOldY - y) * 180.0f) / 200.0f;
		rot.z -= ((mouseOldX - x) * 180.0f) / 200.0f;
		rot.y  = 0;

		if ( rot.z > 360 )
			rot.z -= 360;

		if ( rot.z < -360 )
			rot.z += 360;

		if ( rot.y > 360 )
			rot.y -= 360;

		if ( rot.y < -360 )
			rot.y += 360;

		mouseOldX = x;
		mouseOldY = y;

		glutPostRedisplay ();
	}
	
	virtual void	mouseClick ( int button, int state, int modifiers, int x, int y )
	{
		if ( state == GLUT_DOWN )
		{
			mouseOldX = x;
			mouseOldY = y;
		}
	}
};

int main ( int argc, char * argv [] )
{
	GlutWindow::init( argc, argv );
	
	MyWindow	win;
	
	GlutWindow::run ();
	
	return 0;
}
