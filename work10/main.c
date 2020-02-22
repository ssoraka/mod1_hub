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
	{MAGMA, PART_H, 120.0, PART_MASS_0 * 1.0, 220000000.0, 1000.0, 00.0, 0.0},
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
		draw_line_img2(line, &(vis->pic), vis->grad);
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

	ft_fill_point(&delta, 1, 1, 1);
	if (end->y < start->y)
		delta.y = -1;
	if (end->x < start->x)
		delta.x = -1;
	if (end->z < start->z)
		delta.z = -1;
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





void	ft_print_all_water2(void *ptr, void *param)
{
	t_ipart *ipart;
	t_vis *vis;
	t_vektr vektr;

	vis = (t_vis *)param;
	ipart = (t_ipart *)ptr;
	ft_bzero((void *)&vektr, sizeof(t_vektr));
	ft_fill_dpoint(&vektr.abs, ipart->pos.y, ipart->pos.x, ipart->pos.z);
	ft_change_points4(vis, &vektr, TRUE);
	ft_print_rect2(&(vis->pic), &(vektr.zoom), g_color[ipart->type][RADIUS2], g_color[ipart->type][COLOR]);
}




void	ft_refresh_picture(t_vis *vis)
{
	mlx_clear_window(vis->mlx, vis->win);
	ft_bzero((void *)vis->pic.addr, CONST_WIDTH * CONST_HEINTH * 4);
	ft_memset((void *)vis->pic.z_buffer, 0x80, CONST_WIDTH * CONST_HEINTH * 4);

	//ft_change_points5(vis);
	if (!vis->is_need_print_obstacles)
		ft_print_poligons(vis, vis->plgn);
	//ft_print_all_water(vis);
	//vis->is_rotate_or_csale = TRUE;

	ft_for_each_elem(g_iparts, ft_print_all_water2, (void *)vis);
	//vis->is_rotate_or_csale = FALSE;
	//ft_print_lines(vis, vis->lines);

	mlx_put_image_to_window(vis->mlx, vis->win, vis->img, 0, 0);

}




int loop_hook(void *param)
{
	t_vis *vis;

	vis = (t_vis *)param;

	if (!ft_read_buffers(g_cl, INTERFACE, (void *)g_iparts->elems, g_iparts->elems_used * sizeof(t_ipart)))
		ft_del_all("read error\n");

	ft_refresh_picture(vis);

	printf("%ld\n", clock() - g_clock);
	g_clock = clock();

	return (0);
}



void	ft_print_int(void *param, int j, int i, int k)
{
	int ***arr;

	arr = (int ***)param;
	printf(" %2d", arr[j][i][k]);
	if (i == imax + 1)
		printf("\n");
}

void	ft_print_int2(void *param, int j, int i, int k)
{
	int **arr;

	arr = (int **)param;
	printf(" %2d", (int)(arr[i][k] * KMAX * MAP_KOEF / MAP_HEIGTH2));
	if (k == kmax - 1 )
		printf("\n");
}

void	ft_print_char(void *param, int j, int i, int k)
{
	char ***arr;

	arr = (char ***)param;
	printf(" %c", arr[j][i][k]);
	if (i == imax + 1)
		printf("\n");
}

void	ft_print_char2(void *param, int j, int i, int k)
{
	t_cell *arr;

	arr = (t_cell *)param;
	arr += ft_get_index(j, i, k);

	if (j == 0 && i == imax + 1)
	{
		printf("\n  1  2  3  4  5  6  7  8\n");
		return ;
	}

	if (arr->obstacle)
		printf("  %c",'X');
	else
		printf("  %c",' ');
	if (i == imax)
		printf("\n");
}

void	ft_print_real(void *param, int j, int i, int k)
{
	REAL ***arr;
	char *str;

	if (sizeof(REAL) == sizeof(double))
		str = " %6.2lf";
	else if (sizeof(REAL) == sizeof(float))
		str = " %6.2f";
	else
		return ;
	arr = (REAL ***)param;
	printf(str, arr[j][i][k]);
	if (i == imax + 1)
		printf("\n");
}

void	ft_print_arr(void *arr, void (*f)(void *, int, int, int), int k)
{
	t_point start;
	t_point end;

	ft_fill_point(&start, jmax, 1, k);
	ft_fill_point(&end, 0, imax, k);
	ft_cycle_cube(arr, f, &start, &end);
}

void	ft_print_ground(void *arr, void (*f)(void *, int, int, int), int k)
{
	t_point start;
	t_point end;

	ft_fill_point(&start, 0, 0, 0);
	ft_fill_point(&end, 0, imax -1 , kmax -1);
	ft_cycle_cube(arr, f, &start, &end);
}


void	*ft_solver(void *param)
{
	while (TRUE)
	{
		//запускаем программы
		if (vis->exit)
			break ;
		else if (!vis->pause)
			if (!ft_run_kernels(g_cl))
				ft_del_all("run error\n");
	}
	pthread_exit(0);
	return (NULL);
}





int main(int ac, char **av)
{
	ft_initialization_of_global_variable();


	if (!(ground = ft_read_ground_from_file3("demo.txt")))
		return(0);

	//создаем все массивы, проставляем начальные значения скоростей и давления
	//инициализируем все глобальные переменные


	//заполняем 3д карту с 2д рельефа
	ft_fill_cells_from_ground(g_cell, ground);

	//создаем модель для 3д карты
	ft_create_points_in_cells(vis);
	//ft_create_relief(vis, comlex_ground);

	ft_create_new_area_of_water(&g_parts, &((t_point){2, 2, 2}), &((t_point){JMAX - 50, 11, KMAX - 1}), WATER);
	ft_create_new_area_of_water(&g_parts, &((t_point){2, IMAX - 11, 2}), &((t_point){JMAX - 50, IMAX - 1, KMAX - 1}), WATER);

	g_count = g_parts->elems_used;


	int i = 0;
	while (i < PROGRAMS_COUNT)
	{
		if (g_compile[i].arg_1 == PARTS)
			g_compile[i].global_work_size = g_count;
		i++;
	}


	ft_fill_interface(g_parts, g_iparts);

	//создаем буферы и копируем в них инфу
	if (!ft_create_buffers(g_cl, PARTS, (void *)g_parts->elems, g_parts->elems_used * sizeof(t_part)))
		ft_del_all("buffer error\n");
	if (!ft_create_buffers(g_cl, CELLS, (void *)g_cell, CELL_COUNT * sizeof(t_cell)))
		ft_del_all("buffer error\n");
	if (!ft_create_buffers(g_cl, INTERFACE, (void *)g_iparts->elems, g_parts->elems_used * sizeof(t_ipart)))
		ft_del_all("buffer error\n");

	//привязываем аргументы к программам
	if (!ft_set_kernel_arg(g_cl, g_compile))
		ft_del_all("set error\n");

	pthread_t tid; /* идентификатор потока */
	pthread_attr_t attr; /* атрибуты потока */
	pthread_attr_init(&attr);
	pthread_create(&tid, &attr, ft_solver, NULL);

	//g_clock = clock();

	mlx_hook(vis->win, 2, 0, deal_key, (void *)vis);
	mlx_loop_hook(vis->mlx, loop_hook, (void *)vis);
	mlx_loop(vis->mlx);

	return (0);
}
