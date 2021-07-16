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

#include "../ft_mod1.h"

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

void	*ft_error(t_open_cl *cl, char *message)
{
	if (message)
		ft_putstr_fd(message, 2);
	ft_free_open_cl(&cl);
	return (NULL);
}

t_open_cl	*ft_init_open_cl(void)
{
	t_open_cl *cl;

	if (!(cl = ft_memalloc(sizeof(t_open_cl))))
		return (ft_error(cl, "malloc error!!!\n"));
	if (clGetPlatformIDs(1, &cl->platform, &cl->num_platform) != CL_SUCCESS)
		return (ft_error(cl, "platform error!!!\n"));
	cl->device_type = CL_DEVICE_TYPE_GPU;
	if (clGetDeviceIDs(cl->platform, cl->device_type, 1, &cl->device, &cl->num_devices) != CL_SUCCESS)
		return (ft_error(cl, "device error!!!\n"));
	cl->context = clCreateContext(NULL, cl->num_devices, &cl->device, &pfn_notify, NULL, &cl->errcode_ret);
	if (cl->errcode_ret != CL_SUCCESS)
	{
		ft_context_error(cl->errcode_ret);
		return (ft_error(cl, "context error!!!\n"));
	}
	cl->queue = clCreateCommandQueue(cl->context, cl->device, 0, &cl->errcode_ret);
	if (cl->errcode_ret != CL_SUCCESS)
	{
		ft_queue_error(cl->errcode_ret);
		return (ft_error(cl, "queue error!!!\n"));
	}
	return (cl);
}

int		ft_read_and_build_programs(t_open_cl *cl, t_prog *compile)
{
	char buffer[PROGRAMM_SIZE];
	char *arr[2];
	size_t lengths[1];
	int i;

	arr[1] = NULL;
	i = 0;
	while (i < PROGRAMS_COUNT)
	{
		if (!is_read_programm((char *)buffer, compile[i].file))
			return (FALSE);
		lengths[0] = ft_strlen(buffer);
		arr[0] = (char *)&buffer;
		cl->program[i] = clCreateProgramWithSource(cl->context, 1, (const char **)arr, lengths, &cl->errcode_ret);
		if (cl->errcode_ret != CL_SUCCESS)
			return (FALSE);
		if (clBuildProgram(cl->program[i], cl->num_devices, &cl->device, NULL, NULL, NULL) != CL_SUCCESS)
			return (FALSE);
		cl->kernel[i] = clCreateKernel(cl->program[i], compile[i].kernel, &cl->errcode_ret);
		if (cl->errcode_ret != CL_SUCCESS)
			return (FALSE);
		i++;
	}
	return (TRUE);
}

int		ft_read_buffers(t_open_cl *cl, int num, int need_wait)
{
	void *ptr;

	ptr = cl->buff[num].arr->elems;
	if (clEnqueueReadBuffer(cl->queue, cl->buff[num].buffer, need_wait, 0
	, cl->buff[num].buff_used, ptr, 0, NULL, NULL) != CL_SUCCESS)
		return (FALSE);
	return (TRUE);
}

int		ft_write_buffers(t_open_cl *cl, int num, int need_wait)
{
	void *ptr;

	ptr = cl->buff[num].arr->elems;
	if (clEnqueueWriteBuffer(cl->queue, cl->buff[num].buffer, need_wait, 0
	, cl->buff[num].buff_size, ptr, 0, NULL, NULL) != CL_SUCCESS)
		return (FALSE);
	return (TRUE);
}

int		ft_create_buffers(t_open_cl *cl, int num, int need_wait)
{
	cl->buff[num].buffer = clCreateBuffer(cl->context, CL_MEM_READ_WRITE
	, cl->buff[num].buff_size, NULL , &(cl->errcode_ret));
	if (cl->errcode_ret != CL_SUCCESS)
		return (FALSE);
	return (TRUE);
}

int		ft_recreate_buffers(t_open_cl *cl, int num, int need_wait, size_t new_elem_count, int need_write)
{
	if (!ft_realloc_arr(cl->buff[num].arr, new_elem_count))
		return (FALSE);
	cl->buff[num].arr->elems_used = new_elem_count;
	if (new_elem_count >= cl->buff[num].old_g_work_size)
	{
		if (clReleaseMemObject(cl->buff[num].buffer) != CL_SUCCESS)
			return (FALSE);
		ft_prepare_one_buffer(&(cl->buff[num]));
		if (!ft_create_buffers(cl, num, need_wait))
			return (FALSE);
		if (need_write && !ft_write_buffers(cl, num, need_wait))
			return (FALSE);
	}
	else
		ft_prepare_one_buffer(&(cl->buff[num]));
	return (TRUE);
}



void	ft_create_all_buffers(t_open_cl *cl)
{
	int i;

	i = 0;
	while (i < BUFFER_COUNT)
	{
		if (!ft_create_buffers(cl, i, CL_TRUE))
			ft_del_all("create buffer error\n");
		if (!ft_write_buffers(cl, i, CL_TRUE))
			ft_del_all("write buffer error\n");
		i++;
	}
}

int		ft_set_kernel_arg(t_open_cl *cl, t_prog *compile)
{
	int i;
	int n;

	i = 0;
	while (i < PROGRAMS_COUNT)
	{
		cl->buff_index[i] = compile[i].arg[0];
		n = 0;
		while (n < compile[i].arg_count)
		{
			if (clSetKernelArg(cl->kernel[i], n, sizeof(cl_mem), (void *)&(cl->buff[compile[i].arg[n]].buffer)) != CL_SUCCESS)
				return (FALSE);
			n++;
		}
		i++;
	}
	return (TRUE);
}


int		ft_run_kernels(t_open_cl *cl)
{
	int i;

	i = 0;
	while (i < PROGRAMS_COUNT)
	{
		if (clEnqueueNDRangeKernel(cl->queue, cl->kernel[i], 1, NULL,
		cl->buff[cl->buff_index[i]].g_work_size, NULL, 0, NULL, NULL) != CL_SUCCESS)
			return (FALSE);
		clFinish(cl->queue);
		i++;
	}
	return (TRUE);
}


void	ft_stop_cl(t_open_cl *cl)
{
	clFlush(cl->queue);
	clFinish(cl->queue);
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
	while (cl->buff[i].buffer)
		clReleaseMemObject(cl->buff[i++].buffer);
	i = 0;
	while (cl->program[i])
		clReleaseProgram(cl->program[i++]);
	i = 0;
	while (cl->kernel[i])
		clReleaseKernel(cl->kernel[i++]);
	if (cl->context)
		clRetainContext(cl->context);
	ft_memdel((void **)open_cl);
}
