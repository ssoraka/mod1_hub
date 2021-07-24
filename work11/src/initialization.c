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

#include "../includes/ft_mod1.h"

void	ft_init_variable(void)
{
	g_earth = NULL;
	g_cell = NULL;
	g_parts = NULL;
	g_iparts = NULL;
	g_cl = NULL;
}

void	init_cl_properties(void)
{
	t_cl_prop	g_prop;

	g_prop.g = (t_dpoint){CONST_GY, CONST_GX, CONST_GZ};
	g_prop.f[WATER] = (t_fluid){WATER_COLOR, 3, PART_H, 120.0, PART_MASS_0, 220000000.0, 1000.0, 0000.0};
	g_prop.f[MAGMA] = (t_fluid){MAGMA_COLOR, 5, PART_H, 120.0, PART_MASS_0 * 10.0, 2200000000.0, 10000.0, 2000.0};
	if (!(g_cl_prop = ft_create_arr(sizeof(t_cl_prop), 1, NULL)))
		ft_del_all(NULL);
	ft_arr_add(g_cl_prop, &g_prop);
}

void	ft_init_parts_arr_and_cell(void)
{
	ft_bzero((void *)&solver, sizeof(t_solver));
	if (!(vis = ft_create_mlx(CONST_WIDTH, CONST_HEINTH, "mod1")))
		ft_del_all(NULL);
	if (!(g_parts = ft_init_all_clear_parts()))
		ft_del_all(NULL);
	if (!(g_neighs = ft_create_arr(sizeof(t_neighs), g_parts->elems_count, NULL)))
		ft_del_all(NULL);
	if (!(g_iparts = ft_create_arr(sizeof(t_ipart), g_parts->elems_count, NULL)))
		ft_del_all(NULL);
	if (!(g_iparts_copy = ft_create_arr(sizeof(t_ipart), g_parts->elems_count, NULL)))
		ft_del_all(NULL);
	if (!(g_cell = ft_create_arr(sizeof(t_cell), CELL_COUNT, NULL)))
		ft_del_all(NULL);
	g_cell->elems_used = CELL_COUNT;
	if (!(g_cell_map = ft_create_arr(sizeof(t_cell_map), CELL_COUNT, NULL)))
		ft_del_all(NULL);
	g_cell_map->elems_used = CELL_COUNT;
	if (!(g_earth = ft_create_earth()))
		ft_del_all(NULL);

	//инициализируем сл и компилим программы
	if (!(g_cl = ft_init_open_cl(CL_DEVICE_TYPE_GPU)))
		ft_del_all("init error\n");
	if (!ft_read_and_build_programs(g_cl, g_compile))
		ft_del_all("some error\n");
}


void	ft_initialization_of_global_variable(void)
{
	ft_init_variable();
	ft_init_parts_arr_and_cell();
	init_cl_properties();
}



void	ft_del_all(char *message)
{
	if (vis)
		vis->param.exit = TRUE;
	pthread_join(solver.tid, NULL);
	if (message)
		ft_putstr_fd(message, 2);
	ft_del_arr(&g_parts);
	ft_del_arr(&g_neighs);
	ft_del_arr(&g_iparts);
	ft_del_arr(&g_iparts_copy);
	ft_del_arr(&g_cell);
	ft_del_arr(&g_cell_map);
	ft_del_arr(&g_cl_prop);
	ft_del_earth(&g_earth);
	ft_mem_arr_free((void ***)(&ground));
	ft_free_open_cl(&g_cl);
	ft_destroy_mlx(&vis);
	exit(0);
}
