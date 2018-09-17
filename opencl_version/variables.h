
#include "constants.h"

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


// data structures
#define		m2s_platform_id		cl_platform_id
#define		m2s_context			cl_context
#define		m2s_program			cl_program
#define		m2s_kernel			cl_kernel
#define		m2s_event			cl_event
typedef struct M2S_DEViCE {
	cl_uint			num_entries;
	cl_device_id	*devices;
	// device hint ratio
	cl_uint			*device_hint;

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
	cl_uint			num_entries;
	cl_mem			*mems;
	cl_uint			mem_hint;

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
