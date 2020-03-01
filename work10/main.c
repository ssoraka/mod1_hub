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
	{"clear_cell.cl", "clear_cell", 1, CELLS, -1, -1, CELL_COUNT},
	{"add_part_in_cell.cl", "add_part_in_cell", 2, PARTS, CELLS, -1, PART_COUNT},
	{"up_part_in_cell.cl", "up_part_in_cell", 1, CELLS, -1, -1, CELL_COUNT},
	{"find_neighbors.cl", "find_neighbors", 2, PARTS, CELLS, -1, PART_COUNT},
	{"density_press.cl", "density_press", 1, PARTS, -1, -1, PART_COUNT},
	{"delta_speed.cl", "delta_speed", 1, PARTS, -1, -1, PART_COUNT},
	{"delta_coord.cl", "delta_coord", 1, PARTS, -1, -1, PART_COUNT},
	{"change_cell.cl", "change_cell", 3, PARTS, CELLS, INTERFACE, PART_COUNT},
	{"print_parts.cl", "print_parts", 2, PARTS, -1, -1, PART_COUNT}, //надо бы это в сунуть в другую функцию...
	{"", "", 0, 0, 0, 0, 0}
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




void ft_print_lines(t_vis *vis, t_line *line)
{
	while (line)
	{
		draw_line_img2(line, &(vis->pic), vis->param.grad);
		line = line->next;
	}
}








void	ft_create_cube_poligons(t_plgn **plgn, t_vektr **p, int color)
{
	t_vektr *tmp[4];

	ft_create_rectang_poligon2(plgn, p, OBSTACLES_FRONT_COLOR);
	ft_create_rectang_poligon2(plgn, p + 4, OBSTACLES_FRONT_COLOR);
	tmp[0] = p[1];
	tmp[1] = p[5];
	tmp[2] = p[4];
	tmp[3] = p[0];
	ft_create_rectang_poligon2(plgn, tmp, OBSTACLES_TOP_COLOR);
	tmp[0] = p[3];
	tmp[1] = p[7];
	ft_create_rectang_poligon2(plgn, tmp, OBSTACLES_TOP_COLOR);
	tmp[2] = p[6];
	tmp[3] = p[2];
	ft_create_rectang_poligon2(plgn, tmp, OBSTACLES_RIGHT_COLOR);
	tmp[0] = p[1];
	tmp[1] = p[5];
	ft_create_rectang_poligon2(plgn, tmp, OBSTACLES_RIGHT_COLOR);
}







void	ft_create_obstacles(void *ptr, int j, int i, int k)
{
	t_vis *vis;
	t_vektr *p[8];
	void *points;
	t_point xyz;

	if (!g_cell[ft_get_index(j, i, k)].obstacle)
		return ;

	if (g_cell[ft_get_index(j + 1, i, k)].obstacle
	&& g_cell[ft_get_index(j - 1, i, k)].obstacle
	&& g_cell[ft_get_index(j, i + 1, k)].obstacle
	&& g_cell[ft_get_index(j, i - 1, k)].obstacle
	&& g_cell[ft_get_index(j, i, k + 1)].obstacle
	&& g_cell[ft_get_index(j, i, k - 1)].obstacle)
		return ;
	vis = (t_vis *)ptr;
	points = (void *)(&(vis->points));
	ft_fill_point(&xyz, j, i, k);
	p[0] = ft_add_vektor2(points, &xyz, 0xFF0000);
	ft_fill_point(&xyz, j, (i + 1), k);
	p[1] = ft_add_vektor2(points, &xyz, 0x00FF00);
	ft_fill_point(&xyz, (j + 1), (i + 1), k);
	p[2] = ft_add_vektor2(points, &xyz, 0xFFFFFF);
	ft_fill_point(&xyz, (j + 1), i, k);
	p[3] = ft_add_vektor2(points, &xyz, 0xFFFFFF);

	ft_fill_point(&xyz, j, i, (k + 1));
	p[4] = ft_add_vektor2(points, &xyz, 0xFFFFFF);
	ft_fill_point(&xyz, j, (i + 1), (k + 1));
	p[5] = ft_add_vektor2(points, &xyz, 0x0000FF);
	ft_fill_point(&xyz, (j + 1), (i + 1), (k + 1));
	p[6] = ft_add_vektor2(points, &xyz, 0xFFFFFF);
	ft_fill_point(&xyz, (j + 1), i, (k + 1));
	p[7] = ft_add_vektor2(points, &xyz, 0xFF0000);

	ft_create_cube_poligons(&(vis->plgn), p, 0xFFFF00);
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




void	ft_create_points_in_cells(t_vis *vis)
{
	t_point start;
	t_point end;

	ft_fill_point(&start, J0 + 1, I0 + 1, K0 + 1);
	ft_fill_point(&end, JMAX - 1, IMAX - 1, KMAX - 1);
	ft_cycle_cube((void *)vis, &ft_create_obstacles, &start, &end);
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
	if (!ft_put_pixel_to_img2(&(vis->pic), &vektr.zoom, pict->addr[shift * pict->size_line + shift]))
		return ;
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

	//ft_change_points5(vis);

	//ft_print_poligons(vis->plgn, vis->points, &(vis->pic), &(vis->param));
	ft_print_relief(g_earth, g_cell, &(vis->pic), &(vis->param));
	//ft_print_all_water(vis);
	//vis->is_rotate_or_csale = TRUE;



	//ft_init_picture(&(vis->pict), vis->param.len, WATER_COLOR);
	//ft_for_each_elem(g_iparts, print_img_to_img, (void *)vis);
	//free((void *)vis->pict.addr);

	ft_for_each_elem(g_iparts, ft_print_all_water2, (void *)vis);
	//vis->is_rotate_or_csale = FALSE;
	//ft_print_lines(vis, vis->lines);

	mlx_put_image_to_window(vis->mlx, vis->win, vis->pic.img, 0, 0);

}




int loop_hook(void *param)
{
	t_vis *vis;

	vis = (t_vis *)param;

	if (vis->param.exit)
		return (0);
	if (!ft_read_buffers(g_cl, INTERFACE, (void *)g_iparts->elems, g_iparts->elems_used * sizeof(t_ipart)))
		ft_del_all("read error\n");

	ft_refresh_picture(vis);

	printf("%ld\n", clock() - g_clock);
	g_clock = clock();

	return (0);
}



void	ft_prepare_to_compile(t_prog *compile, t_arr *parts)
{
	int i;

	i = 0;
	while (i < PROGRAMS_COUNT)
	{
		if (compile[i].arg_1 == PARTS)
			compile[i].global_work_size = parts->elems_used;
		i++;
	}
}





int main(int ac, char **av)
{

	if (!(ground = ft_read_ground_from_file3("demo.txt")))
		return(0);

	int **comlex_ground = ft_create_complex_ground_from_simple(ground);

/*
	int i2 = 0;
	int k = 0;
	while (k < KMAX + 2)
	{
		printf("%2d ", k);
		i2 = 0;
		while (i2 < IMAX + 2)
		{
			printf("%4d ", ground[k][i2]);

			i2++;
		}
		printf("\n");
		k++;
	}

	printf("\n");



	i2 = 0;
	k = 0;
	while (k < (KMAX + 2) * (ADD_POINT + 1))
	{
		i2 = 0;
		printf("%2d ", k);
		while (i2 < (IMAX + 2) * (ADD_POINT + 1))
		{
			printf("%4d ", comlex_ground[k][i2]);

			i2++;
		}
		printf("\n");
		k++;
	}*/


	//return (0);

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
	//ft_create_new_area_of_water(&g_parts, &((t_point){30, 30, 2}), &((t_point){30, 30, 3}), WATER);

	//ft_create_new_area_of_water(&g_parts, &((t_point){2, 2, 2}), &((t_point){JMAX - 50, 11, KMAX - 1}), WATER);
	//ft_create_new_area_of_water(&g_parts, &((t_point){2, IMAX - 11, 2}), &((t_point){JMAX - 50, IMAX - 1, KMAX - 1}), MAGMA);


	//ft_create_new_area_of_water(&g_parts, &((t_point){JMAX - 12, IMAX / 2 - 7, KMAX / 2 - 7}), &((t_point){JMAX - 1, IMAX / 2 + 7, KMAX / 2 + 7}), MAGMA);
	//ft_create_new_area_of_water(&g_parts, &((t_point){1, 2, 2}), &((t_point){7, IMAX - 1, KMAX - 1}), WATER);


	ft_fill_interface(g_parts, g_iparts);

	//создаем буферы и копируем в них инфу
	if (!ft_create_buffers(g_cl, PARTS, (void *)g_parts->elems, g_parts->elems_count * sizeof(t_part)))
		ft_del_all("buffer error\n");
	if (!ft_create_buffers(g_cl, CELLS, (void *)g_cell, CELL_COUNT * sizeof(t_cell)))
		ft_del_all("buffer error\n");
	if (!ft_create_buffers(g_cl, INTERFACE, (void *)g_iparts->elems, g_parts->elems_count * sizeof(t_ipart)))
		ft_del_all("buffer error\n");

	//привязываем аргументы к программам
	ft_prepare_to_compile(g_compile, g_parts);
	if (!ft_set_kernel_arg(g_cl, g_compile))
		ft_del_all("set error\n");


	ft_create_thread_for_solver(&solver, g_cl, &(vis->param));


	//g_clock = clock();

	mlx_hook(vis->win, 2, 0, deal_key, (void *)&(vis->param));
	mlx_loop_hook(vis->mlx, loop_hook, (void *)vis);
	mlx_loop(vis->mlx);

	return (0);
}
