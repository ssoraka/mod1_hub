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

void	*ft_solver(void *param)
{
	t_solver *s;

	s = (t_solver *)param;
	while (!s->param->exit)
	{
		if (!s->param->pause)
			if (!ft_run_kernels(s->cl))
				ft_del_all("run error\n");
	}
	ft_stop_cl(s->cl);
	pthread_exit(0);
	return (NULL);
}
