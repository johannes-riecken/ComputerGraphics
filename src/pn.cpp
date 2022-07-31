//
// PN-triangle tessellation for OpenGL 4.1
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


#define	PATCH_SIZE		3							// 3 control points 
#define	VERTEX_SIZE		(6*sizeof(float))
#define	NUM_VERTICES	(PATCH_SIZE)

static const float vertices [] = 
{
	//  Coords                 Normal
	3.0,  0.0,  0.0,	   -3.0, -1.0, -1.0,
	0.0,  3.0,  0.0,	   -1.0, -3.0, -1.0,
	0.0,  0.0,  3.0,	    1.0,  0.0,  7.0
};

class	MyWindow : public GlutRotateWindow
{
	VertexArray		vao;
	VertexBuffer	buf;
	Program			program;
	vec3			eye;
	int				inner;
	int				outer;
	
public:
	MyWindow () : GlutRotateWindow ( 200, 200, 400, 400, "Cellular noisy shader" )
	{
		if ( !program.loadProgram ( "pn.glsl" ) )
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

		program.setAttrPtr ( "position", 3, VERTEX_SIZE, (void *) 0 );
		program.setAttrPtr ( "normal",   3, VERTEX_SIZE, (void *)(3 * sizeof ( float ) ) );

		buf.unbind     ();
		vao.unbind     ();
		program.unbind ();

		updateCaption ();

		eye   = vec3 ( 3, 3, 4 );
		inner = 2;
		outer = 2;
		
		printf ( "Use + and - keys to change inner tessellation level and * and / keys to change outer level.\n" );
	}

	void redisplay ()
	{
		mat4	mv = getRotation ();
		mat3	nm = normalMatrix ( mv );
	
		glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		program.bind ();
		program.setUniformMatrix ( "mv",  mv );
		program.setUniformMatrix ( "nm",  nm );
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
	
    void	keyTyped ( unsigned char key, int modifiers, int x, int y )
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

private:
	void updateCaption ()
	{
		char str [128];

		sprintf ( str, "PN-triangle - Inner Level: %2d, Outer Level: %2d", inner, outer );

		glutSetWindowTitle ( str );
	}
};

int main ( int argc, char * argv [] )
{
	GlutWindow::init( argc, argv );
	
	MyWindow	win;
	
	GlutWindow::run ();
	
	return 0;
}
