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


//
// надо в сигнатуру ft_print_plgn(t_plgn *plgn, t_pict *pic, int grad, REAL cos)
// с углом нормали к свету
//

t_prog    g_compile[PROGRAMS_COUNT + 10] =
{
	{"./cl/clear_cell.cl", "clear_cell", 2, {CELL_MAPS, CELLS, -1}},
	{"./cl/add_part_in_cell.cl", "add_part_in_cell", 2, {PARTS, CELL_MAPS, -1}},
	{"./cl/up_part_in_cell.cl", "up_part_in_cell", 1, {CELL_MAPS, -1, -1}},
	{"./cl/find_neighbors.cl", "find_neighbors", 3, {PARTS, CELL_MAPS, NEIGHS}},
	{"./cl/density_press.cl", "density_press", 2, {PARTS, NEIGHS, -1}},
	{"./cl/delta_speed.cl", "delta_speed", 2, {PARTS, NEIGHS, -1}},
	{"./cl/delta_coord.cl", "delta_coord", 2, {PARTS, NEIGHS, -1}},
	{"./cl/change_cell.cl", "change_cell", 3, {PARTS, CELLS, INTERFACE}},
	{"./cl/print_parts.cl", "print_parts", 2, {PARTS, -1, -1}}, //надо бы это в сунуть в другую функцию...
	{"", "", 0, {0, 0, 0}}
};

REAL	g_param[FLUIDS][COLUMN_COUNT] =
{
	{FLUID, F_H, F_C, F_MASS, F_PRESS, F_DENS, F_VIS, F_Y_SPEED},
	{WATER, PART_H, 120.0, PART_MASS_0, 220000000.0, 1000.0, 0000.0, 00.0},
	{WATER2, PART_H, 120.0, PART_MASS_0 * 0.50, 22000.0, 500.0, 0000.0, 00.0},
	{MAGMA, PART_H, 120.0, PART_MASS_0 * 1.0, 220000000.0, 1000.0, 00.0, -10.0},
	{MAGMA2, PART_H, 120.0, PART_MASS_0 * 10.0, 2200000000.0, 10000.0, 2000.0, 0.0},
	{BLOB, PART_H, 120.0, PART_MASS_0, 220000000.0, 1000.0, 0.0, -20.0},
	{OBSTCL, PART_H, 120.0, PART_MASS_0, 220000000.0, 1000.0, 0000.0, 00.0},
};

int	g_color[FLUIDS][COLUMN_COUNT2] =
{
	{FLUID, COLOR, RADIUS2},
	{WATER, WATER_COLOR, 3},
	{WATER2, 0xFFFFFF, 3},
	{MAGMA, 0xFF0000, 5},
	{MAGMA2, OBSTACLES_TOP_COLOR, 5},
	{BLOB, 0xFFFFFF, 3},
	{OBSTCL, OBSTACLES_TOP_COLOR, 5},
};


/*
 * todo сделать вращение вокруг мышки
 * todo сделать сериаализацию
 * todo разобрать баг с замедлением
 * todo попробовать сделать дождь и волны
 * todo сделать изменение осей гравитации
 */

void	ft_cycle_cube(void *param, void (*f)(void *, int, int, int), t_point *start, t_point *end)
{
	int k;
	int j;
	int i;
	t_point delta;

	delta.y = (end->y < start->y) ? -1 : 1;
	delta.x = (end->x < start->x) ? -1 : 1;
	delta.z = (end->z < start->z) ? -1 : 1;
	j = start->y;
	while (j != end->y + delta.y)
	{
		i = start->x;
		while (i != end->x + delta.x)
		{
			k = start->z;
			while (k != end->z + delta.z)
			{
				f(param, j, i, k);
				k += delta.z;
			}
			i += delta.x;
		}
		j += delta.y;
	}
}


void	ft_fill_picture(t_pict *pict, int color)
{
	int radius;
	int x;
	int y;
	REAL len;

	radius = pict->size_line / 2;
	if (!radius)
		radius++;
	y = 0;
	while (y < pict->size_line)
	{
		x = 0;
		while (x < pict->size_line)
		{
			len = sqrt((radius - x) * (radius - x) + (radius - y) * (radius - y));
			if (len <= radius)
				pict->addr[y * pict->size_line + x] = ft_grad_color(radius - len, radius, color, 0);
			x++;
		}
		y++;
	}
}

void ft_init_picture(t_pict *pict, int diameter, int color)
{
	ft_bzero((void *)pict, sizeof(t_pict));

	if (!(diameter % 2))
		diameter++;
	pict->size_line = diameter;
	pict->addr = ft_memalloc(4 * diameter * (diameter + 1));
	ft_fill_picture(pict, color);
}

//void	ft_print_all_water2(void *ptr, void *param)
//{
//	t_ipart *ipart;
//	t_vis *vis;
//	t_vektr vektr;
//
//	vis = (t_vis *)param;
//	ipart = (t_ipart *)ptr;
//	if (ipart->type >= FLUIDS)
//		return ;
//	ft_bzero((void *)&vektr, sizeof(t_vektr));
//	ft_fill_dpoint(&vektr.abs, ipart->pos.y, ipart->pos.x, ipart->pos.z);
//	ft_rotate_point_around_point(&(vis->param), &vektr);
//	ft_print_rect2(&(vis->pic), &(vektr.zoom), g_color[ipart->type][RADIUS2], g_color[ipart->type][COLOR]);
//}

void	print_img_as_water(t_arr *ipoints, t_vis *vis, t_pict *from)
{
	t_iter iter;
	t_ipart *ipart;
	t_shape printer;
	t_pict *to;
	t_vektr v;

	ft_bzero((void *)&v, sizeof(t_vektr));
	ft_init_shape(&printer, IMAGE, set_param(DEFAULT_IMAGE, DEFAULT_INDEX, WATER_COLOR));
	printer.pic = from;
	to = &vis->pic;
	iter = get_arr_iter(ipoints);
	while ((ipart = (t_ipart *)iter.get_next_elem(&iter)))
	{
		ft_fill_dpoint(&v.abs, ipart->pos.y, ipart->pos.x, ipart->pos.z);
		ft_rotate_point_around_point(&(vis->param), &v);
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

	// todo исправить говно с флагами
	t_param *param = &vis->param;
	if ((ft_move_camera(&vis->param) + ft_auto_rotate(&vis->param)))
	{
		ft_rotate_point_around_point(param, &param->centr);
		param->centr.zoom.x = param->cam_x;
		param->centr.zoom.y = param->cam_y;
		param->need_refresh = TRUE;
	}

	ft_print_relief(g_earth, g_cell, &(vis->pic), &(vis->param));

	if (vis->param.is_need_print_water)
		ft_print_water_cell(g_cell, &(vis->pic), &(vis->param));

	ft_init_picture(&(vis->pict), vis->param.len, WATER_COLOR);
//	ft_for_each_elem(g_iparts, print_img_to_img, (void *)vis);
	print_img_as_water(g_iparts, vis, &vis->pict);
	free((void *)vis->pict.addr);

	//ft_for_each_elem(g_iparts, ft_print_all_water2, (void *)vis);


//	ft_memcpy((void *)vis->pic.addr, (void *)vis->pic.index, vis->pic.count_byte);

	mlx_put_image_to_window(vis->mlx, vis->win, vis->pic.img, 0, 0);

}

void	ft_prepare_one_buffer(t_buff *buff)
{
	buff->work_size = buff->arr->elems_used;
	buff->buff_used = buff->work_size * buff->arr->elem_size;

	ft_putnbr(buff->arr->elem_size);
	ft_putchar('\n');
	ft_putnbr(buff->work_size);
	ft_putchar('\n');
	ft_putnbr(buff->buff_used);
	ft_putchar('\n');
}

void	ft_init_buffers(t_buff *buff, t_arr *arr)
{
	buff->arr = arr;
	buff->g_work_size = &(arr->elems_used);
	ft_prepare_one_buffer(buff);
}



int loop_hook(void *param)
{
	t_vis *vis;

	vis = (t_vis *)param;

	if (vis->param.exit)
		return (0);
	if (vis->param.rain == RAIN_ACTIVATE)
		return (0);

	if (!ft_read_buffers(g_cl, INTERFACE, CL_TRUE))
		ft_del_all("read error\n");

	/*
	if (vis->param.rain && vis->param.solver_pause)
	{
		//ft_add_new_water(g_parts, g_iparts, g_buff, g_cl);
		vis->param.rain = FALSE;
	}
*/

	ft_refresh_picture(vis);

//	printf("%ld\n", clock() - g_clock);
	g_clock = clock();

	if (vis->param.rain == NEED_STOP_PRINT_FOR_RAIN)
		vis->param.rain = RAIN_ACTIVATE;
	return (0);
}




//int		mouse_hook(int button, int x,int y, void *param)
//{
//	t_vis *vis;
//	int cell_number;
//
//	vis = (t_vis *)param;
//
//	if (vis->param.exit)
//		return (0);
//	cell_number = vis->pic.index[y * CONST_WIDTH + x];
//	if (!cell_number)
//		return (0);
//	if (ft_change_obstacles(g_cell, cell_number, button, &vis->param))
//	{
//		vis->param.is_obstacles_change = TRUE;
//		vis->param.is_relief_changed = TRUE;
//	}
//	return (0);
//}



void	ft_fill_heighbors(t_arr *parts, t_arr *neighs)
{
	t_neighs n;
	int i;

	i = 0;
	while (neighs->elems_used < parts->elems_used)
		ft_arr_add(neighs, (void *)&n);
}



int main(int ac, char **av)
{

	if (!(ground = ft_read_ground_from_file3("demo.txt")))
		return(0);

	int **comlex_ground = ft_create_complex_ground_from_simple(ground);


	ft_initialization_of_global_variable();

	//создаем все массивы, проставляем начальные значения скоростей и давления
	//инициализируем все глобальные переменные


	//заполняем 3д карту с 2д рельефа
	ft_fill_cells_from_ground(g_cell, ground);

	//создаем модель для 3д карты
	//ft_create_points_in_cells(vis);




	//ft_create_relief(vis, comlex_ground);
	ft_create_relief2(g_earth, comlex_ground);
	//ft_create_points_in_cells(vis);
	//ft_del_all("exit\n");

	t_part part;
	ft_arr_add(g_parts, (void *)&part);
	//ft_create_new_area_of_water(g_parts, &((t_point){99, 99, 98}), &((t_point){99, 99, 99}), WATER);

//	ft_create_new_area_of_water(g_parts, &((t_point){2, 2, 2}), &((t_point){JMAX - 20, 11, KMAX - 1}), WATER);
//	ft_create_new_area_of_water(g_parts, &((t_point){2, IMAX - 11, 2}), &((t_point){JMAX - 20, IMAX - 1, KMAX - 1}), MAGMA);


	//ft_create_new_area_of_water(g_parts, &((t_point){JMAX - 12, IMAX / 2 - 7, KMAX / 2 - 7}), &((t_point){JMAX - 1, IMAX / 2 + 7, KMAX / 2 + 7}), MAGMA);
	//ft_create_new_area_of_water(g_parts, &((t_point){1, 2, 2}), &((t_point){7, IMAX - 1, KMAX - 1}), WATER);


	ft_fill_interface(g_parts, g_iparts);
	ft_fill_heighbors(g_parts, g_neighs);


	//создаем буферы и копируем в них инфу

	ft_init_buffers(&(g_cl->buff[PARTS]), g_parts);
	ft_init_buffers(&(g_cl->buff[CELLS]), g_cell);
	ft_init_buffers(&(g_cl->buff[INTERFACE]), g_iparts);
	ft_init_buffers(&(g_cl->buff[CELL_MAPS]), g_cell_map);
	ft_init_buffers(&(g_cl->buff[NEIGHS]), g_neighs);

	ft_create_all_buffers(g_cl);


	//привязываем аргументы к программам
	//ft_prepare_to_compile(g_cl, g_compile, g_buff);
	if (!ft_set_kernel_arg(g_cl, g_compile))
		ft_del_all("set error\n");


	ft_create_thread_for_solver(&solver, g_cl, &(vis->param), g_compile);
	//g_clock = clock();

	ft_init_hooks(vis);
	mlx_loop(vis->mlx);
//	mlx_hook(vis->win, 2, 0, deal_key, (void *)&(vis->param));
//	mlx_loop_hook(vis->mlx, loop_hook, (void *)vis);
//	mlx_mouse_hook(vis->win, mouse_hook, (void *)vis);
//	mlx_loop(vis->mlx);

	return (0);
}
