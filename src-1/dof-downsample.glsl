
-- vertex

#version 330 core

layout(location = 0) in vec4 pos;

uniform sampler2D inputMap;

out vec2 tex;
out vec2 dt;

void main(void)
{
  vec2 sz = textureSize( inputMap, 0 );
  
  dt          = vec2 ( 1.0/float(sz.x), 1.0/float(sz.y) );
  tex         = pos.zw;
  gl_Position = vec4 ( pos.xy, 0.0, 1.0 );
}

-- fragment

#version 330 core

uniform sampler2D inputMap;

in vec2 tex;
in vec2 dt;

out vec4 color;
void main (void)
{
	vec2	d1 = vec2 ( dt.x, dt.y );
	vec2	d2 = vec2 ( dt.x, -dt.y  );
	vec2	d3 = vec2 ( -dt.x, dt.y  );
	vec2	d4 = vec2 ( -dt.x, -dt.y );

	color = (texture( inputMap, vec2 ( tex + d1 ) ) + 
             texture( inputMap, vec2 ( tex + d2 ) ) + 
             texture( inputMap, vec2 ( tex + d3 ) ) + 
             texture( inputMap, vec2 ( tex + d4 ) ) ) * 0.25;
}
