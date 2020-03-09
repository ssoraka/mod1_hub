/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_solver.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/29 16:10:09 by ssoraka           #+#    #+#             */
/*   Updated: 2020/02/29 16:10:10 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_mod1.h"

void	ft_create_thread_for_solver(t_solver *solver, t_open_cl *cl, t_param *param)
{
	solver->cl = cl;
	solver->param = param;
	pthread_attr_init(&(solver->attr));
	pthread_create(&(solver->tid), &(solver->attr), ft_solver, (void *)solver);
}

void	ft_after_change_relief(t_open_cl *cl, t_param *param)
{
	if (!param->is_relief_changed)
		return ;
	if (!ft_write_buffers(cl, CELLS, CL_TRUE))
		ft_del_all("write in buffer error\n");
	param->is_relief_changed = FALSE;
}

/*
void	ft_after_add_water(t_buff *buff, t_param *param, t_open_cl *cl)
{
	if (!param->add_new_water)
		return ;
	if (clEnqueueReadBuffer(cl->queue, cl->buffer[PARTS], CL_TRUE, 0,
	buff[PARTS].buff_size, buff[PARTS].ptr, 0, NULL, NULL) != CL_SUCCESS)
		ft_del_all("read in buffer error\n");


	if (clEnqueueWriteBuffer(cl->queue, cl->buffer[PARTS], CL_TRUE, 0,
	buff[PARTS].buff_size, buff[PARTS].ptr , 0, NULL, NULL) != CL_SUCCESS)

	if (!ft_create_buffers(cl, PARTS, buff[PARTS].ptr, buff[PARTS].buff_size))
		ft_del_all("create buffer error\n");
	param->add_new_water = FALSE;
}*/

void	*ft_solver(void *param)
{
	t_solver *s;

	s = (t_solver *)param;
	while (!s->param->exit)
	{
		if (!s->param->solver_pause)
		{
			ft_after_change_relief(s->cl, s->param);
			if (!ft_run_kernels(s->cl))
				ft_del_all("run error\n");
		}
		s->param->solver_pause = s->param->pause;
	}
	ft_stop_cl(s->cl);
	pthread_exit(0);
	return (NULL);
}
