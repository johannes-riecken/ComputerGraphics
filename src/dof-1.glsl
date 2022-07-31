
-- vertex

#version 330 core

uniform mat4  mv;
uniform mat4  proj;

layout ( location = 0 ) in vec3 position;
layout ( location = 1 ) in vec2 texCoord;

out vec2  tex;
out float z;

void main(void)
{
	vec4 p = mv * vec4 ( position, 1.0 );
	
	gl_Position = proj * p;
	tex         = texCoord;
	z           = p.z;
}

-- fragment

#version 330 core

uniform sampler2D imageMap;
uniform float focalDistance, focalRange;

in  vec2  tex;
in  float z;
out vec4  color;

void main(void)
{
	float	blur = clamp(0.5 + (focalDistance + z)/focalRange, 0.0, 1.0 );
	
	color = vec4 ( texture ( imageMap, tex ).rgb, blur );
}
