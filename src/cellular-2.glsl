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
layout(location = 3) in vec3 tangent;
layout(location = 4) in vec3 binormal;

out vec3 vt;
out vec3 lt;
out	vec3 ht;
out vec2 tex;
out vec3 n;

void main(void)
{
	n   = normalize ( nm * normal );
	
	vec4 p = mv * vec4 ( pos, 1.0 );
	vec3 v = normalize ( eye - p.xyz );
	vec3 l = normalize ( light - p.xyz );
	vec3 h = normalize ( l + v );	
	vec3 t = normalize ( nm * tangent );
	vec3 b = normalize ( nm * binormal );
	
	vt  = vec3 ( dot ( v, t ), dot ( v, b ), dot ( v, n ) );
	lt  = vec3 ( dot ( l, t ), dot ( l, b ), dot ( l, n ) );
	ht  = vec3 ( dot ( h, t ), dot ( h, b ), dot ( h, n ) );
	tex = texCoord;
	
	gl_Position = proj * p;
}

-- fragment

#version 330 core

uniform sampler2D cellMap;

in vec3 vt;
in vec3 lt;
in vec3 ht;
in vec2 tex;
in vec3 n;
out vec4 color;

vec2	dcell ( in vec2 tex )
{
	float	vx1 = texture( cellMap, tex + vec2 ( 0.01, 0.0 ) ).x;
	float	vx2 = texture( cellMap, tex - vec2 ( 0.01, 0.0 ) ).x;
	float	vy1 = texture( cellMap, tex + vec2 ( 0.0, 0.01 ) ).x;
	float	vy2 = texture( cellMap, tex - vec2 ( 0.0, 0.01 ) ).x;

	vec2	res = 4.0*vec2( vx1*vx1 - vx2*vx2, vy1*vy1 - vy2*vy2 );

	return res;
}

void main(void)
{
    vec2 tx    = vec2 ( 1.0, 3.0 )*tex;
    vec3 cel  = texture ( cellMap, tx ).xyz;
    vec3 cel2 = 0.5*texture ( cellMap, 2.0*tx ).xyz;

    cel += cel2;

    vec2  dn      = dcell ( tx );
	vec3  n       = vec3 ( 0, 0, 1 );
	vec3  t       = vec3 ( 1, 0, 0 );
	vec3  b       = vec3 ( 0, 1, 0 );
    vec3  nn      = normalize ( n + dn.x * t + dn.y * b );
    vec3  l2      = normalize ( lt );
    vec3  h2      = normalize ( ht );
    float diffuse = 0.4 + max ( 0.0, dot ( nn, l2 ) );
    float spec    = pow ( max ( 0.0, dot ( nn, h2 ) ), 30.0 );

	color = pow(1.0-cel.x,2.0)*diffuse * vec4 ( 1.0, 0.0, 0.0, 1.0 ) + vec4 ( 1.0 ) * spec;
}
