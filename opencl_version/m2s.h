

#include <stdio.h>
#include <stdlib.h>

#include <CL/cl.h>

// support only single platform
typedef struct container {
  cl_platform_id    platform;
  cl_device_id      device[10];
  cl_context        context;
  cl_command_queue  queue[10];
  cl_kernel         kernel;
} cont;


// opencl_api.c
int m2s_clGetPlatformIDs();
int m2s_clGetDeviceIDs();
int m2s_clCreateContext();
int m2s_clCreateCommandQueue();
int m2s_clCreateKernel();
int m2s_clCreateBuffer();
int m2s_clEnqueueWriteBuffer();
int m2s_clEnqueueReadBuffer();
int m2s_clEnqueueCopyBuffer();
int m2s_clSetKernelArg();
int m2s_EnqueueNDRangeKernel();

int m2s_clGetEventProfilingInfo();
