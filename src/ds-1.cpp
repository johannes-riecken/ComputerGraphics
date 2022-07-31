//
// Deferred shading example 
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

vec3		eye    ( -0.5, 0.5, 5 );
Camera		camera ( eye, 0, 0, 0 );	// camera to be used
FrameBuffer fb     ( 640, 480, FrameBuffer::depth24 );
ScreenQuad	quad   ( 640, 480, 1, 1 );

class	MyWindow : public GlutWindow
{
	Program		program, program2;
	Texture 	decalMap;
	Texture 	stoneMap;
	Texture 	knotMap;
	BasicMesh * box1;
	BasicMesh * box2;
	BasicMesh * box3;
	BasicMesh * box4;
	BasicMesh * box5;
	BasicMesh * knot;
	float		angle;
	vec3		light;
	float		yaw, pitch, roll;

public:
	MyWindow () : GlutWindow ( 200, 200, 640, 480, "Deferred Shading" ) 
	{
		fb.create ();
		fb.bind   ();
		fb.attachColorTexture ( fb.createColorTexture ( GL_RGBA, GL_RGBA16F ), 0 );
		fb.attachColorTexture ( fb.createColorTexture ( GL_RGBA, GL_RGBA16F ), 1 );
		
		if ( !fb.isOk () )
			printf ( "Error with fb\n" );
			
		fb.unbind ();
		
		if ( !program.loadProgram ( "ds-1-1.glsl" ) )
		{
			printf ( "Error loading shader: %s\n", program.getLog ().c_str () );
			
			exit ( 1 );
		}
		
		if ( !program2.loadProgram ( "ds-1-2.glsl" ) )
		{
			printf ( "Error loading shader: %s\n", program2.getLog ().c_str () );
			
			exit ( 1 );
		}
		
		program2.bind ();
		program2.setTexture ( "posMap", 0 );
		program2.setTexture ( "normalMap", 1 );
		program2.unbind ();
		
		box1 = createBox  ( vec3 ( -6, -0.1, -6 ), vec3 ( 12, 3, 12 ), NULL, true );
		box2 = createBox  ( vec3 ( -1.5, 0, -0.5 ),  vec3 ( 1,  2,  2 ) );
		box3 = createBox  ( vec3 ( 1.5, 0, -0.5 ),  vec3 ( 1,  1,  1 ) );
		box4 = createBox  ( vec3 ( -4, 0, -0.5 ),  vec3 ( 1,  1,  1 ) );
		box5 = createBox  ( vec3 ( -4, 0, -4 ),  vec3 ( 1,  1,  1 ) ); 
		knot = createKnot ( 1, 4, 120, 30 );

		decalMap.load2D ( "Textures/oak.jpg" );
		stoneMap.load2D ( "Textures/block.jpg" );
		knotMap.load2D  ( "Textures/Oxidated.jpg" );
		
		camera.setRightHanded ( false );
		
		angle = 0;
		yaw   = 0;
		pitch = 0;
		roll  = 0;
		light = vec3 ( 0, 1, 0 );
	}

	void redisplay ()
	{
		fb.bind ();
		fb.drawBuffers ( 2 );
		
		glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		mat4 mv = camera.getModelview ();

		program.bind ();
		program.setUniformMatrix ( "proj",  camera.getProjection () );
		program.setUniformMatrix ( "mv",    mv   );
		program.setUniformMatrix ( "nm",    normalMatrix ( mv ) );
		
		displayBoxes ();
		
		program.unbind ();
		glFinish ();
		
		fb.unbind ();

		glClear ( GL_COLOR_BUFFER_BIT );
		
		fb.getColorBuffer ( 0 ) -> bind ( 0 );
		fb.getColorBuffer ( 1 ) -> bind ( 1 );
		
		glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		fb.getColorBuffer () -> bind ();
		
		program2.bind ();
		program2.setUniformVector ( "light", mv * vec4 ( light, 1 ) );
		
		quad.render ();

		program2.unbind ();
		
		fb.getColorBuffer ( 0 ) -> unbind ();
		fb.getColorBuffer ( 1 ) -> unbind ();
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
	
private:
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
		program.setUniformMatrix ( "nm",  normalMatrix ( camera.getModelview () ) );

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
