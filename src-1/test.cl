__kernel void test ( __global float * a, int n )
{
	int idx = get_global_id ( 0 );

	if ( idx < n )
		a [idx] = sin ( idx * 3.1415926f / 1024.0f );
}
