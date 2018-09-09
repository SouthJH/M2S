
#include <stdio.h>
#include <stdlib.h>

#include <CL/opencl.h>

// variables
#define   m2s_int           cl_int
#define   m2s_uint          cl_uint
#define   m2s_bool          cl_bool
#define   m2s_device_type   cl_device_type
#define   m2s_platform_id   cl_platform_id
#define   m2s_context       cl_context
#define   m2s_program       cl_program
#define   m2s_kernel        cl_kernel
#define   m2s_event         cl_event
typedef struct M2S_DEViCE {
  cl_uint       num_entries;
  cl_device_id  *device;
}m2s_device_id;
typedef struct M2S_QUEUE {
  cl_uint           num_entries;
  cl_command_queue  *queue;
}m2s_command_queue;
typedef struct M2S_MEM {
  cl_uint num_entries;
  cl_mem  *mem;
}m2s_mem;
#define   m2s_command_queue_properties  cl_command_queue_properties
#define   m2s_mem_flags   cl_mem_flags

// not supported
#define   m2s_context_properties  cl_context_properties


// opencl_api.c
m2s_int m2s_clGetPlatformIDs(m2s_uint num_entries,
                             m2s_platform_id *platforms, 
                             m2s_uint *num_platforms);
m2s_int m2s_clGetDeviceIDs(m2s_platform_id platform, 
                           m2s_device_type device_type, 
                           m2s_uint num_entries, 
                           m2s_device_id *device, 
                           m2s_uint *num_devices);
m2s_context m2s_clCreateContext(const m2s_context_properties *properties, 
                                m2s_uint num_devices, 
                                const m2s_device_id device, 
                                void *pfn_notify, 
                                void *user_data, 
                                m2s_int *errcode_ret);
m2s_command_queue m2s_clCreateCommandQueue(m2s_context context, 
                                           m2s_device_id device, 
                                           m2s_command_queue_properties properties, 
                                           m2s_int *errcode_ret);
m2s_program m2s_clCreateProgramWithSource(m2s_context context, 
                                          m2s_uint count, 
                                          const char **strings, 
                                          const size_t *lengths, 
                                          m2s_int *errcode_ret);
m2s_kernel m2s_clCreateKernel(m2s_program program, 
                              const char *kernel_name, 
                              m2s_int *errcode_ret);
m2s_mem m2s_clCreateBuffer(m2s_context context, 
                           m2s_mem_flags flags, 
                           size_t size, 
                           void *host_ptr, 
                           m2s_int *errcode_ret);
m2s_int m2s_clEnqueueWriteBuffer(m2s_command_queue command_queue, 
                                 m2s_mem buffer, 
                                 m2s_bool blocking_write, 
                                 size_t offset, 
                                 size_t size, 
                                 const void *ptr, 
                                 m2s_uint num_events_in_wait_list, 
                                 const m2s_event *event_wait_list, 
                                 m2s_event *event);
m2s_int m2s_clEnqueueReadBuffer(m2s_command_queue command_queue, 
                                m2s_mem buffer, 
                                m2s_bool blocking_write, 
                                size_t offset, 
                                size_t size, 
                                const void *ptr, 
                                m2s_uint num_events_in_wait_list, 
                                const m2s_event *event_wait_list, 
                                m2s_event *event);
m2s_int m2s_clEnqueueCopyBuffer(m2s_command_queue command_queue, 
                                m2s_mem src_buffer, 
                                m2s_mem dst_buffer, 
                                size_t src_offset, 
                                size_t dst_offset, 
                                size_t size, 
                                m2s_uint num_events_in_wait_list, 
                                m2s_event *event);
m2s_int m2s_clSetKernelArg(m2s_kernel kernel, 
                           m2s_uint arg_index, 
                           size_t arg_size, 
                           const void *arg_value);
m2s_int m2s_EnqueueNDRangeKernel(m2s_command_queue command_queue, 
                                 m2s_kernel kernel, 
                                 m2s_uint work_dim, 
                                 const size_t *global_work_offset, 
                                 const size_t *global_work_size, 
                                 const size_t *local_work_size, 
                                 m2s_uint num_events_in_wait_list, 
                                 const m2s_event *event_wait_list, 
                                 m2s_event *event);

