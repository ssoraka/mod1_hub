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

#include "../includes/ft_mod1.h"

#define CUBE_LEN 9

void	ft_create_thread_for_solver(t_solver *solver, t_open_cl *cl, t_param *param, t_prog *compile)
{
	solver->cl = cl;
	solver->param = param;
	solver->compile = compile;
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

void	ft_add_new_water(t_open_cl *cl, t_param *param)
{
	t_point start;
	t_point end;
	t_point p;
	int brush;

	ft_stop_cl(cl);
	//создаем новую воду
	if (!ft_read_buffers(cl, PARTS, CL_TRUE))
		ft_del_all("read error\n");
	//для удаления застрявших частиц
	//ft_del_elems_if(cl->buff[PARTS].arr, &ft_del_unused_part, NULL);
	//создаем новую воду
	p = param->water;
	brush = param->brush;
	ft_fill_point(&start, p.y - brush, p.x - brush, p.z - brush);
	ft_fill_point(&end, p.y + brush, p.x + brush, p.z + brush);
	ft_create_new_area_of_water(cl->buff[PARTS].arr, &start, &end, MAGMA);

	//скопировать содержимое буфера в структуру
	//уничтожить буфер
	if (!ft_recreate_buffers(cl, PARTS, CL_TRUE, *(cl->buff[PARTS].g_work_size), TRUE))
		ft_del_all("recreate buffer error\n");
	if (!ft_recreate_buffers(cl, INTERFACE, CL_TRUE, *(cl->buff[PARTS].g_work_size), TRUE))
		ft_del_all("recreate buffer error\n");
	if (!ft_recreate_buffers(cl, NEIGHS, CL_TRUE, *(cl->buff[PARTS].g_work_size), FALSE))
		ft_del_all("recreate buffer error\n");
}

void	*ft_solver(void *param)
{
	t_solver *s;

	s = (t_solver *)param;
	while (!s->param->exit)
	{
		if (s->param->rain == RAIN_ACTIVATE)
		{
			ft_add_new_water(s->cl, s->param);
			if (!ft_set_kernel_arg(s->cl, s->compile))
				ft_del_all("set error\n");
			s->param->rain = RAIN_FALSE;
		}
		if (!s->param->pause)
		{
			ft_after_change_relief(s->cl, s->param);
			if (!ft_run_kernels(s->cl))
				ft_del_all("run error\n");
		}
	}
	ft_stop_cl(s->cl);
	pthread_exit(0);
	return (NULL);
}
