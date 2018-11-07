#pragma once
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

typedef struct M2S_MEM {
	cl_mem			mem;
	cl_uint			mem_hint;
	cl_mem_flags	flag;
	cl_uint			data_size;
	cl_uint			data_length;
}m2s_mem;

typedef struct M2S_DEViCE {
	std::vector<cl_device_id>	devices;
	std::vector<cl_uint>		device_ratio;
//	std::vector<m2s_mem>		buffers;
}m2s_device_id;

typedef struct M2S_QUEUE {
	std::vector<cl_command_queue>	queues;
	m2s_device_id					*device;
}m2s_command_queue;

#define		m2s_command_queue_properties	cl_command_queue_properties
#define		m2s_mem_flags					cl_mem_flags

// not supported
#define		m2s_context_properties		cl_context_properties
#define		m2s_sampler					cl_sampler