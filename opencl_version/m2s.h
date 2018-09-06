

#include <stdio.h>
#include <stdlib.h>

#include <CL/cl.h>

// support only single platform
typedef struct container {
  cl_platform_id    platform;
  cl_device_id      *device;
  cl_context        context;
  cl_command_queue  *queue;
  cl_kernel         kernel;
} m2s_cont;


// opencl_api.c
int m2s_clGetPlatformIDs(m2s_cont *container);
int m2s_clGetDeviceIDs(m2s_cont *container);
int m2s_clCreateContext(m2s_cont *container);
int m2s_clCreateCommandQueue(m2s_cont *container);
int m2s_clCreateProgramWithSource(m2s_cont *container);
int m2s_clCreateKernel(m2s_cont *container);
int m2s_clCreateBuffer(m2s_cont *container);
int m2s_clEnqueueWriteBuffer(m2s_cont *container);
int m2s_clEnqueueReadBuffer(m2s_cont *container);
int m2s_clEnqueueCopyBuffer(m2s_cont *container);
int m2s_clSetKernelArg(m2s_cont *container);
int m2s_EnqueueNDRangeKernel(m2s_cont *container);

int m2s_clGetEventProfilingInfo(m2s_cont *container);
