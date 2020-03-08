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

#include "ft_mod1.h"

t_prog    g_compile[PROGRAMS_COUNT + 10] =
{
	{"clear_cell.cl", "clear_cell", 1, CELLS, -1, -1},
	{"add_part_in_cell.cl", "add_part_in_cell", 2, PARTS, CELLS, -1},
	{"up_part_in_cell.cl", "up_part_in_cell", 1, CELLS, -1, -1},
	{"find_neighbors.cl", "find_neighbors", 2, PARTS, CELLS, -1},
	{"density_press.cl", "density_press", 1, PARTS, -1, -1},
	{"delta_speed.cl", "delta_speed", 1, PARTS, -1, -1},
	{"delta_coord.cl", "delta_coord", 1, PARTS, -1, -1},
	{"change_cell.cl", "change_cell", 3, PARTS, CELLS, INTERFACE},
	{"print_parts.cl", "print_parts", 2, PARTS, -1, -1}, //надо бы это в сунуть в другую функцию...
	{"", "", 0, 0, 0, 0}
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



int ft_znak(int num)
{
	int znak;

	znak = 0;
	if (num > 0)
		znak = 1;
	else if (num < 0)
		znak = -1;
	return (znak);
}



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


void print_img_to_img(void *ptr, void *param)
{
	t_ipart *ipart;
	t_vis *vis;
	t_pict *pict;
	t_vektr vektr;
	t_point p;
	int shift;
	int x;
	int y;

	vis = (t_vis *)param;
	ipart = (t_ipart *)ptr;
	pict = &(vis->pict);
	ft_bzero((void *)&vektr, sizeof(t_vektr));
	ft_fill_dpoint(&vektr.abs, ipart->pos.y, ipart->pos.x, ipart->pos.z);
	ft_change_points4(&(vis->param), &vektr);

	shift = pict->size_line / 2;
	//if (!ft_put_pixel_to_img2(&(vis->pic), &vektr.zoom, pict->addr[shift * pict->size_line + shift]))
	//	return ;
	//printf("_%d_\n",pict->size_line);
	y = 0;
	while (y < pict->size_line)
	{
		x = 0;
		while (x < pict->size_line)
		{
			if (pict->addr[y * pict->size_line + x])
			{
				ft_fill_point(&p, vektr.zoom.y  - shift + y, vektr.zoom.x - shift + x, vektr.zoom.z);
				ft_put_pixel_to_img2(&(vis->pic), &p, pict->addr[y * pict->size_line + x]);
			}
			x++;
		}
		y++;
	}

}


void	ft_print_all_water2(void *ptr, void *param)
{
	t_ipart *ipart;
	t_vis *vis;
	t_vektr vektr;

	vis = (t_vis *)param;
	ipart = (t_ipart *)ptr;
	ft_bzero((void *)&vektr, sizeof(t_vektr));
	ft_fill_dpoint(&vektr.abs, ipart->pos.y, ipart->pos.x, ipart->pos.z);
	ft_change_points4(&(vis->param), &vektr);
	ft_print_rect2(&(vis->pic), &(vektr.zoom), g_color[ipart->type][RADIUS2], g_color[ipart->type][COLOR]);
}





void	ft_refresh_picture(t_vis *vis)
{
	mlx_clear_window(vis->mlx, vis->win);
	ft_clear_image(&(vis->pic));

	ft_print_relief(g_earth, g_cell, &(vis->pic), &(vis->param));



	//ft_init_picture(&(vis->pict), vis->param.len, WATER_COLOR);
	//ft_for_each_elem(g_iparts, print_img_to_img, (void *)vis);
	//free((void *)vis->pict.addr);

	ft_for_each_elem(g_iparts, ft_print_all_water2, (void *)vis);

	//ft_memcpy((void *)vis->pic.addr, (void *)vis->pic.index, vis->pic.count_byte);

	mlx_put_image_to_window(vis->mlx, vis->win, vis->pic.img, 0, 0);

}

t_buff	ft_prepare_one_buffer(void *ptr, size_t elem_size, size_t elem_used, size_t elem_count)
{
	t_buff buff;

	buff.ptr = ptr;
	buff.elem_size = elem_size;
	buff.global_work_size = elem_used;
	buff.buff_used = elem_size * elem_used;
	buff.buff_size = elem_size * elem_count;
	return (buff);
}

void	ft_prepare_to_buffers(t_buff *buff, t_arr *parts, t_arr *iparts, t_cell *cell)
{
	if (!ft_realloc_arr(iparts, parts->elems_count))
		ft_del_all("realloc error\n");
	iparts->elems_used = parts->elems_used;

	buff[PARTS] = ft_prepare_one_buffer((void *)parts->elems,
		sizeof(t_part), parts->elems_used, parts->elems_count);
	buff[INTERFACE] = ft_prepare_one_buffer((void *)iparts->elems,
		sizeof(t_ipart), parts->elems_used, parts->elems_count);
	if (cell)
		buff[CELLS] = ft_prepare_one_buffer((void *)cell,
			sizeof(t_cell), CELL_COUNT, CELL_COUNT);
}



void	ft_add_new_water(t_arr *parts, t_arr *iparts, t_buff *buff, t_open_cl *cl)
{
	ft_putstr("-1\n");
	ft_create_new_area_of_water(&parts, &((t_point){JMAX/2 - 10, IMAX/2 - 10, KMAX/2 - 10})
	, &((t_point){JMAX/2 + 10, IMAX/2 + 10, KMAX/2 + 10}), MAGMA);
	ft_putstr("0\n");
	//скопировать содержимое буфера в структуру

	buff[PARTS].ptr = parts->elems;
	clFinish(cl->queue);
	if (!ft_read_buffers(cl, PARTS, buff, CL_TRUE))
		ft_del_all("read error\n");
	if (buff[PARTS].buff_size / buff[PARTS].elem_size != parts->elems_count)
	{
		//уничтожить буфер
		ft_putstr("1\n");
		ft_stop_cl(cl);
		if (clReleaseMemObject(cl->buffer[PARTS]) != CL_SUCCESS)
			ft_del_all("del buffer error\n");
		if (clReleaseMemObject(cl->buffer[INTERFACE]) != CL_SUCCESS)
			ft_del_all("del buffer error\n");
		ft_putstr("2\n");
		ft_prepare_to_buffers(buff, parts, iparts, NULL);
		//создать новый
		if (!ft_create_buffers(cl, PARTS, buff[PARTS].ptr, buff[PARTS].buff_size))
			ft_del_all("buffer error\n");
		if (!ft_create_buffers(cl, INTERFACE, buff[INTERFACE].ptr, buff[INTERFACE].buff_size))
			ft_del_all("buffer error\n");
		ft_putstr("3\n");
		if (!ft_set_kernel_arg(cl, g_compile))
			ft_del_all("set error\n");
		ft_putstr("4\n");
	}
	else
		ft_prepare_to_buffers(buff, parts, iparts, NULL);

	if (clEnqueueWriteBuffer(cl->queue, cl->buffer[PARTS], CL_TRUE, 0, buff[PARTS].buff_used, buff[PARTS].ptr
	, 0, NULL, NULL) != CL_SUCCESS)
		ft_del_all("realloc error\n");
	ft_putstr("5\n");
}





int loop_hook(void *param)
{
	t_vis *vis;

	vis = (t_vis *)param;

	if (vis->param.exit)
		return (0);
	if (!vis->param.rain && !ft_read_buffers(g_cl, INTERFACE, g_buff, CL_TRUE))
		ft_del_all("read error\n");
	if (vis->param.rain && vis->param.solver_pause)
	{
		ft_add_new_water(g_parts, g_iparts, g_buff, g_cl);
		vis->param.rain = FALSE;
	}


	ft_refresh_picture(vis);

	printf("%ld\n", clock() - g_clock);
	g_clock = clock();

	return (0);
}




int		mouse_hook(int button, int x,int y, void *param)
{
	t_vis *vis;
	int cell_number;

	vis = (t_vis *)param;

	if (vis->param.exit)
		return (0);
	cell_number = vis->pic.index[y * CONST_WIDTH + x];
	if (!cell_number)
		return (0);
	if (ft_change_obstacles(g_cell, cell_number, button, &vis->param))
	{
		vis->param.is_obstacles_change = TRUE;
		vis->param.is_relief_changed = TRUE;
	}
	return (0);
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

	//t_part part;
	//ft_arr_add(g_parts, (void *)&part);
	ft_create_new_area_of_water(&g_parts, &((t_point){99, 99, 98}), &((t_point){99, 99, 99}), WATER);

	//ft_create_new_area_of_water(&g_parts, &((t_point){2, 2, 2}), &((t_point){JMAX - 50, 11, KMAX - 1}), WATER);
	//ft_create_new_area_of_water(&g_parts, &((t_point){2, IMAX - 11, 2}), &((t_point){JMAX - 50, IMAX - 1, KMAX - 1}), MAGMA);


	//ft_create_new_area_of_water(&g_parts, &((t_point){JMAX - 12, IMAX / 2 - 7, KMAX / 2 - 7}), &((t_point){JMAX - 1, IMAX / 2 + 7, KMAX / 2 + 7}), MAGMA);
	//ft_create_new_area_of_water(&g_parts, &((t_point){1, 2, 2}), &((t_point){7, IMAX - 1, KMAX - 1}), WATER);


	ft_fill_interface(g_parts, g_iparts);

	//создаем буферы и копируем в них инфу

	ft_prepare_to_buffers(g_buff, g_parts, g_iparts, g_cell);
	ft_create_all_buffers(g_cl, g_buff);

	//привязываем аргументы к программам
	//ft_prepare_to_compile(g_cl, g_compile, g_buff);
	if (!ft_set_kernel_arg(g_cl, g_compile))
		ft_del_all("set error\n");


	ft_create_thread_for_solver(&solver, g_cl, &(vis->param), g_buff);

	//g_clock = clock();

	mlx_hook(vis->win, 2, 0, deal_key, (void *)&(vis->param));
	mlx_loop_hook(vis->mlx, loop_hook, (void *)vis);
	mlx_mouse_hook(vis->win, mouse_hook, (void *)vis);
	mlx_loop(vis->mlx);

	return (0);
}
