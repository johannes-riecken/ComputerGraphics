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
	Program		program;      // sky
	Program     program2;     // water plane
	BasicMesh * sky;
	BasicMesh * water;
	Texture		tex;
	Texture 	noise;
	vec3		eye;
	
public:
	MeshWindow () : GlutRotateWindow ( 200, 200, 400, 400, "Water rendering" )
	{
		string	texName = "Textures/Snow.dds";
	
		if ( !tex.loadCubemap ( texName.c_str () ) )
		{
			printf ( "Error loading texture %s\n", texName.c_str () );
			exit   ( 1 );
		}
		
		noise.load3D ( "Textures/noise-3D.dds" );

		if ( !program.loadProgram ( "sky-cube.glsl" ) )
		{
			printf ( "Error building program: %s\n", program.getLog ().c_str () );
			exit   ( 2 );
		}
		
		
		program.bind ();
		program.setTexture ( "image", 0 );
		program.unbind ();
		
		
		if ( !program2.loadProgram ( "water.glsl" ) )
		{
			printf ( "Error building program: %s\n", program2.getLog ().c_str () );
			exit   ( 2 );
		}
		
		program2.bind ();
		program2.setTexture ( "image", 0 );
		program2.setTexture ( "noiseMap", 1 );
		program2.unbind ();
		
		sky = createBox ( vec3 ( -5, -5, -5 ), vec3 ( 10, 10, 10 ) );
		water = createQuad  ( vec3 ( -5, -5, -4.9 ), vec3 ( 10, 0, 0 ), vec3 ( 0, 10, 0 ) );
		eye = vec3 ( 1, 1, -4 );
	}

	void redisplay ()
	{
		mat4 mv = getRotation  ();
		mat3 nm = normalMatrix ( mv );
		
		glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		tex.bind ();
		program.bind ();
		program.setUniformMatrix ( "mv", mv );

		sky -> render ();
		
		program.unbind ();
		
		noise.bind ( 1 );
		program2.bind ();
		program2.setUniformMatrix ( "mv", mv );
		program2.setUniformMatrix ( "nm", nm );
		program2.setUniformFloat  ( "time", getTime () );

		water -> render ();
		
		program2.unbind ();		
		noise.unbind ();
		
		
		tex.unbind ();
	}

	void reshape ( int w, int h )
	{
		GlutWindow::reshape ( w, h );
		
		glViewport ( 0, 0, (GLsizei)w, (GLsizei)h );
	   
		mat4 proj = perspective ( 60.0f, (float)w / (float)h, 0.015f, 20.0f ) * lookAt ( eye, vec3 :: zero, vec3 ( 0, 0, 1 ) );

		program.bind ();
		program.setUniformMatrix ( "proj", proj );
		program.unbind ();  

		program2.bind ();
		program2.setUniformMatrix ( "proj", proj );
		program2.unbind ();  
	}

};

int main ( int argc, char * argv [] )
{
	GlutWindow::init( argc, argv );
	
	MeshWindow	win;
	
	GlutWindow::run ();
	
	return 0;
}
