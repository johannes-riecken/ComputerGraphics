
-- vertex

#version 330 core

layout(location = 0) in vec4 pos;

out vec2 tex;

void main(void)
{
  tex         = pos.zw;
  gl_Position = vec4 ( pos.xy, 0.0, 1.0 );
}

-- fragment

#version 330 core

uniform sampler2DRect	depthMap;
uniform sampler2D		rotateMap;
//uniform float			radius;
//uniform	float 		distScale;
//uniform float			bias;

//in  vec3 pos;
in  vec2 tex;
out vec4 color;

void main(void)
{
	const float zFar      = 100.0;
	const float zNear     = 0.1;
	const float attScale  = 1.0;
	const float radius = 40;//0.1;
	const float distScale = 0.25;
	const float bias      = 0.45;
	
	vec4	rndTable [8] = vec4 [8] 
	(
		vec4 ( -0.5, -0.5, -0.5, 0.0 ),
		vec4 (  0.5, -0.5, -0.5, 0.0 ),
		vec4 ( -0.5,  0.5, -0.5, 0.0 ),
		vec4 (  0.5,  0.5, -0.5, 0.0 ),
		vec4 ( -0.5, -0.5,  0.5, 0.0 ),
		vec4 (  0.5, -0.5,  0.5, 0.0 ),
		vec4 ( -0.5,  0.5,  0.5, 0.0 ),
		vec4 (  0.5,  0.5,  0.5, 0.0 )
	);
	
	float zSample = texture ( depthMap, gl_FragCoord.xy ).x;
	float z       = zFar * zNear / (zSample * (zFar - zNear) - zFar );
	float att     = 0.0;
	vec3 plane    = 2.0 * texture ( rotateMap, gl_FragCoord.xy / 4.0).xyz - vec3 ( 1.0 );

	for ( int i = 0; i < 8; i++ )
	{
		vec3	sample  = reflect( rndTable [i].xyz, plane );
		float	zSample = texture( depthMap, gl_FragCoord.xy + radius*sample.xy / z ).x;

		zSample = zFar * zNear / (zSample * (zFar - zNear) - zFar );
		
		float	dist = max ( zSample - z, 0.0 ) / distScale;	
		float	occl = 15.0 * max ( dist * (2.0 - dist), 0.0 );
		
		att += 1.0 / ( 1.0 + occl*occl );
	}

	att = clamp ( att / 8.0 + 0.45, 0.0, 1.0 ) * attScale;
	
	color = vec4 ( att );
}
