#include <stdio.h>

__global__ void square(float *d_out, float *d_in){
	int idx = threadIdx.x;
	float f = d_in[idx];
	d_out[idx] = f * f;
}

int main(int argc, char **argv){
	const int array_size = 64;
	const int array_bytes = array_size * sizeof(float);

	// generate the input data on the host
	float h_in[array_size];
	for(int i = 0; i < array_size; i++){
		h_in[i] = float(i);
	}
	float h_out[array_size];

	// declare gpu memory
	float *d_in;
	float *d_out;
	
	// allocate gpu memory
	cudaMalloc((void **)&d_in, array_bytes);
	cudaMalloc((void **)&d_out, array_bytes);

	// transfer the array to the gpu
	cudaMemcpy(d_in, h_in, array_bytes, cudaMemcpyHostToDevice);
	
	// launch the kernel
	square<<<1, array_size>>>(d_out, d_in);

	// copy back the result to cpu
	cudaMemcpy(h_out, d_out, array_bytes, cudaMemcpyDeviceToHost);

	for(int i = 0; i < array_size; i++){
		fprintf(stdout, "h_out[i]\n");
	}

	// free gpu memory allocation
	cudaFree(d_in);
	cudaFree(d_out);

	return 0;
}
