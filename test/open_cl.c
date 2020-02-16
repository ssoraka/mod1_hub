/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_cl->c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/16 15:49:41 by ssoraka           #+#    #+#             */
/*   Updated: 2020/02/16 15:49:42 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_mod1.h"

void	pfn_notify(const char *errinfo, const void *private_info, size_t cb, void *user_data)
{
	printf("!!!%s!!!\n", errinfo);
}

void	pfn_notify2(cl_program program, void *user_data)
{
	printf("!!!Что-то не так с программой!!!\n");
}

void	ft_context_error(cl_int errcode_ret)
{
	if (errcode_ret == CL_INVALID_PLATFORM)
		ft_putendl("CL_INVALID_PLATFORM");
	if (errcode_ret == CL_INVALID_PROPERTY)
		ft_putendl("CL_INVALID_PROPERTY");
	if (errcode_ret == CL_INVALID_VALUE)
		ft_putendl("CL_INVALID_VALUE");
	if (errcode_ret == CL_INVALID_DEVICE)
		ft_putendl("CL_INVALID_DEVICE");
	if (errcode_ret == CL_DEVICE_NOT_AVAILABLE)
		ft_putendl("CL_DEVICE_NOT_AVAILABLE");
	if (errcode_ret == CL_OUT_OF_RESOURCES)
		ft_putendl("CL_OUT_OF_RESOURCES");
	if (errcode_ret == CL_OUT_OF_HOST_MEMORY)
		ft_putendl("CL_OUT_OF_HOST_MEMORY");
}

void	ft_queue_error(cl_int errcode_ret)
{
	if (errcode_ret == CL_INVALID_CONTEXT)
		ft_putendl("CL_INVALID_CONTEXT");
	if (errcode_ret == CL_INVALID_DEVICE)
		ft_putendl("CL_INVALID_DEVICE");
	if (errcode_ret == CL_INVALID_VALUE)
		ft_putendl("CL_INVALID_VALUE");
	if (errcode_ret == CL_INVALID_QUEUE_PROPERTIES)
		ft_putendl("CL_INVALID_QUEUE_PROPERTIES");
	if (errcode_ret == CL_OUT_OF_RESOURCES)
		ft_putendl("CL_OUT_OF_RESOURCES");
	if (errcode_ret == CL_OUT_OF_HOST_MEMORY)
		ft_putendl("CL_OUT_OF_HOST_MEMORY");
}

t_open_cl	*ft_init_open_cl(void)
{
	t_open_cl *cl;

	if (!(cl = ft_memalloc(sizeof(t_open_cl))))
		ft_error_and_exit(cl, "malloc error!!!\n");
	if (clGetPlatformIDs(1, &cl->platform, &cl->num_platform) != CL_SUCCESS)
		ft_error_and_exit(cl, "platform error!!!\n");
	cl->device_type = CL_DEVICE_TYPE_GPU;
	if (clGetDeviceIDs(cl->platform, cl->device_type, 1, &cl->device, &cl->num_devices) != CL_SUCCESS)
		ft_error_and_exit(cl, "device error!!!\n");
	cl->context = clCreateContext(NULL, cl->num_devices, &cl->device, &pfn_notify, NULL, &cl->errcode_ret);
	if (cl->errcode_ret != CL_SUCCESS)
	{
		ft_context_error(cl->errcode_ret);
		ft_error_and_exit(cl, "context error!!!\n");
	}
	cl->queue = clCreateCommandQueue(cl->context, cl->device, 0, &cl->errcode_ret);
	if (cl->errcode_ret != CL_SUCCESS)
	{
		ft_queue_error(cl->errcode_ret);
		ft_error_and_exit(cl, "queue error!!!\n");
	}
	return (cl);
}

int		ft_read_and_build_programs(t_open_cl *cl)
{
	char buffer[PROGRAMM_SIZE];
	char *arr[2];
	size_t lengths[1];
	int i;

	arr[1] = NULL;
	i = 0;
	while (i < PROGRAMS_COUNT)
	{
		if (!is_read_programm((char *)buffer, g_names[i]))
			return (FALSE);
		lengths[0] = ft_strlen(buffer);
		arr[0] = (char *)&buffer;
		cl->program[i] = clCreateProgramWithSource(cl->context, 1, (const char **)arr, lengths, &cl->errcode_ret);
		if (cl->errcode_ret != CL_SUCCESS)
			return (FALSE);
		if (clBuildProgram(cl->program[i], cl->num_devices, &cl->device, NULL, NULL, NULL) != CL_SUCCESS)
			return (FALSE);
		cl->kernel[i] = clCreateKernel(cl->program[i], g_kernel[i], &cl->errcode_ret);
		if (cl->errcode_ret != CL_SUCCESS)
			return (FALSE);
		i++;
	}
	return (TRUE);
}


int		ft_read_buffers(t_open_cl *cl, int num, void *dest, size_t size)
{
	if (clEnqueueReadBuffer(cl->queue, cl->buffer[num], CL_FALSE, 0, size, dest
	, 0, NULL, NULL) != CL_SUCCESS)
		return (FALSE);
	return (TRUE);
}

int		ft_create_buffers(t_open_cl *cl, int num, void *src, size_t size)
{
	cl->buffer[num] = clCreateBuffer(cl->context, CL_MEM_READ_WRITE, size, NULL
	, &(cl->errcode_ret));
	if (cl->errcode_ret != CL_SUCCESS)
		return (FALSE);
	if (clEnqueueWriteBuffer(cl->queue, cl->buffer[num], CL_TRUE, 0, size, src
	, 0, NULL, NULL) != CL_SUCCESS)
		return (FALSE);
	return (TRUE);
}


int		ft_set_kernel_arg(t_open_cl *cl)
{
	if (clSetKernelArg(cl->kernel[0], 0, sizeof(cl_mem), (void *)&(cl->buffer[PARTS])) != CL_SUCCESS)
		return (FALSE);
	if (clSetKernelArg(cl->kernel[1], 0, sizeof(cl_mem), (void *)&(cl->buffer[CELLS])) != CL_SUCCESS)
		return (FALSE);
	return (TRUE);
}


int		ft_run_kernels(t_open_cl *cl)
{
	int i;
	size_t global_work_size[1];

	global_work_size[0] = 0;
	i = 0;
	while (i < PROGRAMS_COUNT)
	{
		if (i == CELLS)
			global_work_size[0] = CELL_COUNT;
		if (i == PARTS)
			global_work_size[0] = PART_COUNT;
		if (clEnqueueNDRangeKernel(cl->queue, cl->kernel[i], 1, NULL,
		global_work_size, NULL, 0, NULL, NULL) != CL_SUCCESS)
			return (FALSE);
		clFinish(cl->queue);
		i++;
	}
	return (TRUE);
}

void	ft_free_open_cl(t_open_cl **open_cl)
{
	t_open_cl *cl;
	int i;

	if (!(cl = *open_cl))
		return ;
	if (cl->queue)
	{
		clFlush(cl->queue);
		clFinish(cl->queue);
		clReleaseCommandQueue(cl->queue);
	}
	i = 0;
	while (cl->buffer[i])
		clReleaseMemObject(cl->buffer[i++]);
	i = 0;
	while (cl->program[i])
		clReleaseProgram(cl->program[i++]);
	i = 0;
	while (cl->kernel[i])
		clReleaseKernel(cl->kernel[i++]);
	if (cl->context)
		clRetainContext(cl->context);
	ft_memdel((void **)&cl);
}
