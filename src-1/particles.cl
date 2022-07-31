__kernel void animate ( __global float * pos, __global float * vel, float dt, int n )
{
	int idx = get_global_id ( 0 );

	if ( idx >= n )
		return;

	idx *= 3;
	
	float3 boxSize = (float3)( 0.7f, 0.7f, 0.7f );
	float3 acc  = (float3) ( 0, -0.07, 0 );	// gravity acceleration
	float  damp = 0.7;			// damping for each reflection
	bool   refl = false;
	
	float3 newPos = (float3)(pos[idx], pos[idx+1], pos[idx+2]);
	float3 newVel = (float3)(vel[idx], vel[idx+1], vel[idx+2]);

	float3 delta  = (float3)( dt, dt, dt );
	
	newPos += delta * newVel;
	newVel += delta * acc;
	
	if ( fabs ( newPos.x ) >= boxSize.x )
	{
		newPos   = (float3)(pos[idx], pos[idx+1], pos[idx+2]);	// return to state before collision
		newVel.x = -newVel.x;
		refl     = true;
	}
	
	if ( fabs ( newPos.y ) >= boxSize.y )
	{
		newPos   = (float3)(pos[idx], pos[idx+1], pos[idx+2]);	// return to state before collision
		newVel.y = -newVel.y;
		refl     = true;
	}
	
	if ( fabs ( newPos.z ) >= boxSize.z )
	{
		newPos   = (float3)(pos[idx], pos[idx+1], pos[idx+2]);	// return to state before collision
		newVel.z = -newVel.z;
		refl     = true;
	}
	
	if ( refl )
		newVel *= damp;

	pos[idx  ] = newPos.x;
	pos[idx+1] = newPos.y;
	pos[idx+2] = newPos.z;
	vel[idx  ] = newVel.x;
	vel[idx+1] = newVel.y;
	vel[idx+2] = newVel.z;
}
