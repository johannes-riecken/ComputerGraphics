-- vertex

#version 330 core

uniform mat4 proj;
uniform mat4 mv;
uniform mat3 nm;
uniform	vec3 eye;
uniform vec3 light;

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 normal;

out vec3 n;
out vec3 v;
out vec3 l;
out vec2 tex;

void main(void)
{
	vec4 p = mv * vec4 ( pos, 1.0 );
	
	l = normalize ( light - p.xyz );
	v = normalize ( eye - p.xyz );
	n = normalize ( nm * normal );
	tex         = texCoord;
	gl_Position = proj * p;
}

-- fragment

#version 330 core

uniform sampler2D cetMap;
uniform sampler2D randomMap;

in vec3 n;
in vec3 v;
in vec3 l;
in vec2 tex;

out vec4 color;

void main (void)
{
	vec3 n2 = normalize( n );
	vec3 l2 = normalize( l );
	float c = 0.0 + max ( dot ( n2, l2 ), 0.0 );
	
	c  = pow( clamp( c, 0.0, 1.0 ), 3.5 ); 
	
	vec4	random   = texture( randomMap, 5*vec2 ( 1, 4 ) * tex );
	vec3	contrast = texture( cetMap, vec2 ( random.x, c ) ).rgb;
	vec3	smudge   = 0.5 * ( contrast + vec3 ( c ) );
	vec3	paper    = vec3( 0.0);//texture( paperTex, gl_FragCoord.xy / 512.0 ).rgb;

//color = random;
//color = vec4 ( contrast, 1.0 );
	color = vec4 ( smudge /*+ vec3 ( 1.0 ) - paper*/, 1.0 );
	
//	color = texture ( cetMap, vec2 ( 0.5, c ) ) + vec4 ( 1.0 - random.x );
}
