
-- vertex

#version 330 core

uniform mat4  mv;
uniform mat4  proj;
uniform vec3  up;
uniform vec3  right;

layout ( location = 0 ) in vec3 position;
layout ( location = 1 ) in vec2 texCoord;

out vec2 tex;

void main(void)
{
	vec3 pos = position + 2.0*texCoord.x*right + 2.0*texCoord.y * up;
	
	gl_Position = proj * mv * vec4 ( pos, 1.0 );
	tex         = texCoord;
}

-- fragment

#version 330 core

uniform sampler2DRect	depthMap;
uniform sampler2D		particleMap;

in  vec2 tex;
out vec4 color;

void main(void)
{
	float zSample = texture ( depthMap, gl_FragCoord.xy ).x;
	vec4  c       = texture ( particleMap, tex );
	float zNear   = 0.1;
	float zFar    = 100.0;

	if ( c.r < 0.02 )
		discard;
		
	c.a *= c.r;
	
	float d1 = zFar * zNear / (zSample * (zFar - zNear) - zFar );
	float d2 = zFar * zNear / (gl_FragCoord.z * (zFar - zNear) - zFar );

	float	d = min ( 0.5, abs ( d1 - d2 )*0.3 );
	
	c.a *= d;
	
	color = c;
}
