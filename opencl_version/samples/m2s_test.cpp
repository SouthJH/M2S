#define _CRT_SECURE_NO_WARNINGS
#include "m2s_api.h"

#define   CHECK_ERROR(err) \
  if (err != CL_SUCCESS) { \
     printf("[%s:%d] ERROR %d\n", __FILE__, __LINE__, err); \
     exit(EXIT_FAILURE); \
  }


char *get_source_code(const char *file_name, size_t *len) {
	char *source_code;
	size_t length;
	FILE *file = fopen(file_name, "rb");
	if (file == NULL) {
		printf("[%s:%d] Failed to open %s\n", __FILE__, __LINE__, file_name);
		exit(EXIT_FAILURE);
	}

	fseek(file, 0, SEEK_END);
	length = (size_t)ftell(file);
	rewind(file);

	source_code = (char *)malloc(length + 1);
	fread(source_code, length, 1, file);
	source_code[length] = '\0';

	fclose(file);

	*len = length;
	return source_code;
}

void test(int n) {
	m2s_platform_id platform;
	m2s_device_id device;
	m2s_context context;
	m2s_command_queue queue;
	m2s_program program;
	char *kernel_source;
	size_t kernel_source_size;
	m2s_kernel kernel;
	m2s_int err;

	err = m2sGetPlatformIDs(1, &platform, NULL);								CHECK_ERROR(err);
	err = m2sGetDeviceIDs(platform, M2S_DEVICE_TYPE_GPU, n, &device, NULL);		CHECK_ERROR(err);
	context = m2sCreateContext(NULL, device, NULL, NULL, &err);					CHECK_ERROR(err);
	queue = m2sCreateCommandQueue(context, device, 0, &err);					CHECK_ERROR(err);
	
	kernel_source = get_source_code("kernel.cl", &kernel_source_size);
	program = m2sCreateProgramWithSource(context, 1, (const char **)&kernel_source, &kernel_source_size, &err);		CHECK_ERROR(err);
	err = m2sBuildProgram(program, &device, "", NULL, NULL);
	if (err == CL_BUILD_PROGRAM_FAILURE) {
		printf("failed to build program\n");
		exit(0);
	} CHECK_ERROR(err);

	kernel = m2sCreateKernel(program, "vec_add", &err);		CHECK_ERROR(err);

	int A[16384], B[16384], C[16384];
	for (int idx = 0; idx < 16384; ++idx) {
		A[idx] = rand() % 100;
		B[idx] = rand() % 100;
	}

	m2s_mem buffer_A, buffer_B, buffer_C;

	buffer_A = m2sCreateBuffer(context, &device, M2S_MEM_HINT_WHOLE, M2S_MEM_READ_ONLY, sizeof(int) * 16384, NULL, &err);	CHECK_ERROR(err);
	buffer_B = m2sCreateBuffer(context, &device, M2S_MEM_HINT_WHOLE, M2S_MEM_READ_ONLY, sizeof(int) * 16384, NULL, &err);	CHECK_ERROR(err);
	buffer_C = m2sCreateBuffer(context, &device, M2S_MEM_HINT_WHOLE, M2S_MEM_WRITE_ONLY, sizeof(int) * 16384, NULL, &err);	CHECK_ERROR(err);

	err = m2sEnqueueWriteBuffer(queue, &device, buffer_A, M2S_FALSE, 0, sizeof(int) * 16384, A, 0, NULL, NULL);		CHECK_ERROR(err);
	err = m2sEnqueueWriteBuffer(queue, &device, buffer_B, M2S_FALSE, 0, sizeof(int) * 16384, B, 0, NULL, NULL);		CHECK_ERROR(err);

	err = m2sSetKernelArg(kernel, &device, 0, M2S_MEM_SIZE, &buffer_A);		CHECK_ERROR(err);
	err = m2sSetKernelArg(kernel, &device, 1, M2S_MEM_SIZE, &buffer_B);		CHECK_ERROR(err);
	err = m2sSetKernelArg(kernel, &device, 2, M2S_MEM_SIZE, &buffer_C);		CHECK_ERROR(err);

	size_t global_size = 16384;
	size_t local_size = 256;
	err = m2sEnqueueNDRangeKernel(queue, &device, kernel, 1, NULL, &global_size, &local_size, 0, NULL, NULL);	CHECK_ERROR(err);

	err = m2sEnqueueReadBuffer(queue, &device, buffer_C, M2S_TRUE, 0, sizeof(int) * 16384, C, 0, NULL, NULL);	CHECK_ERROR(err);

	int idx;

	for (idx = 0; idx < 16384; ++idx) {
		if (A[idx] + B[idx] != C[idx]) {
			printf("verification is failed\n");
			break;
		}
	}

	if (idx == 16384)
		printf("verification success\n");
}

int main()
{
	test(1);

	return 0;
}
