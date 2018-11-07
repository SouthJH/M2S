
__kernel void vec_add(__global int *A, __global int *B, __global int *C) {
	int i = get_global_id(0);

	int k = A[i] + B[i];

	C[i] = k;
}

__kernel void vec_add_2d(__global int (*A)[128], __global int (*B)[128], __global int (*C)[128]) {
	int i = get_global_id(0);
	int j = get_global_id(1);

	C[i][j] = A[i][j] + B[i][j];
}