#include "m2s.h"
#include "constants.h"


m2s_int m2s_clGetPlatformIDs(m2s_uint num_entries,
                             m2s_platform_id *platforms, 
                             m2s_uint *num_platforms) 
{
  if (num_entries == 0) {
    return clGetPlatformIDs(0, NULL, num_platforms);
  }
  else if (num_entries > 0) {
    return clGetPlatformIDs(num_entries, platforms, NULL);
  }
  else {
    //printf("ERROR: wrong platform entries\n");
    return GET_PLATFORM_ERR
  }
}

m2s_int m2s_clGetDeviceIDs(m2s_platform_id platform, 
                           m2s_device_type device_type, 
                           m2s_uint num_entries, 
                           m2s_device_id *device, 
                           m2s_uint *num_devices) 
{
  if (num_entries == 0) {
    return clGetDeviceIDs(platform, device_type, 0, NULL, num_devices);
  }
  else if (num_entries > 0) {
    if (device->num_entries != 0) {
      printf("ERROR: already used deivce\n");
      return GET_DEVICE_ID_ERR;
    }
    
    device->num_entries = num_entries;
    device->device = (cl_device_id)malloc(sizeof(cl_device_id) * num_entries);
    if (device->device == NULL) {
      printf("ERROR: failed to malloc\n");
      return ALLOC_ERR;
    }
    
    return clGetDeviceIDs(platform, device_type, num_entries, device->device, NULL);
  }
  else {
    return GET_DEVICE_NUM_ERR
  }
}

// create context with all devices in a platform
int m2s_clCreateContext(m2s_cont *container) {
  cl_int err;
  container->context = clCreateContext(NULL, container->num_devices, container->device, NULL, NULL, &err);
  
  if (err == CL_SUCCESS)
    return SUCCESS;
  else
    return CREATE_CONTEXT_ERR;
}

// create single queue for each device
// I will implement double buffering later
int m2s_clCreateCommandQueue(m2s_cont *container) {
  cl_int err;
  
  container->queue = (cl_command_queue *)malloc(sizeof(cl_command_queue) * container->num_devices);
  if (container->queue == NULL) {
    printf("malloc failed\n");
    return ALLOC_ERR;
  }
  
  for (int idx = 0; idx < container->num_devices; ++idx) {
    container->queue[idx] = clCreateCommandQueue(container->context, container->device[idx], 0, &err);
    if (err != CL_SUCCESS)
      return CREATE_QUEUE_ERR;
  }
  return SUCCESS;
}

/*
typedef struct container {
  cl_platform_id    platform;
  cl_device_id      *device;
  cl_context        context;
  cl_command_queue  *queue;
  cl_program        program;
  cl_kernel         kernel;
  
  cl_uint           num_devices; // -
} m2s_cont;
*/

int m2s_clCreateProgramWithSource(m2s_cont *container, ) {
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
