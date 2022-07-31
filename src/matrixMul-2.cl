
#define BLOCK_SIZE 16
#define AS(i,j) as[j+i*BLOCK_SIZE]
#define BS(i,j) bs[j+i*BLOCK_SIZE]

__kernel void matrixMul( __global float * a, __global float * b, __global float * c, 
	   __local float* as, __local float * bs, int n )
{
    int bx = get_group_id(0);
    int by = get_group_id(1);
    int tx = get_local_id(0);
    int ty = get_local_id(1);

    int   ai    = n * BLOCK_SIZE * by;
    int   aEnd  = ai + n - 1;
    int   aStep = BLOCK_SIZE;
    int   bi    = BLOCK_SIZE * bx;
    int   bStep = BLOCK_SIZE * n;
	int   offs  = n * ty + tx;
    float sum = 0.0f;

    for ( ; ai <= aEnd; ai += aStep, bi += bStep )
	{
            // load the matrices from 
            // global to shared memory
        AS(ty, tx) = a[ai + offs];
        BS(ty, tx) = b[bi + offs];
	
           // make sure the matrices are loaded
        barrier(CLK_LOCAL_MEM_FENCE);

           // multiply the matrices
        for ( int k = 0; k < BLOCK_SIZE; k++ )
            sum += AS(ty, k) * BS(k, tx);

          // make sure that the computation is done
        barrier(CLK_LOCAL_MEM_FENCE);
    }

    c[get_global_id(1) * n + get_global_id(0)] = sum;
}

