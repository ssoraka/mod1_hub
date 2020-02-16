/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cl_struct.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/10 20:59:39 by ssoraka           #+#    #+#             */
/*   Updated: 2019/04/19 13:13:30 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_CL_STRUCT_H
# define FT_CL_STRUCT_H

#include "ft_mod1.h"

typedef enum	e_buff
{
	PARTS,
	CELLS,
	BUFFER_COUNT
}				t_buff;

typedef struct			s_open_cl
{
	cl_platform_id		platform;
	cl_uint				num_platform;
	cl_device_type		device_type;
	cl_uint				num_devices;
	cl_device_id		device;
	cl_context			context;
	cl_command_queue	queue;
	cl_mem				buffer[BUFFER_COUNT + 1];
	cl_program			program[PROGRAMS_COUNT + 1];
	cl_kernel			kernel[PROGRAMS_COUNT + 1];
	cl_int				errcode_ret;
}						t_open_cl;

#endif
