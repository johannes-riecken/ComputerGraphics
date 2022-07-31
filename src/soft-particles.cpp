//
// Soft particles example 
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

#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>

#define	NUM_PARTICLES	3

vec3		eye    ( -0.5, 0.5, 5 );
Camera		camera ( eye, 0, 0, 0 );	// camera to be used

class	MyWindow : public GlutRotateWindow
{
	Program		program, program2;
	Texture 	decalMap;
	Texture 	stoneMap;
	Texture 	knotMap;
	Texture 	depthMap;
	Texture 	smoke;

	BasicMesh * box1;
	BasicMesh * box2;
	BasicMesh * box3;
	BasicMesh * knot;
	BasicMesh * particles;
	mat4 		mv;
	mat4 		proj;
	float		yaw;
	float		pitch;
	float		roll;
	float		angle;

public:
	MyWindow () : GlutRotateWindow ( 200, 200, 400, 400, "Cellular noisy shader" )
	{
		if ( !program.loadProgram ( "decal.glsl" ) )
		{
			printf ( "Error loading shader: %s\n", program.getLog ().c_str () );
			
			exit ( 1 );
		}
		
		program.bind ();
		program.setTexture ( "image", 0 );
		program.unbind ();
		
		if ( !program2.loadProgram ( "soft-particles.glsl" ) )
		{
			printf ( "Error loading shader: %s\n", program2.getLog ().c_str () );
			
			exit ( 1 );
		}
		
		program2.bind ();
		program2.setTexture ( "depthMap", 0 );
		program2.setTexture ( "particleMap", 1 );
		program2.unbind ();
		
		box1 = createBox  ( vec3 ( -6, -0.1, -6 ), vec3 ( 12, 3, 12 ) );
		box2 = createBox  ( vec3 ( -1.5, 0, -0.5 ),  vec3 ( 1,  2,  2 ) );
		box3 = createBox  ( vec3 ( 1.5, 0, -0.5 ),  vec3 ( 1,  1,  1 ) );
		knot = createKnot ( 1, 4, 120, 30 );

		decalMap.load2D ( "Textures/oak.jpg" );
		stoneMap.load2D ( "Textures/block.jpg" );
		knotMap.load2D  ( "Textures/Oxidated.jpg" );
		smoke.load2D    ( "Textures/maskSmoke.bmp" );
		
		depthMap.createDepthRect ( 640, 480 );
		camera.setRightHanded ( false );

		createParticles ();

		yaw       = 0;
		pitch     = 0;
		roll      = 0;
		angle     = 0;
	}

	void redisplay ()
	{
		glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		proj = camera.getProjection ();
		mv   = camera.getModelview ();

		program.bind ();
		program.setUniformMatrix ( "proj",  proj );
		program.setUniformMatrix ( "mv",    mv   );
		
		displayBoxes ();
		
		program.unbind ();
		glFinish ();

		smoke.bind ( 1 );
		depthMap.bind ( 0 );
		glCopyTexImage2D ( depthMap.getTarget (), 0, GL_DEPTH_COMPONENT, 0, 0, depthMap.getWidth (), depthMap.getHeight (), 0 );

		glDepthMask ( GL_FALSE );
		glEnable    ( GL_BLEND );
		glBlendFunc ( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

		program2.bind ();
		program2.setUniformMatrix ( "proj",  proj );
		program2.setUniformMatrix ( "mv",    mv   );
		program2.setUniformVector ( "up", camera.getUpDir () );
		program2.setUniformVector ( "right", camera.getSideDir () );

		particles -> render ();
		
		program2.unbind ();
		
		depthMap.unbind ();
		smoke.unbind ();
		
		glDepthMask ( GL_TRUE );
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

		if ( key == 's' )
		saveScreenAsTga ( "particles.tga", glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT) );
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

    void mouseWheel ( int wheel, int dir, int x, int y )
	{
		eye += 0.5 * vec3 ( dir, dir, dir );
		
					// since eye value has changed
		reshape ( getWidth(), getHeight() );
		
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
		stoneMap.unbind ();

		mat4 m = mv * mat4::translate ( vec3 ( 2, 1, 1 ) ) * mat4::rotateX ( angle * 0.3 ) * mat4::rotateY ( angle * 0.07 ) * mat4::scale( vec3(0.3));
		
		program.setUniformMatrix ( "mv",  m );

		knotMap.bind ();
		
		knot -> render ();
		
		knotMap.unbind ();
	}

	void createParticles ()
	{
		BasicVertex * v = new BasicVertex [NUM_PARTICLES*4];
		int         * ind = new int [6*NUM_PARTICLES];
		
		v [0].pos = vec3 ( 0, -0.5, 0 );
		v [0].tex = vec2 ( 0, 0 );
		v [1].pos = vec3 ( 0, -0.5, 0 );
		v [1].tex = vec2 ( 1, 0 );
		v [2].pos = vec3 ( 0, -0.5, 0 );
		v [2].tex = vec2 ( 1, 1 );
		v [3].pos = vec3 ( 0, -0.5, 0 );
		v [3].tex = vec2 ( 0, 1 );
		
		v [4].pos = vec3 ( -1, -0.5, 1 );
		v [4].tex = vec2 ( 0, 0 );
		v [5].pos = vec3 ( -1, -0.5, 1 );
		v [5].tex = vec2 ( 1, 0 );
		v [6].pos = vec3 ( -1, -0.5, 1 );
		v [6].tex = vec2 ( 1, 1 );
		v [7].pos = vec3 ( -1, -0.5, 1 );
		v [7].tex = vec2 ( 0, 1 );
		
		v [8].pos = vec3 ( 2, 0, 0 );
		v [8].tex = vec2 ( 0, 0 );
		v [9].pos = vec3 ( 2, 0, 0 );
		v [9].tex = vec2 ( 1, 0 );
		v [10].pos = vec3 ( 2, 0, 0 );
		v [10].tex = vec2 ( 1, 1 );
		v [11].pos = vec3 ( 2, 0, 0 );
		v [11].tex = vec2 ( 0, 1 );
		
		for ( int i = 0; i < NUM_PARTICLES; i++ )
		{
			int	k = 6*i;
			int j = 4*i;
			
			ind [k  ] = j;
			ind [k+1] = j + 1;
			ind [k+2] = j+2;
			ind [k+3] = j;
			ind [k+4] = j + 2;
			ind [k+5] = j + 3;
		}
		
		particles = new BasicMesh ( v, ind, 4*NUM_PARTICLES, 2*NUM_PARTICLES );
		
		delete v;
		delete ind;
	}

	void	displayParticle ( const vec3& pos, float r )
	{
		glMultiTexCoord4f ( GL_TEXTURE1_ARB, pos.x, pos.y, pos.z, r );
		
														// now setup particle
		glBegin ( GL_QUADS );
		
		glMultiTexCoord2f ( GL_TEXTURE0_ARB, 0, 0 );
		glVertex3fv  ( pos );
		
		glMultiTexCoord2f ( GL_TEXTURE0_ARB, 0, 1 );
		glVertex3fv  ( pos );
		
		glMultiTexCoord2f ( GL_TEXTURE0_ARB, 1, 1 );
		glVertex3fv  ( pos );
		
		glMultiTexCoord2f ( GL_TEXTURE0_ARB, 1, 0 );
		glVertex3fv  ( pos );
		
		glEnd ();
	}
};

int main ( int argc, char * argv [] )
{
	GlutWindow::init( argc, argv );
	
	MyWindow	win;
	
	GlutWindow::run ();
	
	return 0;
}
