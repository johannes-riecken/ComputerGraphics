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
out vec3 pp;
out vec2 tex;
out vec3 n;

void main(void)
{
	vec4 p = mv * vec4 ( pos, 1.0 );
	vec3 v = normalize ( eye - p.xyz );
	vec3 l = normalize ( light - p.xyz );
	vec3 h = normalize ( l + v );	
	vec3 t = normalize ( nm * tangent );
	vec3 b = normalize ( nm * binormal );
	
	n   = normalize ( nm * normal );
	vt  = vec3 ( dot ( v, t ), dot ( v, b ), dot ( v, n ) );
	lt  = vec3 ( dot ( l, t ), dot ( l, b ), dot ( l, n ) );
	ht  = vec3 ( dot ( h, t ), dot ( h, b ), dot ( h, n ) );
	pp  = 2.0*pos.xyz;
	tex = texCoord;
	
	gl_Position = proj * p;
}

-- fragment

#version 330 core

uniform sampler3D noiseMap;
uniform sampler2D cellMap;
uniform	float     time;

in vec3 vt;
in vec3 lt;
in vec3 ht;
in vec3 pp;
in vec2 tex;
in vec3 n;
out vec4 color;

float f ( const in vec2 tex )
{
    float t = texture( cellMap, tex ).x;

    return t*t;
}

vec3    df ( const in vec2 p )
{
    const float dx = 0.01;

    float fx = f( p + vec2 ( dx, 0 ) ) - f ( p - vec2 ( dx, 0 ) ) / (2.0*dx);
    float fy = f( p + vec2 ( 0, dx ) ) - f ( p - vec2 ( 0, dx ) ) / (2.0*dx);

    return vec3 ( fx, fy, 0 );
}
void main (void)
{
    vec3     ns1     = texture( noiseMap, pp + 4.0*time * vec3 ( 0.1, 0.0831, 0.119 ) ).xyz;
    vec2     t       = vec2( 1.0, 3.0 )*tex + 0.015*ns1.xy;
    float    d1      = abs( f( t + vec2 ( 0.01, 0 ) ) - f ( t ) );
    float    d2      = abs( f( t + vec2 ( 0, 0.01 ) ) - f ( t ) );
    vec3	 n       = vec3( 0.0, 0.0, 1.0 );
    vec3     nn      = normalize( n - 7.0*vec3 ( d1, d2, 0 ) );
    vec3     ln      = normalize( lt );
    vec3     hn      = normalize( ht );
    float    diffuse = 0.4 + max ( 0.0, dot ( nn, ln ) );
    float    spec    = 0.5*pow ( max ( 0.0, dot ( nn, hn ) ), 30.0 );

   color = vec4 ( f(t)+f(2.0*t), 0.0, 0.0, 1.0 )*diffuse + spec*vec4 ( 1.0, 1.0, 0.0, 1.0 );
}
