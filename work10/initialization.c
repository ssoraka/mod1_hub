/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialization.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/04 19:05:18 by ssoraka           #+#    #+#             */
/*   Updated: 2019/10/04 19:05:19 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_mod1.h"

void	ft_init_variable(void)
{
	g_earth = NULL;
	g_cell = NULL;
	g_parts = NULL;
	g_iparts = NULL;
	g_cl = NULL;
}

void	ft_init_delta_xyz(void)
{
	g.x = CONST_GX;
	g.y = CONST_GY;
	g.z = CONST_GZ;
}

void	ft_init_parts_arr_and_cell(void)
{
	ft_bzero((void *)&solver, sizeof(t_solver));
	if (!(vis = ft_create_mlx(CONST_WIDTH, CONST_HEINTH, "mod1")))
		ft_del_all(NULL);
	if (!(g_parts = ft_init_all_clear_parts()))
		ft_del_all(NULL);
	if (!(g_iparts = ft_create_arr(sizeof(t_ipart), g_parts->elems_count, NULL)))
		ft_del_all(NULL);
	if (!(g_cell = ft_memalloc(sizeof(t_cell) * (CELL_COUNT + 1))))
		ft_del_all(NULL);
	if (!(g_earth = ft_create_earth()))
		ft_del_all(NULL);

	//инициализируем сл и компилим программы
	if (!(g_cl = ft_init_open_cl()))
		ft_del_all("init error\n");
	if (!ft_read_and_build_programs(g_cl, g_compile))
		ft_del_all("some error\n");

}


void	ft_initialization_of_global_variable(void)
{
	ft_init_variable();
	ft_init_delta_xyz();
	ft_init_parts_arr_and_cell();
}



void	ft_del_all(char *message)
{
	if (vis)
		vis->param.exit = TRUE;
	pthread_join(solver.tid, NULL);
	if (message)
		ft_putstr_fd(message, 2);
	ft_del_arr(&g_parts);
	ft_del_arr(&g_iparts);
	ft_del_earth(&g_earth);
	ft_str_arr_free((char ***)(&ground));
	ft_memdel((void **)&g_cell);
	ft_free_open_cl(&g_cl);
	ft_destroy_mlx(&vis);
	exit(0);
}
