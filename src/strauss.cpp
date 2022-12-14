//
// Example of using freeglut and glew to get OpenGL 3.3/4.1
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

class	MeshWindow : public GlutRotateWindow
{
	Program		program;
	BasicMesh * mesh;
	vec3		eye;
	vec3        light;
	float		smooth;
	float		metal;
	float		transp;

public:
	MeshWindow () : GlutRotateWindow ( 200, 200, 400, 400, "Strauss lighting model" )
	{
		if ( !program.loadProgram ( "strauss.glsl" ) )
		{
			printf ( "Error building program: %s\n", program.getLog ().c_str () );
			exit   ( 2 );
		}

		program.bind ();

		mesh  = createKnot ( 1, 4, 120, 30 );
		eye   = vec3 ( 5, 5, 5 );
		light = vec3 ( 5, 5, 5 );

		smooth = 0.5;
		metal  = 1.0;
		transp = 0.3;

		printf ( "Use + and - keys to change smoothness\n" );
		printf ( "Use * and / keys to change metallness\n" );
		printf ( "Use [ and ] keys to change transparency\n" );
	}

	void redisplay ()
	{
		mat4	mv = getRotation ();
		mat3	nm = normalMatrix ( mv );

		glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		program.bind ();
		program.setUniformMatrix ( "mv", mv );
		program.setUniformMatrix ( "nm",  nm );

		mesh -> render ();

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

    void	keyTyped ( unsigned char key, int modifiers, int x, int y )
	{
		if ( key == 27 || key == 'q' || key == 'Q' )	//	quit requested
			exit ( 0 );

		if ( key == '+' )
			smooth += 0.05;
		else
		if ( key == '-' )
			smooth -= 0.05;
		else
		if ( key == '*' )
			metal += 0.05;
		else
		if ( key == '/' )
			metal -= 0.05;

		if ( key == ']' )
			transp += 0.05;
		else
		if ( key == '[' )
			transp -= 0.05;

		if ( smooth > 1.0 )
			smooth = 1.0;
		else
		if ( smooth < 0.0 )
			smooth = 0.0;

		if ( metal > 1.0 )
			metal = 1.0;
		else
		if ( metal < 0.0 )
			metal = 0.0;

		if ( transp > 1.0 )
			transp = 1.0;
		else
		if ( transp < 0.0 )
			transp = 0.0;

		printf ( "Smoothness: %6.3f Metallness: %6.3f Transparency: %6.3f\n", smooth, metal, transp );
	}

    void mouseWheel ( int wheel, int dir, int x, int y )
	{
		eye += 0.5 * vec3 ( dir, dir, dir );

					// since eye value has changed
		reshape ( getWidth(), getHeight() );

		glutPostRedisplay ();
	}

	void	idle ()
	{
		float	angle = 4 * getTime ();

		light.x = 8*cos ( angle );
		light.y = 8*sin ( 1.4 * angle );
		light.z = 8 + 0.5 * sin ( angle / 3 );

		program.bind ();
		program.setUniformVector ( "light", light );
		program.setUniformVector ( "eye",   eye );
		program.setUniformFloat  ( "sm", smooth );
		program.setUniformFloat  ( "mtl",  metal );
		program.setUniformFloat  ( "tr", transp );
		program.unbind ();
		program.unbind ();

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
