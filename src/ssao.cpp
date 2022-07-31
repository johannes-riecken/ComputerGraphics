//
// SSAO example 
//
// Author: Alexey V. Boreskov <steps3d@gmail.com>, <steps3d@narod.ru>
//

#include	"GlutRotateWindow.h"
#include	"Program.h"
#include	"glUtilities.h"
#include	"BasicMesh.h"
#include	"Texture.h"
#include	"Framebuffer.h"
#include	"Camera.h"
#include	"randUtils.h"
#include	"ScreenQuad.h"

#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<vector>

using namespace std;

vec3		eye    ( -0.5, 0.8, 0.5 );
Camera		camera ( eye, 0, 0, 0 );	// camera to be used
ScreenQuad	quad   ( 640, 480, 640, 480 );
FrameBuffer fb     ( 640, 480, FrameBuffer::depth32 );
FrameBuffer fb2    ( 640, 480, FrameBuffer::depth32 );

class	MyWindow : public GlutRotateWindow
{
	Program		program, program2;
	Texture     decalMap;
	Texture		depthMap;
	Texture		rotateMap;

	vector<BasicMesh *> boxes;

	float	yaw;
	float	pitch;
	float	roll;
	float	angle;

	//int     mouseOldX = 0;
	//int     mouseOldY = 0;
	//vec3	rot   ( 0.0f );

public:
	MyWindow () : GlutRotateWindow ( 200, 200, 640, 480, "Screen-Space Ambient Occlusion" )
	{
		decalMap.load2D  ( "Textures/oak.jpg" );
		rotateMap.load2D ( "Textures/rotate.bmp" );
		depthMap.createDepthRect ( 640, 480 );
		
		fb.create ();
		fb.bind   ();
		fb.attachColorTexture ( fb.createColorTexture () );
		
		if ( !fb.isOk () )
			printf ( "Error with fb\n" );
			
		fb.unbind ();

		if ( !program.loadProgram ( "decal.glsl" ) )
		{
			printf ( "Error loading shader: %s\n", program.getLog ().c_str () );
			
			exit ( 1 );
		}
		
		program.bind ();
		program.setTexture ( "image", 0 );
		program.unbind ();
		
		if ( !program2.loadProgram ( "ssao.glsl" ) )
		{
			printf ( "Error loading shader: %s\n", program2.getLog ().c_str () );
			
			exit ( 1 );
		}
		
		program2.bind            ();
		program2.setTexture      ( "depthMap",  0 );
		program2.setTexture      ( "rotateMap", 1 );
		program2.unbind          ();

		camera.setRightHanded ( false );

		buildBoxes ( 200 );
		
		yaw   = 0;
		pitch = 0;
		roll  = 0;
		angle = 0;

		//int     mouseOldX = 0;
		//int     mouseOldY = 0;
		//vec3	rot   ( 0.0f );		
	}

	void redisplay ()
	{
													// render for FP FBO
		glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		mat4 proj = camera.getProjection ();
		mat4 mv   = camera.getModelview ();
		
		program.bind ();
		decalMap.bind ();

		program.setUniformMatrix ( "proj", proj );
		program.setUniformMatrix ( "mv",   mv   );
		
		displayBoxes ();
		
		decalMap.unbind ();
		program.unbind ();

		glFinish ();
		
		rotateMap.bind ( 1 );
		depthMap.bind  ( 0 );
		
		glCopyTexImage2D ( depthMap.getTarget (), 0, GL_DEPTH_COMPONENT, 0, 0, depthMap.getWidth (), depthMap.getHeight (), 0 );

		glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		program2.bind ();
		program2.setUniformMatrix ( "proj", proj );
		program2.setUniformMatrix ( "mv",   mv   );

		quad.render ();
		
		program2.unbind ();
		
		depthMap.unbind ();
		rotateMap.unbind ();
	}
	
	void reshape ( int w, int h )
	{
		GlutWindow::reshape ( w, h );
		
		camera.setViewSize ( w, h, 60 );
	}
	
    void	keyTyped ( unsigned char key, int modifiers, int x, int y )
	{
		if ( key == 27 || key == 'q' || key == 'Q' )        // quit requested
			exit ( 0 );
		else
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

		glutPostRedisplay ();
	}

	void    specialKey ( int key, int x, int y )
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

	void    idle ()
	{
		angle = 2 * getTime ();

		glutPostRedisplay ();
	}

private:	
	void	buildBoxes ( int numBoxes )
	{
		vec3  org ( -5, -1, -5 );
		vec3  sz  ( 1, 1, 1 );
		float w1 = 10;
		float w2 = 10;
		
		boxes.push_back ( createBox ( org, vec3 ( 10, 3, 10 ) ) );

		for ( int i = 0; i < numBoxes; i++ )
		{
			vec3  pos ( randUniform ( org.x, org.x + w1 ), randUniform ( org.y, org.y + 0.1 ), randUniform ( org.z, org.z + w2 ) );
			vec3  r   ( vec3 :: getRandomVector () );
			float a   ( randUniform ( 0, 180.0 ) );
			mat4  m = mat4::translate ( pos ) * mat4::rotate ( r, a );
			
			boxes.push_back ( createBox ( vec3 :: zero, sz*randUniform ( 0.5, 1.0 ), &m ) );
		}
	}

	void displayBoxes ()
	{
		for ( vector<BasicMesh *> :: iterator it = boxes.begin (); it != boxes.end (); ++it )
			(*it) -> render ();
	}
};

int main ( int argc, char * argv [] )
{
	GlutWindow::init( argc, argv );
	
	MyWindow	win;
	
	GlutWindow::run ();
	
	return 0;
}
