/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_cl_free.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/16 15:49:41 by ssoraka           #+#    #+#             */
/*   Updated: 2020/02/16 15:49:42 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_mod1.h"

void	ft_stop_cl(t_open_cl *cl)
{
	clFlush(cl->queue);
	clFinish(cl->queue);
}

void	ft_free_open_cl(t_open_cl **open_cl)
{
	t_open_cl	*cl;
	int			i;

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
