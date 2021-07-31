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

void	print_img_as_water(t_arr *ipoints, t_vis *vis, t_pict *from)
{
	t_iter	iter;
	t_ipart	*ipart;
	t_shape	printer;
	t_pict	*to;
	t_vektr	v;

	ft_bzero((void *)&v, sizeof(t_vektr));
	ft_init_shape(&printer, IMAGE, set_param(DEFAULT_IMAGE, DEFAULT_INDEX, WATER_COLOR));
	printer.pic = from;
	to = &vis->pic;
	iter = get_arr_iter(ipoints);
	while ((ipart = (t_ipart *)iter.get_next_elem(&iter)))
	{
		ft_fill_dpoint(&v.abs, ipart->pos.y, ipart->pos.x, ipart->pos.z);
		ft_rotate_point_around_point(&(vis->param), &v);
		printer.params.index = ipart->type;
		if (printer.params.index != NOTHING)
			printer.print(to, &v.zoom, &printer);
	}
}

// todo исправить смену цвета для магмы
// todo добавить изменеие режима отображения для воды в виде точек
void	print_rect_as_water(t_arr *ipoints, t_vis *vis)
{
	t_iter	iter;
	t_ipart	*ipart;
	t_shape	printer;
	t_pict	*to;
	t_vektr	v;

	ft_bzero((void *)&v, sizeof(t_vektr));
	ft_init_shape(&printer, RECTANGLE, set_param(DEFAULT_IMAGE, WATER, WATER_COLOR));
	printer.len = 3;
	to = &vis->pic;
	iter = get_arr_iter(ipoints);
	while ((ipart = (t_ipart *)iter.get_next_elem(&iter)))
	{
		ft_fill_dpoint(&v.abs, ipart->pos.y, ipart->pos.x, ipart->pos.z);
		ft_rotate_point_around_point(&(vis->param), &v);
		printer.params.color = (ipart->type == WATER) * WATER_COLOR | (ipart->type == MAGMA) * MAGMA_COLOR;
		if (printer.params.index != NOTHING)
			printer.print(to, &v.zoom, &printer);
	}
}

void	ft_refresh_picture(t_vis *vis)
{
	mlx_clear_window(vis->mlx, vis->win);
	ft_clear_image(&(vis->pic));
	if (vis->param.is_water_change)
	{
		ft_move_water_cell(g_cell, &(vis->param));
		vis->param.is_water_change = FALSE;
	}
	ft_print_relief(g_earth, g_cell, &(vis->pic), &(vis->param));
	if (vis->param.is_need_print_water)
		ft_print_water_cell(g_cell, &(vis->pic), &(vis->param));
	if (vis->param.print_sprite == FALSE)
	{
		print_rect_as_water(g_iparts_copy, vis);
		mlx_put_image_to_window(vis->mlx, vis->win, vis->pic.img, 0, 0);
		return ;
	}
	if (!ft_init_picture(vis->fluids + WATER, vis->param.len, WATER_COLOR)
	|| !ft_init_picture(vis->fluids + MAGMA, vis->param.len, MAGMA_COLOR))
	{
		ft_memdel((void **)&vis->fluids[WATER].addr);
		ft_memdel((void **)&vis->fluids[MAGMA].addr);
		ft_del_all("malloc error\n");
	}
	print_img_as_water(g_iparts_copy, vis, vis->fluids);
	ft_memdel((void **)&vis->fluids[WATER].addr);
	ft_memdel((void **)&vis->fluids[MAGMA].addr);

//	ft_memcpy((void *)vis->pic.addr, (void *)vis->pic.index, vis->pic.count_byte);

	mlx_put_image_to_window(vis->mlx, vis->win, vis->pic.img, 0, 0);
}

int loop_hook(void *parameters)
{
	t_vis *vis;
	t_param *param;

	vis = (t_vis *)parameters;
	if (vis->param.exit)
		return (0);
	param = &vis->param;
	if ((ft_move_camera(param) + ft_auto_rotate(param)))
	{
		ft_rotate_point_around_point(param, &param->centr);
		param->centr.zoom.x = param->cam_x;
		param->centr.zoom.y = param->cam_y;
		param->need_refresh = TRUE;
	}
	pthread_mutex_lock(&g_mutex);
	if (!ft_copy_arrs(g_iparts_copy, g_cl->buff[INTERFACE].arr))
		ft_del_all("read error\n");
	pthread_mutex_unlock(&g_mutex);
	ft_refresh_picture(vis);
	return (0);
}

int main(int ac, char **av)
{
	ft_init();
	if (!(g_ground = ft_read_ground_from_file3("demo.txt")))
		ft_del_all("some problem with file\n");
	g_comlex_ground = ft_create_complex_ground_from_simple(g_ground);
	if (!g_comlex_ground)
		ft_del_all("some problem with malloc in relief\n");
	ft_initialization_of_global_variable();
	ft_fill_cells_from_ground(g_cell, g_ground);
	if (!ft_create_relief2(g_earth, g_comlex_ground))
		ft_del_all("some problem with malloc in ground\n");
	ft_init_buffers(&(g_cl->buff[PARTS]), g_parts);
	ft_init_buffers(&(g_cl->buff[CELLS]), g_cell);
	ft_init_buffers(&(g_cl->buff[INTERFACE]), g_iparts);
	ft_init_buffers(&(g_cl->buff[CELL_MAPS]), g_cell_map);
	ft_init_buffers(&(g_cl->buff[NEIGHS]), g_neighs);
	ft_init_buffers(&(g_cl->buff[PARAMS]), g_cl_prop);
	if (!ft_create_all_buffers(g_cl))
		ft_del_all("some problem with openCl buffers\n");
	if (!ft_set_kernel_arg(g_cl, g_compile))
		ft_del_all("some problem with openCl arguments\n");
	ft_create_thread_for_solver(&g_solver, g_cl, &(g_vis->param), g_compile);
	ft_init_hooks(g_vis);
	mlx_loop(g_vis->mlx);
	return (0);
}
