//
// Example of using freeglut and glew to get OpenGL 3.3/4.1
//
// Author: Alex V. Boreskoff <steps3d@gmail.com>, <steps3d@narod.ru>
//

#include	"GlutRotateWindow.h"
#include	"Program.h"
#include	"glUtilities.h"
#include	"mat4.h"
#include	"BasicMesh.h"
#include	"Texture.h"

#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>

#define	NUM_VERTICES	3
#define	VERTEX_SIZE		(6*sizeof(float))

class	MyWindow : public GlutRotateWindow
{
	Program		program;
	Texture		decalMap;
	Texture		heightMap;
	BasicMesh * quad;
	float		scale;
	vec3		eye;
	vec3        light;

public:
	MyWindow () : GlutRotateWindow ( 200, 200, 400, 400, "Parallax Occlusion Mapping" )
	{
		if ( !program.loadProgram ( "pom.glsl" ) )
		{
			printf ( "Error building program: %s\n", program.getLog ().c_str () );

			exit ( 1 );
		}

		decalMap.load2D  ( "Textures/rockwall.jpg" );
		heightMap.load2D ( "Textures/rockwall_relief.png" );

		quad  = createQuad  ( vec3 ( -0.5, 0, -0.5 ), vec3 ( 1, 0, 0 ), vec3 ( 0, 0, 1 ) );
		scale = 0.06;
		eye   = 1.1*vec3 ( 0, 0.33, -0.5 );
		light = vec3 ( 0, -9, 1 );

		program.bind             ();
		program.setTexture       ( "decalMap",  0 );
		program.setTexture       ( "heightMap", 1 );
		program.setUniformVector ( "light", light );
		program.setUniformVector ( "eye",   eye );

		printf ( "Press + and - keys to increase and decrease parallax scale\n" );
	}

	void redisplay ()
	{
		mat4	mv = getRotation ();
		mat3	nm = normalMatrix ( mv );

		glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		program.bind ();
		decalMap.bind  ( 0 );
		heightMap.bind ( 1 );

		program.setUniformMatrix ( "mv", mv );
		program.setUniformMatrix ( "nm", nm );
		program.setUniformFloat  ( "scale", scale );

		quad -> render ();

		program.unbind ();
	}

	void reshape ( int w, int h )
	{
		GlutWindow::reshape ( w, h );

		glViewport ( 0, 0, (GLsizei)w, (GLsizei)h );

		mat4 proj = perspective ( 60.0f, (float)w / (float)h, 0.01f, 50.0f ) * lookAt ( eye, vec3 :: zero, vec3 ( 0, 0, 1 ) );

		program.bind ();
		program.setUniformMatrix ( "proj", proj );
		program.unbind ();
	}

    void mouseWheel ( int wheel, int dir, int x, int y )
	{
		eye += 0.5 * dir * vec3 ( 0, 0.33, -0.5 );

					// since eye value has changed
		reshape ( getWidth(), getHeight() );

		glutPostRedisplay ();
	}

    void	keyTyped ( unsigned char key, int modifiers, int x, int y )
	{
									// quit requested
		if ( key == 27 || key == 'q' || key == 'Q' )
			exit ( 0 );

		if ( key == '+' )
			scale += 0.0025;
		else
		if ( key == '-' )
			scale -= 0.0025;
	}
	void	idle ()
	{
		float	angle = 4 * getTime ();

//		light.x = 8*cos ( angle );
//		light.y = 8*sin ( 1.4 * angle );
//		light.z = 8 + 0.5 * sin ( angle / 3 );

		program.bind ();
		program.setUniformVector ( "light", light );
		program.setUniformVector ( "eye",   eye );
		program.unbind ();

		glutPostRedisplay ();
	}
};

int main ( int argc, char * argv [] )
{
	GlutWindow::init( argc, argv );

	MyWindow	win;

	GlutWindow::run ();

	return 0;
}
