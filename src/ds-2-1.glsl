
-- vertex

#version 330 core

uniform mat4 mv;
uniform mat4 proj;
uniform mat3 nm;

layout ( location = 0 ) in vec3 pos;
layout ( location = 1 ) in vec2 texCoord;
layout ( location = 2 ) in vec3 normal;
layout ( location = 3 ) in vec3 tangent;
layout ( location = 4 ) in vec3 binormal;

out vec2 tex;
out vec3 n;
out vec3 t;
out vec3 b;
out vec4 p;

void main(void)
{
	tex         = texCoord;
	n           = nm * normal;
	t           = nm * tangent;
	b           = nm * binormal;
	p           = mv * vec4 ( pos, 1.0 );
	gl_Position = proj * p;
}

-- fragment

#version 330 core

uniform sampler2D diffMap;
uniform sampler2D bumpMap;

in  vec2 tex;
in  vec3 n;
in  vec3 b;
in  vec3 t;
in  vec4 p;
out vec4 color [3];

void main(void)
{
	const float specPower = 70.0;
	
	vec4 c = texture( diffMap, tex );
	vec3 nt = normalize( 2.0*texture( bumpMap, tex ).rgb - vec3( 1.0 ) );
	
    color [0] = vec4 ( p.xyz, 1.0 );
	color [1] = vec4 ( nt.x*t + nt.y*b + nt.z*n, 1.0 );
	color [2] = vec4 ( c.rgb, specPower );
}
