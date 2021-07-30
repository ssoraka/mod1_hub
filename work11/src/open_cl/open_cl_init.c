/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_cl_init.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/16 15:49:41 by ssoraka           #+#    #+#             */
/*   Updated: 2020/02/16 15:49:42 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_mod1.h"

void	pfn_notify(const char *errinfo, const void *private_info, size_t cb,
				void *user_data)
{
	(void)private_info;
	cb = 0;
	(void)user_data;
	ft_putendl(errinfo);
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

void	*ft_cl_error(t_open_cl *cl, char *message)
{
	if (message)
		ft_putstr_fd(message, 2);
	ft_free_open_cl(&cl);
	return (NULL);
}

t_open_cl	*ft_init_open_cl(int device)
{
	t_open_cl	*cl;

	if (is_null(ft_memalloc(sizeof(t_open_cl)), (void **)&cl))
		return (ft_cl_error(cl, "malloc error!!!\n"));
	if (clGetPlatformIDs(1, &cl->platform, &cl->num_platform) != CL_SUCCESS)
		return (ft_cl_error(cl, "platform error!!!\n"));
	cl->device_type = device;
	if (clGetDeviceIDs(cl->platform, cl->device_type, 1, &cl->device,
			&cl->num_devices) != CL_SUCCESS)
		return (ft_cl_error(cl, "device error!!!\n"));
	cl->context = clCreateContext(NULL, cl->num_devices, &cl->device,
			&pfn_notify, NULL, &cl->errcode_ret);
	if (cl->errcode_ret != CL_SUCCESS)
	{
		ft_context_error(cl->errcode_ret);
		return (ft_cl_error(cl, "context error!!!\n"));
	}
	cl->queue = clCreateCommandQueue(cl->context, cl->device, 0,
			&cl->errcode_ret);
	if (cl->errcode_ret != CL_SUCCESS)
	{
		ft_queue_error(cl->errcode_ret);
		return (ft_cl_error(cl, "queue error!!!\n"));
	}
	return (cl);
}
