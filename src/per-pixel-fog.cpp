//
// Per-pixel fog example 
//
// Author: Alexey V. Boreskov <steps3d@gmail.com>, <steps3d@narod.ru>
//

#include	"GlutRotateWindow.h"
#include	"Program.h"
#include	"glUtilities.h"
#include	"BasicMesh.h"
#include	"Texture.h"
#include	"Camera.h"
#include	"randUtils.h"

#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<vector>

using namespace std;

vec3		eye    ( -0.5, 0.8, 0.5 );
Camera		camera ( eye, 0, 0, 0 );	// camera to be used

class	MyWindow : public GlutWindow
{
	float	yaw;
	float	pitch;
	float	roll;
	float	angle;
	Program		program, program2;
	Texture     decalMap;
	Texture		depthMap;
	BasicMesh * plane;
	vector<BasicMesh *> boxes;

	vec3	fogColor;
	float	fogDensity;
	float	hPlane;

public:
	MyWindow () : GlutWindow ( 200, 200, 640, 480, "Per-pixel fog" )
	{
		plane      = createQuad ( vec3 ( -5, hPlane, -5 ), vec3 ( 10, 0, 0 ), vec3 ( 0, 0, 10 ) );
		angle      = 0;
		fogColor   = vec3 ( 0.2, 0.8, 0.8 );
		fogDensity = 1.0;
		hPlane     = 0.2;
		yaw   = 0;
		pitch = 0;
		roll  = 0;
		angle = 0;

		decalMap.load2D ( "Textures/oak.jpg" );
		depthMap.createDepthRect ( 640, 480 );

		if ( !program.loadProgram ( "decal.glsl" ) )
		{
			printf ( "Error loading shader: %s\n", program.getLog ().c_str () );
			
			exit ( 1 );
		}
		
		program.bind ();
		program.setTexture ( "image", 0 );
		program.unbind ();
		
		if ( !program2.loadProgram ( "fog.glsl" ) )
		{
			printf ( "Error loading shader: %s\n", program2.getLog ().c_str () );
			
			exit ( 1 );
		}
		
		program2.bind            ();
		program2.setUniformVector( "fogColor", fogColor );
		program2.setTexture      ( "depthMap",  0 );
		program2.unbind          ();

		camera.setRightHanded ( false );

		buildBoxes ( 200 );
		
		printf ( "Use:\n\t+ and - keys to change fog density\n\t* and / to change plane heights\n." );
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

		depthMap.bind ();
		glCopyTexImage2D ( depthMap.getTarget (), 0, GL_DEPTH_COMPONENT, 0, 0, depthMap.getWidth (), depthMap.getHeight (), 0 );

		glEnable    ( GL_BLEND );
		glBlendFunc ( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
		
		program2.bind ();
		program2.setUniformMatrix ( "proj", proj );
		program2.setUniformMatrix ( "mv",   mv   );
		program2.setUniformFloat  ( "fogDensity", fogDensity );
		plane -> render ();
		
		program2.unbind ();
		
		depthMap.unbind ();

		glDisable ( GL_BLEND );
	}

	void reshape ( int w, int h )
	{
		GlutWindow::reshape ( w, h );
		
		glViewport ( 0, 0, (GLsizei)w, (GLsizei)h );
	   
		camera.setViewSize ( w, h, 60 );
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
		else
		if ( key == '+' )
			fogDensity += 0.01;
		else
		if ( key == '-' )
			fogDensity -= 0.01;
		else
		if ( key == '*' )
			hPlane += 0.05;
		else
		if ( key == '/' )
			hPlane -= 0.05;

		glutPostRedisplay ();
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

private:
	inline float zPlane ( float x, float y )
	{
		return hPlane;
	}

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
