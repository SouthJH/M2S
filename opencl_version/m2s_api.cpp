#include "m2s_api.h"

/*
*	target API: clGetPlatformIDs
*
*	Supposed that we have single platform
*	for now, i use only NVIDIA GPU
*	therefore, the single platform will be nvidia opencl platform
*/
m2s_int m2sGetPlatformIDs(m2s_uint num_entries, m2s_platform_id *platforms, m2s_uint *num_platforms)
{
#ifdef	DEBUG_MODE
	printf("GetPlatformIDs \t\t\t");
#endif
	cl_int err;

	if (num_entries == 0) {
		err = clGetPlatformIDs(0, NULL, num_platforms);
		goto GetPlatformIDs_success;
	}
	else if (num_entries > 0) {
		err = clGetPlatformIDs(num_entries, platforms, NULL);
		goto GetPlatformIDs_success;
	}

#ifdef	DEBUG_MODE
	printf("M2S ERROR: negative num_entries of platforms\n");
#endif
	return M2S_INVALID_NUM_ENTRIES;

GetPlatformIDs_success:
#ifdef	DEBUG_MODE
	printf("done\n");
#endif
	return err;
}

/*
*	target API: clGetDeviceIDs
*
*	get the number of devices  or  get device ids
*	with these device ids, i will make context where kernel codes actually run
*/
m2s_int m2sGetDeviceIDs(m2s_platform_id platform, m2s_device_type device_type, m2s_uint num_entries, m2s_device_id *device, m2s_uint *num_devices)
{
#ifdef	DEBUG_MODE
	printf("GetDeviceIDs \t\t\t");
#endif
	cl_int err;

	if (num_entries == 0) {
		err = clGetDeviceIDs(platform, device_type, 0, NULL, num_devices);
		goto GetDeviceIDs_success;
	}
	else if (num_entries > 0)
	{
		cl_device_id	*temp = (cl_device_id*)malloc(sizeof(cl_device_id) * num_entries);
		if (temp == NULL) {
			free(temp);
			goto GetDeviceIDs_alloc_fail;
		}
		else {
			for (m2s_uint i = 0; i < num_entries; ++i)
				temp[i] = NULL;
		}

		err = clGetDeviceIDs(platform, device_type, num_entries, temp, NULL);
		if (err == CL_SUCCESS) {
			for (m2s_uint i = 0; i < num_entries; ++i) {
				if (temp[i] != NULL)
					device->devices.push_back(temp[i]);
			}
		}
		free(temp);
		goto GetDeviceIDs_success;
	}
	
#ifdef	DEBUG_MODE
	printf("M2S ERROR: negative num_entries of devices\n");
#endif
	return M2S_INVALID_NUM_ENTRIES;

GetDeviceIDs_alloc_fail:
#ifdef	DEBUG_MODE
	printf("M2S ERROR: not enough memory for temporary cl_devices\n");
#endif
	return M2S_MEMORY_ALLOC_FAILED;

GetDeviceIDs_success:
#ifdef DEBUG_MODE
	printf("done (found %zd devices)\n", device->devices.size());
#endif
	return err;
}

/*
*	target API: clCreateContext
*
*	create context for m2s system
*	any devices not included in the context cannot be used
*/
m2s_context m2sCreateContext(const m2s_context_properties *properties, const m2s_device_id device, void *pfn_notify, void *user_data, m2s_int *errcode_ret)
{
#ifdef	DEBUG_MODE
	printf("CreateContext \t\t\t");
#endif
	cl_context ret;

	if (!device.devices.empty()) {
		ret = clCreateContext(properties, device.devices.size(), &device.devices[0], NULL, NULL, errcode_ret);
		goto CreateContext_success;
	}
	
	*errcode_ret = M2S_INVALID_NUM_ENTRIES;

#ifdef	DEBUG_MODE
	printf("M2S ERROR: there is no device found\n");
#endif
	return NULL;

CreateContext_success:
#ifdef	DEBUG_MODE
	printf("done\n");
#endif
	return ret;
}

/*
*	target API: clCreateCommandQueue
*
*	we will control devices through this command queue
*	Data transfer, Run kernel, and so on
*	These commands are delivered
*/
m2s_command_queue m2sCreateCommandQueue(m2s_context context, m2s_device_id *device, m2s_command_queue_properties properties, m2s_int *errcode_ret)
{
#ifdef	DEBUG_MODE
	printf("CreateCommandQueue \t\t");
#endif

	m2s_command_queue queue;
	int len;

	if (device->devices.empty())
		goto CreateCommandQueue_wrong_entries;

	len = device->devices.size();
	for (int idx = 0; idx < len; ++idx) {
		cl_command_queue q = clCreateCommandQueue(context, device->devices[idx], properties, errcode_ret);
		if (*errcode_ret != CL_SUCCESS)
			goto CreateCommandQueue_wrong_queue;
		queue.queues.push_back(q);
	}
	queue.device = device;

#ifdef	DEBUG_MODE
	printf("done\n");
#endif
	return queue;

CreateCommandQueue_wrong_entries:
#ifdef	DEBUG_MODE
	printf("M2S Error: there is no found device\n");
#endif
	*errcode_ret = M2S_INVALID_NUM_ENTRIES;
	return queue;

CreateCommandQueue_wrong_queue:
#ifdef	DEBUG_MODE
	printf("M2S ERROR: cannot create command queue\n");
#endif
	*errcode_ret = M2S_CREATE_QUEUE_FAILED;
	return queue;
}

/*
*	target API: clCreateProgramWithSource
*
*	exactly same function with clCreateProgramWithSource
*/
m2s_program m2sCreateProgramWithSource(m2s_context context, m2s_uint count, const char **strings, const size_t *lengths, m2s_int *errcode_ret) {
#ifdef	DEBUG_MODE
	printf("CreateProgramWithSource \t");
#endif
	cl_program ret = clCreateProgramWithSource(context, count, strings, lengths, errcode_ret);
#ifdef	DEBUG_MODE
	printf("done\n");
#endif
	return ret;
}

/*
*	target API: clBuildProgram
*
*	Build program to run on devices
*/
m2s_int m2sBuildProgram(m2s_program program, const m2s_device_id *device, const char *options, void *pfn_notify, void *user_data) {
#ifdef	DEBUG_MODE
	printf("BuildProgram \t\t\t");
#endif
	cl_int err;

	if (device->devices.empty())
		goto BuildProgram_wrong_entries;
	err = clBuildProgram(program, device->devices.size(), &device->devices[0], options, NULL, NULL);

#ifdef	DEBUG_MODE
	printf("done\n");
#endif
	return err;

BuildProgram_wrong_entries:
#ifdef	DEBUG_MODE
	printf("M2S Error: there is no device found\n");
#endif
	return M2S_INVALID_NUM_ENTRIES;
}

/*
*	target API: clCreateKernel
*
*	exactly same function with clCreateKernel
*/
m2s_kernel m2sCreateKernel(m2s_program program, const char *kernel_name, m2s_int *errcode_ret) {
#ifdef	DEBUG_MODE
	printf("CreateKernel \t\t\t");
#endif
	cl_kernel ret = clCreateKernel(program, kernel_name, errcode_ret);
#ifdef	DEBUG_MODE
	printf("done\n");
#endif
	return ret;
}

/*
*	target API: clCreateBuffer
*
*	device hint is not created yet
*	
*	
*/
m2s_mem m2sCreateBuffer(m2s_context context, m2s_device_id *device, m2s_uint mem_hint, m2s_mem_flags flags, size_t size, size_t length, m2s_int *errcode_ret) {
#ifdef	DEBUG_MODE
	printf("CreateBuffer \t\t\t");
#endif
	m2s_mem mem;

	if (!(M2S_HINT_HEAD < mem_hint && mem_hint < M2S_HINT_TAIL))
		goto CreateBuffer_invalid_hint;
	if (size * length <= 0)
		goto CreateBuffer_invalid_size;

	mem.mem_hint = mem_hint;
	mem.data_size = size;
	mem.data_length = length;
	mem.flag = flags;

	mem.mem = clCreateBuffer(context, flags, size * length, NULL, errcode_ret);
	//printf("CreateBuffer(context, flag, %d, NULL, errcode_ret)\n", size * length);

	// device->buffers.push_back(mem);

#ifdef	DEBUG_MODE
	printf("done\n");
#endif
	return mem;

CreateBuffer_invalid_hint:
#ifdef	DEBUG_MODE
	printf("M2S Error: invalid hint is given\n");
#endif
	*errcode_ret = M2S_INVALID_DEVICE_HINT;
	mem.mem = NULL;
	return mem;

CreateBuffer_invalid_size:
#ifdef	DEBUG_MODE
	printf("M2S ERROR: cannot create buffer\n");
#endif
	*errcode_ret = M2S_CREATE_BUFFR_FAILED;
	mem.mem = NULL;
	return mem;
}

/*
*	target API: none
*
*	create device hint for later processes
*	this is critical part for m2s performance
*/
m2s_int m2sCreateDeviceHint(m2s_kernel kernel, m2s_device_id *device, size_t size) {
#ifdef	DEBUG_MODE
	printf("CreateDeviceHint \t\t");
#endif
	cl_ulong	*local_mem_used;
	cl_ulong	*private_mem_size;
	cl_ulong	*total_local_mem;
	cl_ulong	*max_mem_alloc_size;
	
	cl_uint		*ratio;

	cl_uint		sum = 0, cnt = 0, left = 0;
	cl_uint		n, idx;
	cl_int		errcode_ret;

	if (device->devices.empty())
		goto CreateDeviceHint_empty;
	else {
		device->device_ratio.clear();
		n = device->devices.size();
	}

	local_mem_used		= (cl_ulong *)malloc(sizeof(cl_ulong) * n);
	private_mem_size	= (cl_ulong *)malloc(sizeof(cl_ulong) * n);
	total_local_mem		= (cl_ulong *)malloc(sizeof(cl_ulong) * n);
	max_mem_alloc_size	= (cl_ulong *)malloc(sizeof(cl_ulong) * n);

	ratio				= (cl_uint *)malloc(sizeof(cl_uint) * n);

	/*
	*	get informations to make hint
	*
	*	KernelWorkGroupInfo ??????????
	*/
	for (idx = 0; idx < n; ++idx) {
		clGetDeviceInfo(device->devices[idx], CL_DEVICE_MAX_MEM_ALLOC_SIZE, sizeof(cl_ulong), &max_mem_alloc_size[idx], NULL);
		clGetDeviceInfo(device->devices[idx], CL_DEVICE_LOCAL_MEM_SIZE, sizeof(cl_ulong), &total_local_mem[idx], NULL);

		clGetKernelWorkGroupInfo(kernel, device->devices[idx], CL_KERNEL_LOCAL_MEM_SIZE, sizeof(cl_ulong), &local_mem_used[idx], NULL);
		clGetKernelWorkGroupInfo(kernel, device->devices[idx], CL_KERNEL_PRIVATE_MEM_SIZE, sizeof(cl_ulong), &private_mem_size[idx], NULL);

		//printf(":::: total local mem: %lu\n", toal_local_mem[idx]);
		//printf(":::: used  local mem: %lu\n", local_mem_used[idx]);

		//device->device_ratio.push_back(max_mem_alloc_size[idx]);
		cnt += max_mem_alloc_size[idx];
		sum += total_local_mem[idx];
	}

	// maximum ratio
	for (idx = 0; idx < n; ++idx) {
		device->device_ratio.push_back(100 * total_local_mem[idx] / sum);
		ratio[idx] = 100 * max_mem_alloc_size[idx] / cnt;
	}
	for (int idx = 0; idx < n - 1; ++idx) {
		if (device->device_ratio[idx] + left <= ratio[idx])
		{
			device->device_ratio[idx] += left;
			left = 0;
		}
		else
		{
			left = device->device_ratio[idx] + left - ratio[idx];
			device->device_ratio[idx] = ratio[idx];
		}
	}


	free(local_mem_used);
	free(private_mem_size);
	free(total_local_mem);
	free(ratio);
	free(max_mem_alloc_size);

#ifdef	DEBUG_MODE
	printf("done\n");
#endif
	return 0;


CreateDeviceHint_empty:
#ifdef	DEBUG_MODE
	printf("M2S Error: ");
#endif
	return M2S_INVALID_DEVICE_HINT;
}

/*
*	target API: clEnqueueWriteBuffer
*	CL_TRUE case
*	event is not yet supported
*/
m2s_int m2sEnqueueWriteBuffer(m2s_command_queue *command_queue, m2s_mem *buffer, m2s_bool blocking_write, size_t offset, size_t size, void *ptr,
	m2s_uint num_events_in_wait_list, const m2s_event *event_wait_list, m2s_event *event)
{
#ifdef	DEBUG_MODE
	printf("EnqueueWriteBuffer \t\t");
#endif

	m2s_device_id *device = command_queue->device;
	int num = device->devices.size();
	cl_int err;

	if (command_queue->queues.empty() || device->devices.empty())
		goto EnqueueWriteBuffer_wrong_entries;
	if (command_queue->queues.size() != device->devices.size())
		goto EnqueueWriteBuffer_mismatch;
	if (device->device_ratio.size() != device->devices.size())
		goto EnqueueWriteBuffer_invalid_hint;

	

	if (num == 1 || 
		buffer->mem_hint == M2S_HINT_1D_ALL || 
		buffer->mem_hint == M2S_HINT_2D_ALL || 
		buffer->mem_hint == M2S_HINT_3D_ALL || 
		buffer->mem_hint == M2S_HINT_1D_X || 
		buffer->mem_hint == M2S_HINT_1D_SORT)
	{
		for (int idx = 0; idx < num; ++idx) {
			err = clEnqueueWriteBuffer(command_queue->queues[idx], buffer->mem, CL_FALSE, 0, size, ptr, num_events_in_wait_list, event_wait_list, event);
			//printf("WriteBuffer(queue[%d], buffer, CL_FALSE, 0, %d, ptr, 0, NULL, NULL)\n", idx, size);
			if (err != CL_SUCCESS)
				goto EnqueueWriteBuffer_write_fail;
		}
	}

	else if (buffer->mem_hint == M2S_HINT_HEAD)
	{
		int idx;
		size_t current_size = 0;
		size_t total_size = 0;

		for (idx = 0; idx < num - 1; ++idx)
		{
			current_size = size * device->device_ratio[idx] / 100;

			if (current_size <= 0)
				continue;
			if (total_size + current_size > size)
				break;

			err = clEnqueueWriteBuffer(command_queue->queues[idx], buffer->mem, CL_FALSE, 0, size, ptr, 0, NULL, NULL);
			if (err != CL_SUCCESS)
				goto EnqueueWriteBuffer_write_fail;

			total_size += current_size;
		}

		if (total_size < size) 
		{
			current_size = size - total_size;

			err = clEnqueueWriteBuffer(command_queue->queues[idx], buffer->mem, CL_FALSE, 0, size, ptr, 0, NULL, NULL);

			if (err != CL_SUCCESS)
				goto EnqueueWriteBuffer_write_fail;
		}
	}

	if (blocking_write == CL_TRUE) {
		for (int idx = 0; idx < num; ++idx) {
			clFinish(command_queue->queues[idx]);
		}
	}

#ifdef	DEBUG_MODE
	printf("done\n");
#endif
	return CL_SUCCESS;

EnqueueWriteBuffer_wrong_entries:
#ifdef	DEBUG_MODE
	printf("M2S ERROR: there is no device to use\n");
#endif
	return M2S_INVALID_NUM_ENTRIES;

EnqueueWriteBuffer_mismatch:
#ifdef	DEBUG_MODE
	printf("M2S ERROR: device num and queue num are mismatched\n");
#endif
	return M2S_INVALID_NUM_ENTRIES;

EnqueueWriteBuffer_invalid_hint:
#ifdef	DEBUG_MODE
	printf("M2S ERROR: this is not valid device hint\n");
#endif
	return M2S_INVALID_DEVICE_HINT;

EnqueueWriteBuffer_write_fail:
#ifdef	DEBUG_MODE
	printf("Error: cannot write buffer\n");
#endif
	return err;
}

/*
*	target API: clEnqueueReadBuffer
*
*	CL_TRUE case
*/
m2s_int m2sEnqueueReadBuffer(m2s_command_queue *command_queue, m2s_mem *buffer, m2s_bool blocking_write, size_t offset, size_t size, void *ptr,
	m2s_uint num_events_in_wait_list, const m2s_event *event_wait_list, m2s_event *event)
{
#ifdef	DEBUG_MODE
	printf("EnqueueReadBuffer \t\t");
#endif

	m2s_device_id *device = command_queue->device;
	int num = device->devices.size();
	cl_int err;

	if (command_queue->queues.empty() || device->devices.empty())
		goto EnqueueReadBuffer_wrong_entries;
	if (command_queue->queues.size() != device->devices.size())
		goto EnqueueReadBuffer_mismatch;
	if (device->device_ratio.size() != device->devices.size())
		goto EnqueueReadBuffer_invalid_hint;



	if (num == 1 || buffer->mem_hint == M2S_HINT_1D_ALL || buffer->mem_hint == M2S_HINT_2D_ALL || buffer->mem_hint == M2S_HINT_3D_ALL) {
		for (int idx = 0; idx < num; ++idx) {
			err = clEnqueueReadBuffer(command_queue->queues[idx], buffer->mem, CL_FALSE, offset, size, ptr, num_events_in_wait_list, event_wait_list, event);
			if (err != CL_SUCCESS)
				goto EnqueueReadBuffer_read_fail;
		}
	}

	else if (buffer->mem_hint == M2S_HINT_1D_X)
	{
		/*
		size_t current_size = size / 2;
		size_t offset = size / 2;
		err = clEnqueueReadBuffer(command_queue->queues[0], buffer->mem, CL_TRUE, 0, size * sizeof(int) / 2, ptr, 0, NULL, NULL);
		if (err != CL_SUCCESS)
			goto EnqueueReadBuffer_read_fail;
		//printf("passed previous one\n");
		err = clEnqueueReadBuffer(command_queue->queues[1], buffer->mem, CL_TRUE, offset, size * sizeof(int) / 2, (int *)ptr + (size / 2), 0, NULL, NULL);
		if (err != CL_SUCCESS)
			goto EnqueueReadBuffer_read_fail;
		//*/

		//*
		int idx;
		size_t unit = buffer->data_size;
		size_t current_size = 0;
		size_t total_size = 0;

		for (idx = 0; idx < num - 1; ++idx)
		{
			current_size = ((size * device->device_ratio[idx] / 100) / 4) * 4;
			//current_size = size * sizeof(int) / 2;

			if (current_size == 0)
				continue;
			if (total_size + current_size > size)
				break;

			if (idx == 0)
				err = clEnqueueReadBuffer(command_queue->queues[idx], buffer->mem, CL_TRUE, 0, current_size, ptr, 0, NULL, NULL);
			else
				err = clEnqueueReadBuffer(command_queue->queues[idx], buffer->mem, CL_TRUE, total_size, current_size, (int *)ptr + (total_size / unit), 0, NULL, NULL);
			if (err != CL_SUCCESS)
				goto EnqueueReadBuffer_read_fail;

			total_size += current_size;
		}

		if (total_size < size)
		{
			current_size = size - total_size;
			//current_size = size * sizeof(int) / 2;

			err = clEnqueueReadBuffer(command_queue->queues[idx], buffer->mem, CL_TRUE, total_size, current_size, (int *)ptr + (total_size / unit), 0, NULL, NULL);

			if (err != CL_SUCCESS)
				goto EnqueueReadBuffer_read_fail;
		}
		else
			goto EnqueueReadBuffer_invalid_hint;
		//*/
	}

	if (blocking_write == CL_TRUE) {
		for (int idx = 0; idx < num; ++idx) {
			clFinish(command_queue->queues[idx]);
		}
	}

#ifdef	DEBUG_MODE
	printf("done\n");
#endif
	return CL_SUCCESS;

EnqueueReadBuffer_wrong_entries:
#ifdef	DEBUG_MODE
	printf("M2S ERROR: there is no device to use\n");
#endif
	return M2S_INVALID_NUM_ENTRIES;

EnqueueReadBuffer_mismatch:
#ifdef	DEBUG_MODE
	printf("M2S ERROR: device num and queue num are mismatched\n");
#endif
	return M2S_INVALID_NUM_ENTRIES;

EnqueueReadBuffer_invalid_hint:
#ifdef	DEBUG_MODE
	printf("M2S ERROR: this is not valid device hint\n");
#endif
	return M2S_INVALID_DEVICE_HINT;

EnqueueReadBuffer_read_fail:
#ifdef	DEBUG_MODE
	printf("Error: cannot read buffer\n");
#endif
	return err;
}

/*
*	target API: clSetKernelArg
*
*	Set argument of kernel
*/
m2s_int m2sSetKernelArg(m2s_kernel kernel, m2s_device_id *device, m2s_uint arg_index, size_t arg_size, m2s_mem *arg_value)
{
#ifdef	DEBUG_MODE
	printf("SetKernelArg %u \t\t\t", arg_index);
#endif
	if (device->devices.empty())
		goto SetKernelArg_wrong_entries;

	cl_int err;

	//for (m2s_uint idx = 0; idx < device->devices.size(); ++idx) {
		err = clSetKernelArg(kernel, arg_index, sizeof(cl_mem), &arg_value->mem);
		//printf("SetKernelArg(kernel, %d, sizeof(cl_mem), buffer)\n", arg_index);

		if (err != CL_SUCCESS)
			goto SetKernelArg_failed;
	//}

#ifdef	DEBUG_MODE
	printf("done\n");
#endif
	return err;

SetKernelArg_wrong_entries:
#ifdef	DEBUG_MODE
	printf("M2S Error: wrong num_entries of device\n");
#endif
	return M2S_INVALID_NUM_ENTRIES;

SetKernelArg_failed:
#ifdef	DEBUG_MODE
	printf("M2S Error: cannot set arguement <%d>\n", err);
#endif
	return err;
}

/*
*	target API: EnqueueNDRangeKernel
*
*
*/
m2s_int m2sEnqueueNDRangeKernel(m2s_command_queue *command_queue, m2s_kernel kernel, m2s_uint m2s_hint, const size_t *global_work_offset, const size_t *global_work_size,
	const size_t *local_work_size, m2s_uint num_events_in_wait_list, const m2s_event *event_wait_list, m2s_event *event)
{
#ifdef	DEBUG_MODE
	printf("EnqueueNDRangeKernel \t\t");
#endif

	int num_entries = command_queue->device->devices.size();
	m2s_device_id	*device = command_queue->device;
	cl_int err;


	if (device->devices.empty())
		goto EnqueueNDRangeKernel_wrong_entries;
	if (!(M2S_HINT_HEAD < m2s_hint && m2s_hint < M2S_HINT_TAIL))
		goto EnqueueNDRangeKernel_invalid_hint;
	if (device->device_ratio.empty())
		goto EnqueueNDRangeKernel_no_hint;


	// Run whole kernel on every GPUs
	if (m2s_hint == M2S_HINT_1D_ALL || (m2s_hint == M2S_HINT_1D_X && device->devices.size() == 1)) {
		for (int idx = 0; idx < num_entries; ++idx) {
			err = clEnqueueNDRangeKernel(command_queue->queues[idx], kernel, 1, NULL, global_work_size, NULL, num_events_in_wait_list, event_wait_list, event);
			if (err != CL_SUCCESS)
				goto EnqueueNDRangeKernel_kernel_fail;
		}
	}
	else if (m2s_hint == M2S_HINT_2D_ALL) {
		for (int idx = 0; idx < num_entries; ++idx) {
			err = clEnqueueNDRangeKernel(command_queue->queues[idx], kernel, 2, NULL, global_work_size, NULL, num_events_in_wait_list, event_wait_list, event);
			if (err != CL_SUCCESS)
				goto EnqueueNDRangeKernel_kernel_fail;
		}
	}
	else if (m2s_hint == M2S_HINT_3D_ALL) {
		for (int idx = 0; idx < num_entries; ++idx) {
			err = clEnqueueNDRangeKernel(command_queue->queues[idx], kernel, 3, NULL, global_work_size, NULL, num_events_in_wait_list, event_wait_list, event);
			if (err != CL_SUCCESS)
				goto EnqueueNDRangeKernel_kernel_fail;
		}
	}


	// Run a partial of kernel on a GPU
	else if (m2s_hint == M2S_HINT_1D_X) {
		/*
		size_t current_size = global_work_size[0] / 2;
		size_t offset = global_work_size[0] / 2;

		err = clEnqueueNDRangeKernel(command_queue->queues[0], kernel, 1, NULL, &current_size, NULL, 0, NULL, NULL);
		if (err != CL_SUCCESS)
			goto EnqueueNDRangeKernel_kernel_fail;
		err = clEnqueueNDRangeKernel(command_queue->queues[1], kernel, 1, &offset, &current_size, NULL, 0, NULL, NULL);
		if (err != CL_SUCCESS)
			goto EnqueueNDRangeKernel_kernel_fail;
		//*/

		//*
		int idx;
		size_t current_size = 0;
		size_t total_size = 0;

		for (idx = 0; idx < num_entries - 1; ++idx) {
			current_size = ((global_work_size[0] * device->device_ratio[idx] / 100) / 4) * 4;
			//if (current_size != global_work_size[0] / 2) printf("front is failed\n");

			if (idx == 0)
				err = clEnqueueNDRangeKernel(command_queue->queues[idx], kernel, 1, NULL, &current_size, NULL, num_events_in_wait_list, event_wait_list, event);
			else
				err = clEnqueueNDRangeKernel(command_queue->queues[idx], kernel, 1, &total_size, &current_size, NULL, num_events_in_wait_list, event_wait_list, event);

			if (err != CL_SUCCESS)
				goto EnqueueNDRangeKernel_kernel_fail;

			total_size += current_size;
		}

		if (global_work_size[0] > total_size) {
			current_size = global_work_size[0] - total_size;
			//if (current_size != global_work_size[0] / 2) printf("back is failed\n");

			err = clEnqueueNDRangeKernel(command_queue->queues[idx], kernel, 1, &total_size, &current_size, NULL, num_events_in_wait_list, event_wait_list, event);

			if (err != CL_SUCCESS)
				goto EnqueueNDRangeKernel_kernel_fail;

		}
		else
			goto EnqueueNDRangeKernel_invalid_hint;
		//*/
	}

#ifdef	DEBUG_MODE
	printf("done\n");
#endif
	return err;


EnqueueNDRangeKernel_wrong_entries:
#ifdef	DEBUG_MODE
	printf("M2S ERROR: wrong num_entries of device\n");
#endif
	return M2S_INVALID_NUM_ENTRIES;

EnqueueNDRangeKernel_no_hint:
#ifdef	DEBUG_MODE
	printf("M2S ERROR: cannot produce device hint\n");
#endif
	return M2S_INVALID_DEVICE_HINT;

EnqueueNDRangeKernel_kernel_fail:
#ifdef	DEBUG_MODE
	printf("M2S ERROR: cannot run kernel on device\n");
#endif
	return err;

EnqueueNDRangeKernel_invalid_hint:
#ifdef	DEBUG_MODE
	printf("M2S ERROR: invalid hint is given\n");
#endif
	return M2S_INVALID_DEVICE_HINT;
}