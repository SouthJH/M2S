#include <stdio.h>
#include <stdlib.h>

#include "variables.h"

// supposed that single platform & context

// opencl_api.c
extern m2s_int m2sGetPlatformIDs(m2s_uint num_entries,
								 m2s_platform_id *platforms,
								 m2s_uint *num_platforms);
extern m2s_int m2sGetDeviceIDs(m2s_platform_id platform,
							   m2s_device_type device_type,
							   m2s_uint num_entries,
							   m2s_device_id *device,
							   m2s_uint *num_devices);
extern m2s_context m2sCreateContext(const m2s_context_properties *properties,
									const m2s_device_id device,
									void *pfn_notify,
									void *user_data,
									m2s_int *errcode_ret);
extern m2s_command_queue m2sCreateCommandQueue(m2s_context context,
											   m2s_device_id device,
											   m2s_command_queue_properties properties,
											   m2s_int *errcode_ret);
extern m2s_program m2sCreateProgramWithSource(m2s_context context,
											  m2s_uint count,
											  const char **strings,
											  const size_t *lengths,
											  m2s_int *errcode_ret);
extern m2s_int m2sBuildProgram(m2s_program program,
							   const m2s_device_id *device, 
							   const char *options, 
							   void *pfn_notify, 
							   void *user_data);
extern m2s_kernel m2sCreateKernel(m2s_program program,
								  const char *kernel_name,
								  m2s_int *errcode_ret);
extern m2s_mem m2sCreateBuffer(m2s_context context,
							   m2s_device_id *device,
							   cl_uint mem_hint,
							   m2s_mem_flags flags,
							   size_t size,
							   void *host_ptr,
							   m2s_int *errcode_ret);
extern m2s_int m2sEnqueueWriteBuffer(m2s_command_queue command_queue,
									 m2s_device_id *device,
									 m2s_mem buffer,
									 m2s_bool blocking_write,
									 size_t offset,
									 size_t size,
									 void *ptr,
									 m2s_uint num_events_in_wait_list,
									 const m2s_event *event_wait_list,
									 m2s_event *event);
extern m2s_int m2sEnqueueReadBuffer(m2s_command_queue command_queue,
									m2s_device_id *device,
									m2s_mem buffer,
									m2s_bool blocking_write,
									size_t offset,
									size_t size,
									void *ptr,
									m2s_uint num_events_in_wait_list,
									const m2s_event *event_wait_list,
									m2s_event *event);
extern m2s_int m2sEnqueueCopyBuffer(m2s_command_queue command_queue,
									m2s_mem *src_buffer,
									m2s_mem *dst_buffer,
									size_t src_offset,
									size_t dst_offset,
									size_t size,
									m2s_uint num_events_in_wait_list,
									const m2s_event *event_wait_list,
									m2s_event *event);
extern m2s_int m2sSetKernelArg(m2s_kernel kernel,
							   m2s_device_id *device, 
							   m2s_uint arg_index,
							   size_t arg_size,
							   m2s_mem *arg_value);
extern m2s_int m2sEnqueueNDRangeKernel(m2s_command_queue command_queue,
									   m2s_device_id *device,
									   m2s_kernel kernel,
									   m2s_uint work_dim,
									   const size_t *global_work_offset,
									   const size_t *global_work_size,
									   const size_t *local_work_size,
									   m2s_uint num_events_in_wait_list,
									   const m2s_event *event_wait_list,
									   m2s_event *event);

extern m2s_int m2sFinish(m2s_command_queue command_queue);
extern m2s_int m2sWaitForEvents(m2s_uint num_events,
								const m2s_event *event_list);


extern m2s_int m2sReleaseMemObject();
extern m2s_int m2sReleaseCommandQueue();

extern m2s_int m2sGetDeviceHints(m2s_device_id *device);


// only GPU
extern m2s_device_id m2sMakeDeviceID(m2s_uint num_entries, cl_device_id *devices);
