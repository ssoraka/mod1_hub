/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/04 16:23:42 by ssoraka           #+#    #+#             */
/*   Updated: 2019/03/07 16:22:42 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_mod1.h"

t_prog	g_compile[PROGRAMS_COUNT + 2] =
{
	{"./cl/clear_cell.cl", "clear_cell", 2, {CELL_MAPS, CELLS, -1}},
	{"./cl/add_part_in_cell.cl", "add_part_in_cell", 2, {PARTS, CELL_MAPS, -1}},
	{"./cl/up_part_in_cell.cl", "up_part_in_cell", 1, {CELL_MAPS, -1, -1}},
	{"./cl/find_neighbors.cl", "find_neighbors", 3, {PARTS, CELL_MAPS, NEIGHS}},
	{"./cl/density_press.cl", "density_press", 3, {PARTS, NEIGHS, PARAMS}},
	{"./cl/delta_speed.cl", "delta_speed", 3, {PARTS, NEIGHS, PARAMS}},
	{"./cl/delta_coord.cl", "delta_coord", 3, {PARTS, NEIGHS, PARAMS}},
	{"./cl/change_cell.cl", "change_cell", 3, {PARTS, CELLS, INTERFACE}},
	{"./cl/print_parts.cl", "print_parts", 2, {PARTS, -1, -1}},
	{"", "", 0, {-1, -1, -1}}
};

void	usage(void)
{
	ft_putendl("USAGE:");
	ft_putendl("./mod1 file.mod1");
	ft_putendl("\nFILE_CONTENT:");
	ft_putendl("(x1,z1,y1) (x2,z2,y2) ... (xn,zn,yn)");
	ft_putendl("where n <= 50");
	ft_putendl("\nKEY CONTROL:");
	ft_putendl("Q,W,E,A,S,D, LEFT MOUSE - rotate");
	ft_putendl("UP,DOWN,LEFT,RIGHT,MIDDLE MOUSE - shift");
	ft_putendl("1,2,MIDDLE ROTATE MOUSE - backtrack");
	ft_putendl("+/- - increase/decrease virtual fluid or ground");
	ft_putendl("4,5,6,8,1,2 - shift virtual fluid or ground");
	ft_putendl("LEFT MOUSE/RIGHT MOUSE  - add/cut ground");
	ft_putendl("V - change fluid visualisation");
	ft_putendl("R/T - create water/magma in virtual area");
	ft_putendl("G - change gravitation direction");
	ft_putendl("Z - change ground visualisation");
	ft_putendl("X - hide ground");
	ft_putendl("C - gradient");
}

void	init_all_buffers_param(void)
{
	ft_init_buffers(&(g_cl->buff[PARTS]), g_parts);
	ft_init_buffers(&(g_cl->buff[CELLS]), g_cell);
	ft_init_buffers(&(g_cl->buff[INTERFACE]), g_iparts);
	ft_init_buffers(&(g_cl->buff[CELL_MAPS]), g_cell_map);
	ft_init_buffers(&(g_cl->buff[NEIGHS]), g_neighs);
	ft_init_buffers(&(g_cl->buff[PARAMS]), g_cl_prop);
}

t_bool	valid_file_name(char *str)
{
	int	len;

	len = ft_strlen(str);
	return (len > 5 && ft_strequ(".mod1", str + len - 5));
}

int	main(int argc, char **argv)
{
	ft_init();
	if (argc != 2 || !valid_file_name(argv[1]))
	{
		usage();
		return (0);
	}
	g_ground = ft_read_ground_from_file(argv[1]);
	if (!g_ground)
		ft_del_all("some problem with file\n");
	g_comlex_ground = ft_create_complex_ground_from_simple(g_ground);
	if (!g_comlex_ground)
		ft_del_all("some problem with malloc in relief\n");
	ft_initialization_of_global_variable();
	ft_fill_cells_from_ground(g_cell, g_ground);
	if (!ft_create_relief(g_earth, g_comlex_ground))
		ft_del_all("some problem with malloc in model\n");
	init_all_buffers_param();
	if (!ft_create_all_buffers(g_cl))
		ft_del_all("some problem with openCl buffers\n");
	if (!ft_set_kernel_arg(g_cl, g_compile))
		ft_del_all("some problem with openCl arguments\n");
	ft_create_thread_for_solver(&g_solver, g_cl, &(g_vis->param), g_compile);
	ft_init_hooks(g_vis);
	mlx_loop(g_vis->mlx);
	return (0);
}
