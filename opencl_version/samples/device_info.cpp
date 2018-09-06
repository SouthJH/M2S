#include <CL/cl.h>

#define   CHECK_ERROR(err) \
  if (err != CL_SUCCESS) { \
     printf("[%s:%d] ERROR %d\n", __FILE__, __LINE__, err); \
     exit(EXIT_FAILURE); \
  }

void printType(cl_device_type type) {
	printf("\t  - Type \t\t");

	if (type & CL_DEVICE_TYPE_CPU)
		printf("CPU");

	if (type & CL_DEVICE_TYPE_GPU)
		printf("GPU");

	if (type & CL_DEVICE_TYPE_ACCELERATOR)
		printf("ACCELERATOR");

	if (type & CL_DEVICE_TYPE_DEFAULT)
		printf("DEFAULT");

	if (type & CL_DEVICE_TYPE_CUSTOM)
		printf("CUSTOM");

	printf("\n");
}

int main()
{
	cl_platform_id		*platforms;
	cl_device_id		**devices;

	cl_int err;

	cl_uint num_platforms;
	err = clGetPlatformIDs(0, NULL, &num_platforms);			CHECK_ERROR(err);	printf("Number of platforms: %u\n", num_platforms);

	platforms = (cl_platform_id *)malloc(sizeof(cl_platform_id) * num_platforms);
	devices = (cl_device_id **)malloc(sizeof(cl_device_id *) * num_platforms);

	err = clGetPlatformIDs(num_platforms, platforms, NULL);		CHECK_ERROR(err);

	for (cl_uint idx = 0; idx < num_platforms; ++idx)
	{
		printf("\nplatform #%u\n", idx);
		
		//size_t name_size;
		//char *name;
		char name[1024];

		//err = clGetPlatformInfo(platforms[idx], CL_PLATFORM_NAME, 0, NULL, &name_size);		CHECK_ERROR(err);
		//name = (char *)malloc(sizeof(char) * name_size);
		err = clGetPlatformInfo(platforms[idx], CL_PLATFORM_NAME, 1024, name, NULL);	CHECK_ERROR(err);	printf(" - Name \t\t%s\n", name);
		err = clGetPlatformInfo(platforms[idx], CL_PLATFORM_VENDOR, 1024, name, NULL);	CHECK_ERROR(err);	printf(" - Vendor \t\t%s\n", name);
		err = clGetPlatformInfo(platforms[idx], CL_PLATFORM_VERSION, 1024, name, NULL); CHECK_ERROR(err);	printf(" - Version \t\t%s\n", name);

		cl_uint num_devices;
		err = clGetDeviceIDs(platforms[idx], CL_DEVICE_TYPE_ALL, 0, NULL, &num_devices);			CHECK_ERROR(err);	printf(" - Number of devices \t%u\n", num_devices);
		devices[idx] = (cl_device_id *)malloc(sizeof(cl_device_id) * num_devices);
		err = clGetDeviceIDs(platforms[idx], CL_DEVICE_TYPE_ALL, num_devices, devices[idx], NULL);	CHECK_ERROR(err);
		for (cl_uint jdx = 0; jdx < num_devices; ++jdx)
		{
			printf("\t device #%u\n", jdx);

			cl_device_type type;
			cl_uint cu;
			cl_ulong mem;
			size_t wg;

			err = clGetDeviceInfo(devices[idx][jdx], CL_DEVICE_NAME, 1024, name, NULL);								CHECK_ERROR(err);	printf("\t  - Name \t\t%s\n", name);
			err = clGetDeviceInfo(devices[idx][jdx], CL_DEVICE_TYPE, sizeof(cl_device_type), &type, NULL);			CHECK_ERROR(err);	printType(type);
			err = clGetDeviceInfo(devices[idx][jdx], CL_DEVICE_VENDOR, 1024, name, NULL);							CHECK_ERROR(err);	printf("\t  - Vendor \t\t%s\n", name);
			err = clGetDeviceInfo(devices[idx][jdx], CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(cl_uint), &cu, NULL);		CHECK_ERROR(err);	printf("\t  - Max CU's # \t\t%u\n", cu);
			err = clGetDeviceInfo(devices[idx][jdx], CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(size_t), &wg, NULL);		CHECK_ERROR(err);	printf("\t  - Max Workgroup Size \t%zu\n", wg);
			err = clGetDeviceInfo(devices[idx][jdx], CL_DEVICE_GLOBAL_MEM_SIZE, sizeof(cl_ulong), &mem, NULL);		CHECK_ERROR(err);	printf("\t  - Global Mem Size \t%lu\n", mem);
			err = clGetDeviceInfo(devices[idx][jdx], CL_DEVICE_LOCAL_MEM_SIZE, sizeof(cl_ulong), &mem, NULL);		CHECK_ERROR(err);	printf("\t  - local Mem Size \t%lu\n", mem);
			err = clGetDeviceInfo(devices[idx][jdx], CL_DEVICE_MAX_MEM_ALLOC_SIZE, sizeof(cl_ulong), &mem, NULL); 	CHECK_ERROR(err);	printf("\t  - Max Mem Alloc Size \t%lu\n", mem);
		}
	}

	return 0;
}
