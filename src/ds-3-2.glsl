
-- vertex

#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 texCoord;

uniform mat4 mv;
uniform mat4 proj;

out vec4 pp;
out vec2 tex;

void main(void)
{
  tex         = texCoord;
  pp           = mv * vec4 ( pos.xyz, 1.0 );
  gl_Position = proj * pp;
}

-- fragment

#version 330 core

uniform sampler2D normalMap;
uniform sampler2D diffMap;
uniform vec4      light;

in  vec4 pp;
in  vec2 tex;
out vec4 color;

vec3 decodeNormal ( in vec2 nn )
{
	if ( nn.x > 1.5 )		// negative n.z
	{
		float	nx = nn.x - 3.0;
		float	nz = -sqrt( 1.0 - nx*nx - nn.y*nn.y );
		
		return vec3( nx, nn.y, nz );
	}
	
	return vec3 ( nn.x, nn.y, sqrt ( 1.0 - nn.x*nn.x - nn.y*nn.y ) );
}

void main(void)
{
	vec4	c1 = texture ( normalMap, tex );
	float	z  = c1.x;
	float	ks = c1.w;
	vec3	p  = pp.xyz * z / pp.z;
	vec3	n  = normalize(decodeNormal ( c1.yz ));
	vec4    c2 = texture( diffMap, tex );
	vec3	l  = normalize ( light.xyz - p );
	vec3	v  = normalize ( -p );
	vec3	h  = normalize ( l + v );
	float	diff = max ( 0.2, max( 0.0, dot ( l, n ) ) );	
	float	spec = pow ( max( 0.0, dot ( h, n ) ), c2.a );
	
	color = vec4 ( diff*c2.rgb + vec3( spec ), 1.0 );
}
