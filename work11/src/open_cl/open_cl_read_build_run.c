/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_cl_read_build_run.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/16 15:49:41 by ssoraka           #+#    #+#             */
/*   Updated: 2020/02/16 15:49:42 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_mod1.h"

int		is_read_programm(char *buffer, char *filename)
{
	int fd;
	int ret;

	if ((fd = open(filename, O_RDONLY)) == -1)
		return (FALSE);
	if (read(fd, NULL, 0) == -1)
		return (FALSE);
	ret = read(fd, buffer, PROGRAMM_SIZE);
	buffer[ret] = '\0';
	close(fd);
	return (TRUE);
}

int		ft_read_and_build_programs(t_open_cl *cl, t_prog *compile)
{
	char	buffer[PROGRAMM_SIZE];
	char	*arr[2];
	size_t	lengths[1];
	int		i;

	arr[1] = NULL;
	i = -1;
	while (++i < PROGRAMS_COUNT)
	{
		if (!is_read_programm((char *)buffer, compile[i].file))
			return (FALSE);
		lengths[0] = ft_strlen(buffer);
		arr[0] = (char *)&buffer;
		cl->program[i] = clCreateProgramWithSource(cl->context, 1,
								(const char **)arr, lengths, &cl->errcode_ret);
		if (cl->errcode_ret != CL_SUCCESS
		|| clBuildProgram(cl->program[i], cl->num_devices,
		&cl->device, NULL, NULL, NULL) != CL_SUCCESS)
			return (FALSE);
		cl->kernel[i] = clCreateKernel(cl->program[i],
									compile[i].kernel, &cl->errcode_ret);
		if (cl->errcode_ret != CL_SUCCESS)
			return (FALSE);
	}
	return (TRUE);
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
		while (n < (int)compile[i].arg_count)
		{
			if (clSetKernelArg(cl->kernel[i], n, sizeof(cl_mem),
			(void *)&(cl->buff[compile[i].arg[n]].buffer)) != CL_SUCCESS)
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
		cl->buff[cl->buff_index[i]].g_work_size, NULL, 0, NULL, NULL)
		!= CL_SUCCESS)
			return (FALSE);
		clFinish(cl->queue);
		i++;
	}
	return (TRUE);
}
