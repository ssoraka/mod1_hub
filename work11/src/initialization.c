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

void	init_cl_properties(void)
{
	t_cl_prop	g_prop;
	t_part		part;

	g_prop.g = (t_dpoint){-CONST_G, 0.0, 0.0};
	g_prop.f[WATER] = (t_fluid){WATER_COLOR, 3, PART_H, 120.0,
		PART_MASS_0, 220000000.0, 1000.0, 0000.0};
	g_prop.f[MAGMA] = (t_fluid){MAGMA_COLOR, 5, PART_H, 120.0,
		PART_MASS_0 * 10.0, 2200000000.0, 10000.0, 2000.0};
	ft_arr_add(g_cl_prop, &g_prop);
	ft_bzero(&part, sizeof(t_part));
	ft_arr_add(g_parts, (void *)&part);
}

void	ft_init(void)
{
	g_comlex_ground = NULL;
	g_ground = NULL;
	g_vis = NULL;
	g_parts = NULL;
	g_neighs = NULL;
	g_vis = NULL;
	g_parts = NULL;
	g_neighs = NULL;
	g_iparts = NULL;
	g_iparts_copy = NULL;
	g_cell = NULL;
	g_cell_map = NULL;
	g_earth = NULL;
	g_cl_prop = NULL;
	g_cl = NULL;
}

void	ft_initialization_of_global_variable(void)
{
	g_vis = ft_create_mlx(CONST_WIDTH, CONST_HEINTH, "mod1");
	g_parts = ft_create_arr(sizeof(t_part), 16, NULL);
	g_neighs = ft_create_arr(sizeof(t_neighs), 16, NULL);
	g_iparts = ft_create_arr(sizeof(t_ipart), 16, NULL);
	g_iparts_copy = ft_create_arr(sizeof(t_ipart), 16, NULL);
	g_cell = ft_create_arr(sizeof(t_cell), CELL_COUNT, NULL);
	g_cell_map = ft_create_arr(sizeof(t_cell_map), CELL_COUNT, NULL);
	g_earth = ft_create_earth();
	g_cl_prop = ft_create_arr(sizeof(t_cl_prop), 1, NULL);
	g_cl = ft_init_open_cl(CL_DEVICE_TYPE_GPU);
	if (!g_earth || !g_cell || !g_parts || !g_iparts || !g_cl
		|| !g_iparts_copy || !g_cell_map || !g_neighs || !g_cl_prop)
		ft_del_all("malloc error\n");
	if (!ft_read_and_build_programs(g_cl, g_compile))
		ft_del_all("some error\n");
	g_cell->elems_used = CELL_COUNT;
	g_cell_map->elems_used = CELL_COUNT;
	pthread_mutex_init(&g_mutex, NULL);
	ft_bzero((void *)&g_solver, sizeof(t_solver));
	init_cl_properties();
}

int	ft_del_all(char *message)
{
	ft_mem_arr_free((void ***)(&g_ground));
	ft_mem_arr_free((void ***)(&g_comlex_ground));
	if (g_vis)
		g_vis->param.exit = TRUE;
	pthread_join(g_solver.tid, NULL);
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
	ft_free_open_cl(&g_cl);
	ft_destroy_mlx(&g_vis);
	exit(0);
	return (0);
}
