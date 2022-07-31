
-- vertex

#version 330 core

layout(location = 0) in vec4 pos;

out vec2 tex;

void main(void)
{
  tex         = pos.zw;
  gl_Position = vec4 ( pos.xy, 0.0, 1.0 );
}

-- fragment

#version 330 core

uniform sampler2D inputMap;
uniform sampler2D inputLowMap;

in  vec2  tex;
out vec4  color;

void main(void)
{

	vec2  poisson [] = vec2 [8] (  0.0, 0.0, 0.527837, -0.85868, -0.040088,  0.536087, -0.670445, -0.179949, -0.419418, -0.616039, 0.440453, -0.639399, -0.757088,  0.349334, 0.574619,  0.685879 );
	float radiusScale = 0.02;
	vec4  c  = texture( inputMap, tex );
	float blur = abs( c.a - 0.5 );
	float cd   = c.a;
	float discRadius = blur * radiusScale;
	vec4  tapLow, tapHigh, tap;
	
	c = vec4 ( 0.0 );

	for ( int i = 0; i < 8; i++ )
	{
	    tapHigh = texture( inputMap,    tex + poisson [i] * discRadius );
	    tapLow  = texture( inputLowMap, tex + poisson [i] * discRadius );
	    blur    = abs(tapHigh.a - 0.5);
	    tap     = mix( tapHigh, tapLow, blur );		
							
		if ( tap.a >= cd ) 	// apply leaking reduction
			blur = 1.0;
			
		c.rgb += tap.rgb * blur;
		c.a   += blur;
	}
	color = c / c.a;
}
