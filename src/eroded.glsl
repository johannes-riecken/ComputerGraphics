-- vertex

#version 330 core

uniform mat4 proj;
uniform mat4 mv;
uniform mat3 nm;
uniform	vec3 eye;
uniform vec3 light;

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 tex;
layout(location = 2) in vec3 normal;

out vec3 n;
out vec3 v;
out vec3 l;
out	vec3 h;
out vec3 pp;

void main(void)
{
	vec4	p = mv * vec4 ( pos, 1.0 );

	v           = normalize ( eye - p.xyz );
	n           = normalize ( nm * normal );
	l            = normalize ( light - p.xyz );
	h            = normalize ( l + v );	
	pp          = pos.xyz * 0.234567;
	gl_Position = proj * p;
}

-- fragment

#version 330 core

//
// Eroded fragment shader
//

uniform	float		offs;
uniform sampler3D 	noiseMap;

in vec3 n;
in vec3 v;
in vec3 l;
in vec3 h;
in vec3 pp;

out vec4 color;

vec3	turbulence ( const in vec3 p, const in float freqScale )
{
	float	sum = 0.0;
	vec3	t   = vec3 ( 0.0 );
	float	f   = 1.0;

	for ( int i = 0; i <= 3; i++ )
	{
		t   += abs ( 2.0 * texture ( noiseMap, f * p.xyz ).rgb - vec3 ( 1.0 ) ) / f;
		sum += 1.0 / f;
		f   *= freqScale;
	}

										// remap from [0,sum] to [-1,1]
	return 2.0 * t / sum - vec3 ( 1.0 );
}

void main (void)
{
	const vec4	diffColor = vec4 ( 0.5, 0.0, 0.0, 1.0 );
	const vec4	specColor = vec4 ( 0.7, 0.7, 0.0, 1.0 );
	const float	specPower = 70.0;
	const float delta     = 0.3;

	vec3	ns     = abs( turbulence ( pp, 2.07193 ));
	float	factor = (ns.x + ns.y + ns.z) / 3.0;

	if ( factor < offs )
		discard;

	vec3	n2   = normalize ( n );
	vec3	l2   = normalize ( l );
	vec3	h2   = normalize ( h );
	vec4	diff = vec4 ( max ( dot ( n2, l2 ), 0.0 ) + 0.3 );
	vec4	spec = specColor * pow ( max ( dot ( n2, h2 ), 0.0 ), specPower );
	float	f    = smoothstep ( offs, offs + delta, factor );
		
	color = f * (diff * diffColor + spec);
}
