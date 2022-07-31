
-- vertex

#version 330 core

uniform mat4 mv;
uniform mat4 proj;

layout ( location = 0 ) in vec3 position;

out vec2 tex;
out vec3 pos;

void main(void)
{
	pos         = (mv * vec4 ( position, 1.0 )).xyz;
	gl_Position = proj * mv * vec4 ( position, 1.0 );
}

-- fragment

#version 330 core

uniform sampler2DRect	depthMap;
uniform float           fogDensity;
uniform vec3            fogColor;

in  vec3 pos;
out vec4 color;

void main(void)
{
	float zSample = texture ( depthMap, gl_FragCoord.xy ).x;
	float zNear   = 0.1;
	float zFar    = 100.0;

	zSample = zFar * zNear / (zSample * (zFar - zNear) - zFar );

	float alpha = abs(zSample - pos.z) * fogDensity;

	color = vec4 ( fogColor, alpha );		// get fog color from texCoord[0]
}
