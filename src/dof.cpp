//
// Depth of field example
//
// Author: Alexey V. Boreskov <steps3d@gmail.com>, <steps3d@narod.ru>
//

#include	"GlutWindow.h"
#include	"Program.h"
#include	"glUtilities.h"
#include	"BasicMesh.h"
#include	"Texture.h"
#include	"Camera.h"
#include	"Framebuffer.h"
#include	"ScreenQuad.h"

#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>

vec3		eye   ( -0.5, 0.5, 5 );
Camera		camera( eye, 0, 0, 0 );
FrameBuffer fb    ( 640, 480, FrameBuffer::depth24 );
FrameBuffer fb2   ( 640/4, 480/4 );
ScreenQuad	quad  ( 640, 480, 1, 1 );
ScreenQuad	quad2 ( 640/4, 480/4, 1, 1 );

class	MyWindow : public GlutWindow
{
	Program		program, program2, program3;
	Texture 	decalMap;
	Texture 	stoneMap;
	Texture 	knotMap;
	BasicMesh * box1;
	BasicMesh * box2;
	BasicMesh * box3;
	BasicMesh * box4;
	BasicMesh * box5;
	BasicMesh * knot;
	float		focalDistance;
	float		focalRange;
	float		yaw, pitch, roll;
	float		angle;

public:
	MyWindow () : GlutWindow ( 200, 200, 640, 480, "Depth of field" )
	{
		fb.create ();
		fb.bind   ();
		fb.attachColorTexture ( fb.createColorTexture () );

		if ( !fb.isOk () )
			printf ( "Error with fb\n" );

		fb.unbind ();

		fb2.create ();
		fb2.bind   ();
		fb2.attachColorTexture ( fb2.createColorTexture () );

		if ( !fb2.isOk () )
			printf ( "Error with fb2\n" );

		fb2.unbind ();

		if ( !program.loadProgram ( "dof-1.glsl" ) )
		{
			printf ( "Error loading shader: %s\n", program.getLog ().c_str () );

			exit ( 1 );
		}

		program.bind ();
		program.setTexture ( "imageMap", 0 );
		program.unbind ();

		if ( !program2.loadProgram ( "dof-2.glsl" ) )
		{
			printf ( "Error loading shader: %s\n", program2.getLog ().c_str () );

			exit ( 1 );
		}

		program2.bind ();
		program2.setTexture ( "inputMap", 0 );
		program2.setTexture ( "inputLowMap", 1 );
		program2.unbind ();

		if ( !program3.loadProgram ( "dof-downsample.glsl" ) )
		{
			printf ( "Error loading shader: %s\n", program3.getLog ().c_str () );

			exit ( 1 );;
		}

		program3.bind ();
		program3.setTexture ( "inputMap", 0 );
		program3.unbind ();

		box1 = createBox  ( vec3 ( -6, -0.1, -6 ), vec3 ( 12, 3, 12 ) );  // stoneMap, false
		box2 = createBox  ( vec3 ( -1.5, 0, -0.5 ),  vec3 ( 1,  2,  2 ) );  // decalMap, false );
		box3 = createBox  ( vec3 ( 1.5, 0, -0.5 ),  vec3 ( 1,  1,  1 ) );  // decalMap, false );
		box4 = createBox  ( vec3 ( -4, 0, -0.5 ),  vec3 ( 1,  1,  1 ) );  // decalMap, false );
		box5 = createBox  ( vec3 ( -4, 0, -4 ),  vec3 ( 1,  1,  1 ) );  // decalMap, false );
		knot = createKnot ( 1, 4, 120, 30 );

		decalMap.load2D ( "Textures/oak.jpg" );
		stoneMap.load2D ( "Textures/block.jpg" );
		knotMap.load2D  ( "Textures/Oxidated.jpg" );

		camera.setRightHanded ( false );

		focalDistance = 4.5;
		focalRange    = 20;
		yaw           = 0;
		pitch         = 0;
		roll          = 0;
		angle         = 0;

		printf ( "Depth of Field demo.\n\tUse + and - to change focal distance.\n\tUse * and / to change focal range.\n\tUse mouse and wsad to control camera.\n" );
	}

	void redisplay ()
	{
		fb.bind ();
		glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		program.bind ();
		program.setUniformMatrix ( "proj",  camera.getProjection () );
		program.setUniformMatrix ( "mv",    camera.getModelview  () );
		program.setUniformFloat ( "focalDistance", focalDistance );
		program.setUniformFloat ( "focalRange",    focalRange    );

		displayBoxes ();

		program.unbind ();
		glFinish ();

		fb.unbind ();

		glClear ( GL_COLOR_BUFFER_BIT );

		fb.getColorBuffer () -> bind ();

		fb2.bind ();
		program3.bind ();
		quad2.render ();
		program3.unbind ();
		fb2.unbind ();

		glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		fb.getColorBuffer () -> bind ();
		fb2.getColorBuffer () -> bind ( 1 );

		program2.bind ();

		quad.render ();

		program2.unbind ();

		fb2.getColorBuffer () -> unbind ();
		fb.getColorBuffer () -> unbind ();
	}

	void reshape ( int w, int h )
	{
		GlutWindow::reshape ( w, h );

		camera.setViewSize ( w, h, 60 );
	}

    void	keyTyped ( unsigned char key, int modifiers, int x, int y )
	{
		if ( key == 27 || key == 'q' || key == 'Q' )	//	quit requested
			exit ( 0 );

		if ( key == 'w' || key == 'W' )
			camera.moveBy ( camera.getViewDir () * 0.2 );
		else
		if ( key == 'x' || key == 'X' )
			camera.moveBy ( -camera.getViewDir () * 0.2 );
		else
		if ( key == 'a' || key == 'A' )
			camera.moveBy ( -camera.getSideDir () * 0.2 );
		else
		if ( key == 'd' || key == 'D' )
			camera.moveBy ( camera.getSideDir () * 0.2 );
		else
		if ( key == '+' )
			focalDistance += 0.1;
		else
		if ( key == '-' )
			focalDistance -= 0.1;
		else
		if ( key == '*' )
			focalRange += 0.3;
		else
		if ( key == '/' )
			focalRange -= 0.3;
	}

     void	specialKey ( int key, int modifier, int x, int y )
	{
		if ( key == GLUT_KEY_UP )
			pitch += M_PI / 90;
		else
		if ( key == GLUT_KEY_DOWN )
			pitch -= M_PI / 90;
		else
		if ( key == GLUT_KEY_RIGHT )
			yaw -= M_PI / 90;
		else
		if ( key == GLUT_KEY_LEFT )
			yaw += M_PI / 90;

		camera.setEulerAngles ( yaw, pitch, roll );

		glutPostRedisplay ();
	}

	void	mousePassiveMotion ( int x, int y )
	{
		static	int	lastX = -1;
		static	int	lastY = -1;

		if ( lastX == -1 )				// not initialized
		{
			lastX = x;
			lastY = y;
		}

		yaw   -= (x - lastX) * 0.01;
		pitch -= (y - lastY) * 0.01;
		lastX  = x;
		lastY  = y;

		camera.setEulerAngles ( yaw, pitch, roll );

		glutPostRedisplay ();
	}

    void mouseWheel ( int wheel, int dir, int x, int y )
	{
		eye += 0.5 * vec3 ( dir, dir, dir );

		reshape ( glutGet ( GLUT_WINDOW_WIDTH ), glutGet ( GLUT_WINDOW_HEIGHT ) );

		glutPostRedisplay ();
	}

	void	idle ()
	{
		angle = 4 * getTime ();

		glutPostRedisplay ();
	}

protected:
	void displayBoxes ()
	{
		decalMap.bind ();
		box1 -> render ();
		decalMap.unbind ();
		stoneMap.bind ();
		box2 -> render ();
		box3 -> render ();
		box4 -> render ();
		box5 -> render ();
		stoneMap.unbind ();

		mat4 m = camera.getModelview () * mat4::translate ( vec3 ( 2, 1, 1 ) ) * mat4::rotateX ( angle * 0.3 ) * mat4::rotateY ( angle * 0.07 ) * mat4::scale( vec3(0.3));

		program.setUniformMatrix ( "mv",  m );

		knotMap.bind ();

		knot -> render ();

		knotMap.unbind ();
	}

};

int main ( int argc, char * argv [] )
{
	GlutWindow::init( argc, argv );

	MyWindow	win;

	GlutWindow::run ();

	return 0;
}
