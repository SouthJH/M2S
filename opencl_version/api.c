#include "m2s.h"
#include "constants.h"

// support only singel platform
int m2s_clGetPlatformIDs(m2s_cont *container) {
  if (clGetPlatformIDs(1, &container->platform, NULL) == CL_SUCCESS)
    return SUCCESS;
  else
    return GET_PLATFORM_ERR;
}

// support DEVICE TYPE: GPU
// 
int m2s_clGetDeviceIDs(m2s_cont *container) {
  
}

int m2s_clCreateContext() {
}

int m2s_clCreateCommandQueue() {
}

int m2s_clCreateKernel() {
}

int m2s_clCreateBuffer() {
}

int m2s_clEnqueueWriteBuffer() {
}

int m2s_clEnqueueReadBuffer() {
}

int m2s_clEnqueueCopyBuffer() {
}

int m2s_clSetKernelArg() {
}

int m2s_EnqueueNDRangeKernel() {
}
