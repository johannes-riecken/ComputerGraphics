#include	"GlutRotateWindow.h"
#include	"Program.h"
#include	"glUtilities.h"
#include	"BasicMesh.h"
#include	"Texture.h"

#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>

class	MyWindow : public GlutRotateWindow
{
	Program		program;
	Texture		tex;
	BasicMesh * mesh;
	vec3		eye;
	vec3		light;
	float		scale;

public:
	MyWindow () : GlutRotateWindow ( 200, 200, 400, 400, "Procedural marble" )
	{
		if ( !tex.load3D ( "Textures/noise-3D.dds" ) )
			printf ( "Error loading texture\n" );
			
		if ( !program.loadProgram ( "marble.glsl" ) )
		{
			printf ( "Error building program: %s\n", program.getLog ().c_str () );
			
			exit ( 1 );
		}
		
		mesh = createTorus ( 2, 4, 30, 30 );
		
		program.bind ();
		program.setTexture ( "noiseMap", 0 );
		program.unbind ();
	
		eye    = vec3 ( 7, 7, 7 );
		light  = vec3 ( 7, 7, 7 );
		scale  = 10.0;
		
		printf ( "Procedural marble\nUse + and - keys to change scale\n" );
	}
	
	void redisplay ()
	{
		mat4	mv = getRotation ();
		mat3	nm = normalMatrix ( mv );
		
		glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		program.bind ();
		program.setUniformMatrix ( "mv",  mv );
		program.setUniformMatrix ( "nm",  nm );

		tex.bind ( 0 );
		glTexParameteri ( GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		glTexParameteri ( GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		
		mesh -> render ();
		tex.unbind ();

		program.unbind ();
	}

	void reshape ( int w, int h )
	{
		GlutWindow::reshape ( w, h );
		
		glViewport ( 0, 0, (GLsizei)w, (GLsizei)h );
	   
		mat4 proj = perspective ( 60.0f, (float)w / (float)h, 0.5f, 20.0f ) * lookAt ( eye, vec3 :: zero, vec3 ( 0, 0, 1 ) );

		program.bind ();
		program.setUniformMatrix ( "proj",  proj );
		program.setUniformVector ( "eye",   eye );
		program.setUniformVector ( "light", light );
		program.setUniformFloat  ( "scale", scale );
		program.unbind ();  
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
		program.unbind ();
		
		glutPostRedisplay ();
	}

    void	keyTyped ( unsigned char key, int modifiers, int x, int y )
	{
		if ( key == 27 || key == 'q' || key == 'Q' )	//	quit requested
			exit ( 0 );

		if ( key == '+' )
			scale += 0.1;
		else
		if ( key == '-' )
			scale -= 0.1;
			
		program.bind ();
		program.setUniformFloat  ( "scale", scale );
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

