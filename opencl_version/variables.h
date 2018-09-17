#include <CL/cl.h>

// variable types
#define		m2s_char			cl_char
#define		m2s_uchar			cl_uchar
#define		m2s_short			cl_short
#define		m2s_ushort			cl_ushort
#define		m2s_int				cl_int
#define		m2s_uint			cl_uint
#define		m2s_long			cl_long
#define		m2s_ulong			cl_ulong
#define		m2s_half			cl_half
#define		m2s_float			cl_float
#define		m2s_double			cl_double
#define		m2s_bool			cl_bool
#define		m2s_device_type		cl_device_type

// -------------- test code
typedef struct M2S_DEVICE_HINT {
	cl_uint 	num_entries;
	cl_uint 	*ratio;
}hint;
// -----------------------

// data structures
#define		m2s_platform_id		cl_platform_id
#define		m2s_context			cl_context
#define		m2s_program			cl_program
#define		m2s_kernel			cl_kernel
#define		m2s_event			cl_event
typedef struct M2S_DEViCE {
	cl_uint       num_entries;
	cl_device_id  *devices;
	// hit

	int initialize(int num) {
		if (num <= 0)
			return 0;
		num_entries = num;
		devices = (cl_device_id *)malloc(sizeof(cl_device_id) * num);
		if (devices == NULL) {
			return 1;
		}
		return 0;
	}
	int finalize() {
		num_entries = 0;
		free(devices);
		return 0;
	}
}m2s_device_id;
typedef struct M2S_QUEUE {
	cl_uint				num_entries;
	cl_command_queue	*queues;
	//m2s_device_id		*device;

	int initialize(int num) {
		if (num <= 0)
			return 0;
		num_entries = num;
		queues = (cl_command_queue *)malloc(sizeof(cl_command_queue) * num);
		if (queues == NULL) {
			return 1;
		}
		return 0;
	}
	int finalize() {
		num_entries = 0;
		free(queues);
		return 0;
	}
}m2s_command_queue;
typedef struct M2S_MEM {
	cl_uint num_entries;
	cl_mem  *mems;
	
	// hint
	cl_uint mem_hint;

	int initialize(int num) {
		if (num <= 0)
			return 0;
		num_entries = num;
		mems = (cl_mem *)malloc(sizeof(cl_mem) * num);
		if (mems == NULL) {
			return 1;
		}
		return 0;
	}
	int finalize() {
		num_entries = 0;
		free(mems);
		return 0;
	}
}m2s_mem;
#define		m2s_command_queue_properties	cl_command_queue_properties
#define		m2s_mem_flags					cl_mem_flags

// not supported
#define		m2s_context_properties		cl_context_properties
#define		m2s_sampler					cl_sampler



// parameters
#define		M2S_MEM_SIZE					sizeof(cl_mem)

#define		M2S_TRUE						CL_TRUE
#define		M2S_FALSE						CL_FALSE
#define		M2S_BLOCKING					CL_BLOCKING
#define		M2S_NON_BLOCKING				CL_NON_BLOCKING

#define		M2S_MEM_READ_WRITE				CL_MEM_READ_WRITE
#define		M2S_MEM_READ_ONLY				CL_MEM_READ_ONLY
#define		M2S_MEM_WRITE_ONLY				CL_MEM_WRITE_ONLY
#define		M2S_MEM_COPY_HOST_PTR			CL_MEM_COPY_HOST_PTR

#define		M2S_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE		CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE

#define		M2S_PLATFORM_NAME				CL_PLATFORM_NAME
#define		M2S_PLATFORM_VENDOR				CL_PLATFORM_VENDOR
#define		M2S_PLATFORM_VERSION			CL_PLATFORM_VERSION
#define		M2S_PLATFORM_PROFILE			CL_PLATFORM_PROFILE
#define		M2S_PLATFORM_EXTENSIONS			CL_PLATFORM_EXTENSIONS
#define		M2S_DEVICE_TYPE_ALL				CL_DEVICE_TYPE_ALL
#define		M2S_DEVICE_TYPE_GPU				CL_DEVICE_TYPE_GPU
#define		M2S_DEVICE_TYPE_CPU				CL_DEVICE_TYPE_CPU
#define		M2S_DEVICE_TYPE_ACCELERATOR		CL_DEVICE_TYPE_ACCELERATOR
#define		M2S_DEVICE_TYPE_CUSTOM			CL_DEVICE_TYPE_CUSTOM
#define		M2S_DEVICE_TYPE_DEFAULT			CL_DEVICE_TYPE_DEFAULT
