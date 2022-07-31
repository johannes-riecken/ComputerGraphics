//
// Lighting example 
//
// Author: Alexey V. Boreskov <steps3d@gmail.com>, <steps3d@narod.ru>
//

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
	Program	    program;
	BasicMesh * mesh;
	Texture	    furMap;
	vec3		eye;
	vec3		light;
	int			numLayers;
	float		angle;
	
public:
	MyWindow () : GlutRotateWindow ( 200, 200, 600, 600, "Fur rendering" )
	{
		furMap.load2D ( "Textures/fur-map-00.png" );
		
		if ( !program.loadProgram ( "fur.glsl" ) )
		{
			printf ( "Error loading shader: %s\n", program.getLog ().c_str () );
			
			exit ( 1 );
		}
		
		program.bind ();
		program.setTexture ( "furMap", 0 );
		program.unbind ();

		mesh      = createTorus ( 1.5, 4, 30, 30 );
		eye       = vec3 ( 7, 7, 7 );
		light     = vec3 ( 7, 7, 7 );
		numLayers = 20;
		angle     = 0;
	}
	
	void redisplay ()
	{
		mat4	mv = getRotation ();
		mat3	nm = normalMatrix ( mv );
		
		glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		float	shadowMin = 0.2;
		float	shadowMax = 0.5;
		
		glEnable           ( GL_BLEND );
		glBlendFunc        ( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
		
		furMap.bind ();
		program.bind ();
		program.setUniformMatrix ( "mv",  mv );
		program.setUniformMatrix ( "nm",  nm );
		program.setUniformFloat  ( "time", 4*getTime () );

		for ( int i = 0; i < numLayers; i++ )
		{
			float	t      = (float) i / 16.0;
			float	shadow = 0.7*(shadowMin * (1 - t) + shadowMax * t);

			program.setUniformFloat ( "tl", t );

			mesh -> render ();
		}
		
		program.unbind ();
		furMap.unbind ();
		
		glDisable ( GL_BLEND );
	}

	void reshape ( int w, int h )
	{
		GlutWindow::reshape ( w, h );
		
		glViewport ( 0, 0, (GLsizei)w, (GLsizei)h );
	   
		mat4 proj = perspective ( 60.0f, (float)w / (float)h, 0.5f, 20.0f ) * lookAt ( eye, vec3 :: zero, vec3 ( 0, 0, 1 ) );

		program.bind ();
		program.setUniformMatrix ( "proj", proj );
		program.setUniformVector ( "eye",   eye );
		program.setUniformVector ( "light", light );
		program.unbind ();  
	}

	void	idel ()
	{
		angle   = 4 * getTime ();
		light.x = 8*cos ( angle );
		light.y = 8*sin ( 1.4 * angle );
		light.z = 8 + 0.5 * sin ( angle / 3 );

		program.bind ();
		program.setUniformVector ( "eye",   eye   );
		program.setUniformVector ( "light", light );
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

