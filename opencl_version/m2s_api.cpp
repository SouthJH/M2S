#include "m2s.h"

/*
*	target API: clGetPlatformIDs
*	
*
*/
m2s_int m2sGetPlatformIDs(m2s_uint num_entries, m2s_platform_id *platforms, m2s_uint *num_platforms)
{
	if (num_entries == 0) {
		return clGetPlatformIDs(0, NULL, num_platforms);
	}
	else if (num_entries > 0) {
		return clGetPlatformIDs(num_entries, platforms, NULL);
	}
	else {
		printf("GetPlatformIDs ERROR: negative num_entries of platforms\n");
		return M2S_INVALID_NUM_ENTRIES;
	}
}

/*
*	target API: clGetDeviceIDs
*	
*
*/
m2s_int m2sGetDeviceIDs(m2s_platform_id platform, m2s_device_type device_type, m2s_uint num_entries, m2s_device_id *device, m2s_uint *num_devices)
{
	if (num_entries == 0) {
		return clGetDeviceIDs(platform, device_type, 0, NULL, num_devices);
	}
	else if (num_entries > 0) {
		device->initialize(num_entries);
		if (m2sGetDeviceHints(device) != CL_SUCCESS) {
			return M2S_INVALID_DEVICE_HINT;
		}
		return clGetDeviceIDs(platform, device_type, num_entries, device->devices, NULL);
	}
	else {
		printf("GetDeviceIDs ERROR: negative num_entries of devices\n");
		return M2S_INVALID_NUM_ENTRIES;
	}
}

/*
*	target API: clCreateContext
*
*
*/
m2s_context m2sCreateContext(const m2s_context_properties *properties, const m2s_device_id device, void *pfn_notify, void *user_data, m2s_int *errcode_ret)
{
	if (device.num_entries > 0) {
		return clCreateContext(properties, device.num_entries, device.devices, NULL, NULL, errcode_ret);
	}
	else {
		printf("CreateContext ERROR: negative num_entries of device\n");
		*errcode_ret = M2S_INVALID_NUM_ENTRIES;
		return NULL;
	}
}

/*
*	target API: clCreateCommandQueue
*
*
*/
m2s_command_queue m2sCreateCommandQueue(m2s_context context, m2s_device_id device, m2s_command_queue_properties properties, m2s_int *errcode_ret)
{
	m2s_command_queue queue;
	queue.initialize(device.num_entries);

	if (device.num_entries <= 0) {
		printf("CreateCommandQueue Error: wrong num_entries of device\n");
		return queue;
	}

	for (int idx = 0; idx < device.num_entries; ++idx)
	{
		queue.queues[idx] = clCreateCommandQueue(context, device.devices[idx], properties, errcode_ret);
		if (*errcode_ret != CL_SUCCESS) {
			printf("CreateCommandQueue ERROR: cannot create command queue\n");
			return queue;
		}
	}

	return queue;
}

/*
*	target API: clCreateProgramWithSource
*
*	exactly same function with clCreateProgramWithSource
*/
m2s_program m2sCreateProgramWithSource(m2s_context context, m2s_uint count, const char **strings, const size_t *lengths, m2s_int *errcode_ret) {
	return clCreateProgramWithSource(context, count, strings, lengths, errcode_ret);
}

/*
*	target API: clBuildProgram
*
*
*/
m2s_int m2sBuildProgram(m2s_program program, const m2s_device_id *device, const char *options, void *pfn_notify, void *user_data) {
	if (device->num_entries <= 0) {
		printf("BuildProgram Error: wrong num_entries of device\n");
		return M2S_INVALID_NUM_ENTRIES;
	}
	else {
		return clBuildProgram(program, device->num_entries, device->devices, options, NULL, NULL);
	}
}

/*
*	target API: clCreateKernel
*
*	exactly same function with clCreateKernel
*/
m2s_kernel m2sCreateKernel(m2s_program program, const char *kernel_name, m2s_int *errcode_ret) {
	return clCreateKernel(program, kernel_name, errcode_ret);
}

/*
*	target API: clCreateBuffer
*
*	need to set hint
*/
m2s_mem m2sCreateBuffer(m2s_context context, m2s_device_id *device, cl_uint mem_hint, m2s_mem_flags flags, size_t size, void *host_ptr, m2s_int *errcode_ret) {
	m2s_mem mem;

	if (device->num_entries <= 0) {
		printf("BuildProgram Error: wrong num_entries of device\n");
		*errcode_ret = M2S_INVALID_NUM_ENTRIES;
		mem.initialize(0);
		return mem;
	}

	mem.initialize(device->num_entries);
	mem.mem_hint = mem_hint;

	for (int idx = 0; idx < mem.num_entries; ++idx)
	{
		mem.mems[idx] = clCreateBuffer(context, flags, size, host_ptr, errcode_ret);
		if (*errcode_ret != CL_SUCCESS) {
			printf("CreateBuffer ERROR: cannot create buffer\n");
			return mem;
		}
	}

	return mem;
}

/*
*	target API: clEnqueueWriteBuffer
*	CL_TRUE case
*	event is not yet supported
*/
m2s_int m2sEnqueueWriteBuffer(m2s_command_queue command_queue, m2s_device_id *device, m2s_mem buffer, m2s_bool blocking_write, size_t offset, size_t size, const void *ptr, 
	m2s_uint num_events_in_wait_list, const m2s_event *event_wait_list, m2s_event *event)
{
	/*
	switch (buffer->mem_hint) {

	}
	*/
	cl_int err;

	// whole transfer version
	for (int idx = 0; idx < buffer.num_entries; ++idx)
	{
		err = clEnqueueWriteBuffer(command_queue.queues[idx], buffer.mems[idx], blocking_write, offset, size, ptr, num_events_in_wait_list, event_wait_list, event);

		if (err != CL_SUCCESS) {
			printf("EnqueueWriteBuffer Error: cannot write buffer\n");
			return err;
		}
	}

	return err;
}

/*
*	target API: clEnqueueReadBuffer
*
*	CL_TRUE case
*/
m2s_int m2sEnqueueReadBuffer(m2s_command_queue command_queue, m2s_device_id *device, m2s_mem buffer, m2s_bool blocking_write, size_t offset, size_t size, void *ptr,
	m2s_uint num_events_in_wait_list, const m2s_event *event_wait_list, m2s_event *event)
{
	/*
	switch (buffer->mem_hint) {

	}
	*/
	cl_int err;

	// whole transfer version
	for (int idx = 0; idx < buffer.num_entries; ++idx)
	{
		err = clEnqueueReadBuffer(command_queue.queues[idx], buffer.mems[idx], blocking_write, offset, size, ptr, num_events_in_wait_list, event_wait_list, event);

		if (err != CL_SUCCESS) {
			printf("EnqueueWriteBuffer Error: cannot write buffer\n");
			return err;
		}
	}

	return err;
}

/*
*	target API: clEnqueueCopyBuffer
*
*	for now, not supported
*/
m2s_int m2sEnqueueCopyBuffer(m2s_command_queue *command_queue, m2s_mem *src_buffer, m2s_mem *dst_buffer, size_t src_offset, size_t dst_offset, size_t size, m2s_uint num_events_in_wait_list,
	const m2s_event *event_wait_list, m2s_event *event)
{
	if (src_buffer->num_entries != dst_buffer->num_entries) {
		printf("EnqueueCopyBuffer Error: cannot transfer data between differnet size buffer\n");
		return M2S_TRANSFER_DATA_ERROR;
	}

	cl_int err;

	for (int idx = 0; idx < command_queue->num_entries; ++idx) {
		err = clEnqueueCopyBuffer(command_queue->queues[idx], src_buffer->mems[idx], dst_buffer->mems[idx], src_offset, dst_offset, size, num_events_in_wait_list, event_wait_list, event);

		if (err != CL_SUCCESS) {
			printf("EnqueueCopyBuffer Error: cannot write buffer\n");
			return err;
		}
	}

	return err;
}

/*
*	target API: clSetKernelArg
*
*	 
*/
m2s_int m2sSetKernelArg(m2s_kernel kernel, m2s_device_id *device, m2s_uint arg_index, size_t arg_size, m2s_mem *arg_value)
{
	if (device->num_entries <= 0) {
		printf("SetKernelArg Error: wrong num_entries of device\n");
		return M2S_INVALID_NUM_ENTRIES;
	}

	cl_int err;

	for (int idx = 0; idx < device->num_entries; ++idx) {
		err = clSetKernelArg(kernel, arg_index, sizeof(cl_mem), &arg_value->mems[idx]);

		if (err != CL_SUCCESS) {
			printf("SetKernelArg Error: cannot set arguement\n");
			return err;
		}
	}

	return err; 
}

/*
*	target API: EnqueueNDRangeKernel
*
*
*/
m2s_int m2sEnqueueNDRangeKernel(m2s_command_queue command_queue, m2s_kernel kernel, m2s_uint work_dim, const size_t *global_work_offset, const size_t *global_work_size,
	const size_t *local_work_size, m2s_uint num_events_in_wait_list, const m2s_event *event_wait_list, m2s_event *event)
{
	int num_entries = command_queue.num_entries;

	if (num_entries == 0) {
		printf("EnqueueNDRangeKernel Error: wrong num_entries of device\n");
		return M2S_INVALID_NUM_ENTRIES;
	}

	cl_int err;

	for (int idx = 0; idx < num_entries; ++idx)
	{
		err = clEnqueueNDRangeKernel(command_queue.queues[idx], kernel, work_dim, global_work_offset, global_work_size, local_work_size, num_events_in_wait_list, event_wait_list, event);

		if (err != CL_SUCCESS) {
			printf("EnqueueNDRangeKernel Error: cannot run kernel on device\n");
			return err;
		}
	}

	return err;
}

/*
*	target API: None
*
*	This API function is created only for hints of gpus
*/
m2s_int m2sGetDeviceHints(m2s_device_id *device)
{
	if (device->num_entries <= 0) {
		printf("GetDeviceHints Error: wrong num_entries of device\n");
		return M2S_INVALID_NUM_ENTRIES;
	}
	else if (device->devices == NULL) {
		printf("GetDeviceHints Error: m2s device is not set properly\n");
	}

	int n = device->num_entries;
	device->device_hint = (cl_uint *)malloc(sizeof(cl_uint) * n);

	cl_ulong *local_mem_size = (cl_ulong *)malloc(sizeof(cl_ulong) * n);
	cl_ulong *global_mem_size = (cl_ulong *)malloc(sizeof(cl_ulong) * n);
	size_t *max_work_group_size = (size_t *)malloc(sizeof(size_t) * n);
	cl_uint *max_compute_units = (cl_uint *)malloc(sizeof(cl_uint) * n);
	cl_ulong *max_mem_alloc_size = (cl_ulong *)malloc(sizeof(cl_ulong) * n);

	// load balance algorithm runs
	cl_uint sum = 0;
	for (int idx = 0; idx < n; ++idx) {
		device->device_hint[idx] = 100/n;
		sum += device->device_hint[idx];
	}

	if (sum < 100) {
		device->device_hint[0] += (100 - sum);
	}
	else if (sum > 100) {
		printf("GetDeviceHints Error: device hint is not valid\n");
		return M2S_INVALID_DEVICE_HINT;
	}

	free(local_mem_size);
	free(global_mem_size);
	free(max_work_group_size);
	free(max_compute_units);
	free(max_mem_alloc_size);

	return CL_SUCCESS;
}
