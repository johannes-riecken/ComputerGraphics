//
// Transform feedback in OpenGL 3.3 example 
//
// Author: Alexey V. Boreskov <steps3d@gmail.com>, <steps3d@narod.ru>
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

#define	NUM_PARTICLES	5000

class	MeshWindow : public GlutRotateWindow
{
	float 		 lastTime;
	vec3         eye;
	int          ind;
	vec3         p [NUM_PARTICLES];
	vec3         v [NUM_PARTICLES];
	Program      program;
	VertexArray  vao [2];
	VertexBuffer posBuf [2], velBuf [2];

public:
	MeshWindow () : GlutRotateWindow ( 200, 200, 400, 400, "Transform feedback" )
	{
		if ( !program.loadProgram ( "tf3.glsl" ) )
		{
			printf ( "Error loading shader: %s\n", program.getLog ().c_str () );		
			exit   ( 1 );
		}

		lastTime = 0;
		eye      = vec3 ( 3, 3, 3 );
		ind      = 0;
		
		initParticles ();

		program.bind ();
		program.transformFeedbacksVars ( "newPos;newVel", GL_SEPARATE_ATTRIBS );
		program.relink ();
		program.unbind ();

		for ( int i = 0; i < 2; i++ )
		{
			vao [i].create ();
			vao [i].bind   ();

			posBuf [i].create     ();
			posBuf [i].bind       ( GL_ARRAY_BUFFER );
			posBuf [i].setData    ( NUM_PARTICLES * sizeof ( vec3 ), p, GL_STATIC_DRAW );
			posBuf [i].setAttrPtr ( 0, 3, sizeof ( vec3 ), (void *) 0 );

			velBuf [i].create     ();
			velBuf [i].bind       ( GL_ARRAY_BUFFER );
			velBuf [i].setData    ( NUM_PARTICLES * sizeof ( vec3 ), v, GL_STATIC_DRAW );
			velBuf [i].setAttrPtr ( 1, 3, sizeof ( vec3 ), (void *) 0 );
		
			vao [i].unbind     ();
		}
	}
	
	void	initParticles ()
	{
		for ( int i = 0; i < NUM_PARTICLES; i++ )
		{
			p [i] = vec3 ( 0, 0, 0 );
			v [i] = vec3 ( randUniform ( -0.1, 0.1 ), randUniform ( -0.1, 0.1 ), randUniform ( -0.1, 0.1 ) );
		}
	}

	void redisplay ()
	{
		float t = getTime ();
		mat4  mv = getRotation ();

		glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		program.bind ();
		program.setUniformMatrix ( "mv",  mv );
		program.setUniformFloat  ( "dt",  t - lastTime );
		
		posBuf [ind^1].bindBase ( GL_TRANSFORM_FEEDBACK_BUFFER, 0 );
		velBuf [ind^1].bindBase ( GL_TRANSFORM_FEEDBACK_BUFFER, 1 );

		glBeginTransformFeedback ( GL_POINTS );

		vao [ind].bind ();

		glDrawArrays ( GL_POINTS, 0, NUM_PARTICLES );
		
		vao [ind].unbind ();
		
		glEndTransformFeedback ();

		program.unbind ();

		lastTime = t;
	}

	void reshape ( int w, int h )
	{
		glViewport ( 0, 0, (GLsizei)w, (GLsizei)h );
	   
		mat4 proj = perspective ( 60.0f, (float)w / (float)h, 0.5f, 20.0f ) * lookAt ( eye, vec3 :: zero, vec3 ( 0, 1, 0 ) );

		program.bind ();
		program.setUniformMatrix ( "proj",    proj );
		program.setUniformVector ( "boxSize", vec3 ( 2 ) );
		program.unbind ();  
	}

	void idle ()
	{
		ind ^= 1;
		
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
