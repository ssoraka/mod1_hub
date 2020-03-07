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

void	ft_create_thread_for_solver(t_solver *solver, t_open_cl *cl, t_param *param, t_buff *buff)
{
	solver->cl = cl;
	solver->buff = buff;
	solver->param = param;
	pthread_attr_init(&(solver->attr));
	pthread_create(&(solver->tid), &(solver->attr), ft_solver, (void *)solver);
}

void	ft_after_pause(t_buff *buff, t_param *param, t_open_cl *cl)
{
	if (!param->is_relief_changed)
		return ;
	if (clEnqueueWriteBuffer(cl->queue, cl->buffer[CELLS], CL_TRUE, 0,
	buff[CELLS].global_work_size, buff[CELLS].ptr , 0, NULL, NULL) != CL_SUCCESS)
		ft_del_all("write in buffer error\n");
	param->is_relief_changed = FALSE;
}

void	*ft_solver(void *param)
{
	t_solver *s;

	s = (t_solver *)param;
	while (!s->param->exit)
	{
		if (!s->param->pause)
		{
			ft_after_pause(s->buff, s->param, s->cl);
			if (!ft_run_kernels(s->cl))
				ft_del_all("run error\n");
		}
	}
	ft_stop_cl(s->cl);
	pthread_exit(0);
	return (NULL);
}
