//
// Rippled reflection example
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
#include	"ScreenQuad.h"

#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>

vec3		eye    (  0, 0.5, 5 );
Camera		camera ( eye, 0, 0, 0 );	// camera to be used
FrameBuffer fb     ( 512, 512, FrameBuffer::depth32 );
ScreenQuad	q      ( 500, 500, 1, 1 );

class	MyWindow : public GlutWindow
{
	Program		program, program2;
	Texture     decalMap;
	Texture     stoneMap;
	Texture     knotMap;
	Texture		noise;
	float		yaw;
	float		pitch;
	float		roll;
	int     	mouseOldX;
	int     	mouseOldY;
	float		angle;
	BasicMesh * box1;
	BasicMesh * box2;
	BasicMesh * knot;
	BasicMesh * w;		// water mesh

	mat4 mv;
	mat4 proj;
	mat4 mirrorMat;

public:
	MyWindow () : GlutWindow ( 200, 200, 500, 500, "Rippled reflection" )
	{
		Texture * image = fb.createColorTexture ( GL_RGBA, GL_RGBA8, GL_CLAMP, 0 );

		fb.create ();
		fb.bind   ();

		if ( !fb.attachColorTexture ( image ) )
			printf ( "buffer error with color attachment\n");

		if ( !fb.isOk () )
			printf ( "Error with framebuffer\n" );

		fb.unbind ();

		if ( !program.loadProgram ( "reflection-1.glsl" ) )
		{
			printf ( "Error loading shader: %s\n", program.getLog ().c_str () );

			exit ( 1 );
		}

		program.bind ();
		program.setTexture ( "image", 0 );
		program.unbind ();

		if ( !program2.loadProgram ( "reflection-2.glsl" ) )
		{
			printf ( "Error loading shader: %s\n", program2.getLog ().c_str () );

			exit ( 1 );
		}

		program2.bind ();
		program2.setTexture ( "image",    0 );
		program2.setTexture ( "noiseMap", 1 );
		program2.unbind ();

		box1 = createBox  ( vec3 ( -6, -2, -6 ),    vec3 ( 12, 5, 12 ) );
		box2 = createBox  ( vec3 ( -1.5, 1, -0.5 ), vec3 ( 1,  2,  2 ) );
		knot = createKnot ( 1, 4, 120, 30 );
		w    = createQuad ( vec3 ( -6, 0, -6 ), vec3 ( 12, 0, 0 ), vec3 ( 0, 0, 12 ) );

		decalMap.load2D ( "Textures/oak.jpg" );
		stoneMap.load2D ( "Textures/block.jpg" );
		knotMap.load2D  ( "Textures/Oxidated.jpg" );
		noise.load3D    ( "Textures/noise-3D.dds" );

		camera.setRightHanded ( false );

		yaw   = 0;
		pitch = 0;
		roll  = 0;
		mouseOldX = 0;
		mouseOldY = 0;
		angle     = 0;
	}

	void redisplay ()
	{
		vec3 up   = camera.getUpDir ();
		vec3 view = camera.getViewDir ();
		vec3 pos  = camera.getPos ();

		proj = camera.getProjection ();
		mv   = lookAt ( pos, pos + view, up );

		renderToBuffer ();

		glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		proj = camera.getProjection ();
		mv   = lookAt ( pos, pos + view, up );

		program.bind ();
		program.setUniformMatrix ( "proj",  proj );
		program.setUniformMatrix ( "mv",    mv   );

		displayBoxes ();

		program.unbind ();

		renderWater ();
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

		reshape ( getWidth (), getHeight () );

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
		stoneMap.unbind ();

		mat4 m = mv * mat4::translate ( vec3 ( 2, 0, 1 ) ) * mat4::rotateX ( angle * 0.3 ) * mat4::rotateY ( angle * 0.07 ) * mat4::scale( vec3(0.3));

		program.setUniformMatrix ( "mv",  m );

		knotMap.bind ();

		knot -> render ();

		knotMap.unbind ();
	}

	void    renderToBuffer ()
	{
		fb.bind ();

		glClearColor ( 0, 0, 1, 1 );
		glClear      ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		vec3 up    = camera.getUpDir ();
		vec3 view  = camera.getViewDir ();
		vec3 pos   = camera.getPos ();

		up.y   = -up.y;
		view.y = -view.y;
		pos.y  = -pos.y;

		mat4 m = lookAt ( pos, pos + view, up );// * mat4 :: scale ( vec3 (1, -1, 1) );

		mv        = m;
		mirrorMat = mat4::translate(vec3(0.5, 0.5, 0 ) ) * mat4::scale(vec3(0.5, 0.5, 1)) * proj * m;

		program.bind ();
		program.setUniformMatrix ( "proj",  camera.getProjection () );
		program.setUniformMatrix ( "mv",    m );

		displayBoxes ();

		fb.unbind ();
	}

	void renderWater ()
	{
		noise.bind ( 1 );
		fb.getColorBuffer () -> bind ();

		program2.bind ();
		program2.setUniformMatrix ( "proj",  proj );
		program2.setUniformMatrix ( "mv",    mv   );
		program2.setUniformMatrix ( "mirrorMat", mirrorMat );
		program2.setUniformFloat  ( "time",  getTime () );

		w  -> render ();

		program2.unbind ();

		fb.getColorBuffer () -> unbind ();
		noise.unbind ();
	}
};

int main ( int argc, char * argv [] )
{
	GlutWindow::init( argc, argv );

	MyWindow	win;

	GlutWindow::run ();

	return 0;
}
