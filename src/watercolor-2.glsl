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

out vec3 diffuseThickness;
out vec3 unlitThickness;
out vec2 tex;

void main(void)
{
	const	vec3	ambient   = vec3 ( 0.4, 0.4, 0.4 );
	const	vec3	diffuse   = vec3 ( 0.0, 0.0, 1.0 );
	const	float	specPower = 50.0;

	vec4 p = mv * vec4 ( pos, 1.0 );
	vec3 l = normalize ( light - p.xyz );
	vec3 v = normalize ( eye - p.xyz );
	vec3 h = normalize ( l + v );
	vec3 n = normalize ( nm * normal );

											// compute layers thicknesses
	diffuseThickness = (1.0 - pow ( max ( dot ( n, h ), 0.0 ), specPower ) ) * 
	                   (vec3( 1.0 ) - diffuse) * max ( dot ( n, v ), 0.0 );
	unlitThickness   = (1.0 - max ( dot ( n, l ), 0.0 ) ) * (vec3( 1.0 ) - ambient);
	tex              = texCoord;
	gl_Position      = proj * p;
}

-- fragment

#version 330 core

uniform sampler2D noiseMap;

in vec3 diffuseThickness;
in vec3 unlitThickness;
in vec2 tex;

out vec4 color;

void main (void)
{
	vec3	c     = diffuseThickness;
	vec3	noise = texture( noiseMap, tex * vec2 ( 9*0.7, 2.0 ) ).xyz;

	color = vec4 ( vec3 ( 1.0 ) - c * unlitThickness * noise.x, 1.0 );
}
