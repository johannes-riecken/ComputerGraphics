
#define BLOCK_SIZE 16

__kernel void matrixMul( __global float * a, __global float * b, __global float * c, int n )
{
    int   x     = get_global_id(0);
    int   y     = get_global_id(1);
    float sum   = 0.0f;
    int   aOffs = y * n;
    int   bOffs = x;

	for ( int i = 0; i < n; i++ )
	{
		sum += a[aOffs + i] * b [bOffs];
		bOffs += n;
	}
	
    c[get_global_id(1) * n + get_global_id(0)] = sum;
}

