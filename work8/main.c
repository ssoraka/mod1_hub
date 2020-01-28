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


void	ft_print_points(t_vis *vis, t_part *points)
{
	while (points)
	{
		ft_change_points4(vis, &(points->pos), vis->is_rotate_or_csale);
		//if (points->tension.is_surface)
		//	ft_print_rect2(&(vis->pic), &(points->pos.zoom), RADIUS, 0xFF0000);
		//else
			ft_print_rect2(&(vis->pic), &(points->pos.zoom), RADIUS, points->pos.color);
		points = points->next;
	}
}

void	ft_print_points2(t_vis *vis, t_vektr *points)
{
	while (points)
	{
		circle2(&(vis->pic), points, RADIUS + 4, WATER_COLOR);
		points = points->next;
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

	if (map[j][i][k] != OBSTACLES)
		return ;
	if (map[j + 1][i][k] == OBSTACLES && map[j - 1][i][k] == OBSTACLES
	&& map[j][i + 1][k] == OBSTACLES && map[j][i - 1][k] == OBSTACLES
	&& map[j][i][k + 1] == OBSTACLES && map[j][i][k - 1] == OBSTACLES)
		return ;
	vis = (t_vis *)ptr;
	points = (void *)(&(vis->points));
	ft_fill_point(&xyz, j * delta.y, i * delta.x, k * delta.z);
	p[0] = ft_add_vektor2(points, &xyz, 0xFF0000);
	ft_fill_point(&xyz, j * delta.y, (i + 1) * delta.x, k * delta.z);
	p[1] = ft_add_vektor2(points, &xyz, 0x00FF00);
	ft_fill_point(&xyz, (j + 1) * delta.y, (i + 1) * delta.x, k * delta.z);
	p[2] = ft_add_vektor2(points, &xyz, 0xFFFFFF);
	ft_fill_point(&xyz, (j + 1) * delta.y, i * delta.x, k * delta.z);
	p[3] = ft_add_vektor2(points, &xyz, 0xFFFFFF);

	ft_fill_point(&xyz, j * delta.y, i * delta.x, (k + 1) * delta.z);
	p[4] = ft_add_vektor2(points, &xyz, 0xFFFFFF);
	ft_fill_point(&xyz, j * delta.y, (i + 1) * delta.x, (k + 1) * delta.z);
	p[5] = ft_add_vektor2(points, &xyz, 0x0000FF);
	ft_fill_point(&xyz, (j + 1) * delta.y, (i + 1) * delta.x, (k + 1) * delta.z);
	p[6] = ft_add_vektor2(points, &xyz, 0xFFFFFF);
	ft_fill_point(&xyz, (j + 1) * delta.y, i * delta.x, (k + 1) * delta.z);
	p[7] = ft_add_vektor2(points, &xyz, 0xFF0000);

	ft_create_cube_poligons(&(vis->plgn), p, 0xFFFF00);
}










void	ft_cycle_cube(void *param, void (*f)(void *, int, int, int), t_point *start, t_point *end)
{
	int k;
	int j;
	int i;
	t_point delta;

	//ft_fill_point(&delta, ft_znak(end->y - start->y), ft_znak(end->x - start->x), ft_znak(end->z - start->z));
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


void	ft_mark_obstacles_on_map(void *param, int j, int i, int k)
{
	if (j <= (ground[k - 1][i - 1] * KMAX * MAP_KOEF) / MAP_HEIGTH2)
		map[j][i][k] = OBSTACLES;
	else
		map[j][i][k] = EMPTY;
}



void	ft_fill_map_from_ground(int **ground)
{
	t_point start;
	t_point end;

	ft_fill_point(&start, 1, 1, 1);
	ft_fill_point(&end, jmax, imax, kmax);
	ft_cycle_cube(NULL, &ft_mark_obstacles_on_map, &start, &end);
}


void	ft_create_points_in_cells(t_vis *vis)
{
	t_point start;
	t_point end;

	ft_fill_point(&start, J0 + 1, I0 + 1, K0 + 1);
	ft_fill_point(&end, JMAX - 1, IMAX - 1, KMAX - 1);
	ft_cycle_cube((void *)vis, &ft_create_obstacles, &start, &end);
}




void	ft_print_water_in_cell(void *param, int j, int i, int k)
{
	t_vis *vis;

	vis = (t_vis *)param;
	ft_print_points(vis, parts[j][i][k]);
}



void	ft_print_all_water(t_vis *vis)
{
	t_point start;
	t_point end;

	vis->is_rotate_or_csale = TRUE;
	ft_fill_point(&start, 1, 1, 1);
	ft_fill_point(&end, jmax, imax, kmax);
	ft_cycle_cube((void *)vis, &ft_print_water_in_cell, &start, &end);
	vis->is_rotate_or_csale = FALSE;
}

void	ft_print_all_water2(void *ptr, void *param)
{
	t_part *part;
	t_vis *vis;

	vis = (t_vis *)param;
	part = (t_part *)ptr;

	int rad = RADIUS;
	ft_change_points4(vis, &(part->pos), TRUE);
	if (part->type == OBSTACLES || part->type == MAGMA)
		rad *= 2;
	ft_print_rect2(&(vis->pic), &(part->pos.zoom), rad, part->pos.color);
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

	ft_for_each_ptr(g_parts, ft_print_all_water2, (void *)vis);
	//vis->is_rotate_or_csale = FALSE;
	//ft_print_lines(vis, vis->lines);

	mlx_put_image_to_window(vis->mlx, vis->win, vis->img, 0, 0);

}






void	ft_print_int(void *param, int j, int i, int k)
{
	int ***arr;

	arr = (int ***)param;
	printf(" %2d", arr[j][i][k]);
	if (i == imax + 1)
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

	ft_fill_point(&start, jmax + 1, 0, k);
	ft_fill_point(&end, 0, imax + 1, k);
	ft_cycle_cube(arr, f, &start, &end);
}




int loop_hook(void *param)
{
	t_vis *vis;
	int i;

	vis = (t_vis *)param;
	i = 0;

	if (!vis->pause)
	{

		/*while (i < IMAX)
		{
			if (parts[39][i][2] || parts[38][i][2])
				rain = FALSE;
			i++;
		}*/
		//if (rain)
		//	ft_create_new_area_of_water(&g_parts, &((t_point){39, 2, 2}), &((t_point){39, 39, 3}), BLOB);
		//if (vis->rain && !parts[35][2][2] && !parts[34][2][2])
		//	ft_create_new_area_of_water(&g_parts, &((t_point){35, 2, 2}), &((t_point){35, 39, 39}), BLOB);

		if (vis->rain)
		{
			ft_create_new_area_of_water(&g_parts, &((t_point){26, 17, 17}), &((t_point){32, 24, 24}), MAGMA);
			vis->rain = !vis->rain;
		}
		// if (vis->rain)// && !parts[34][20][20])
		// 	ft_create_new_area_of_water(&g_parts, &((t_point){35, 20, 20}), &((t_point){35, 20, 20}), BLOB);
		// if (vis->rain)// && !parts[34][20][20])
		// {
		// 	ft_create_new_area_of_water(&g_parts, &((t_point){35, 10, 10}), &((t_point){35, 11, 11}), BLOB);
		// 	ft_create_new_area_of_water(&g_parts, &((t_point){35, 30, 10}), &((t_point){35, 31, 11}), BLOB);
		// 	ft_create_new_area_of_water(&g_parts, &((t_point){35, 10, 30}), &((t_point){35, 11, 31}), BLOB);
		// 	ft_create_new_area_of_water(&g_parts, &((t_point){35, 30, 30}), &((t_point){35, 31, 31}), BLOB);
		// }

		i = 0;
		while(i < 20)
		{
			//ft_create_new_area_of_water(&g_parts, &((t_point){35, 20, 20}), &((t_point){35, 21, 20}), BLOB);
			ft_solve_and_move_parts();
			i++;
		}

	}

	/*ft_print_arr(flags, &ft_print_int, 6);
	ft_print_arr(map, &ft_print_char, 6);
	//ft_print_arr(flags_surface, &ft_print_int, 6);
	ft_print_arr(press_p, &ft_print_real, 6);
	ft_print_arr(speed_v, &ft_print_real, 6);*/
	//ft_print_flags(fluid, fluid->flags);
	//
	//ft_print_fluid(fluid, fluid->speed_v);
	//printf("%d_%d\n", fluid->jmax, fluid->imax);
	/*int j = 1;
	while (j <= jmax)
	{
		printf("%s\n", map[j][2] + 1);
		j++;
	}*/
	//ft_print_fluid(fluid, fluid->speed_v);
	//ft_print_flags(fluid, fluid->flags);
	//ft_print_flags2(fluid, fluid->flags, C_X);
	ft_refresh_picture(vis);

	//ft_new_pos_of_points(vis);


	printf("%ld\n", clock() - g_clock);
	g_clock = clock();

	return (0);
}


void	ft_del_each_parts(void *param, int j, int i, int k)
{
	ft_del_part(&(parts[j][i][k]));
}




void ft_del_all_print_error(char *msg_error)
{
	t_point start;
	t_point end;

	ft_fill_point(&start, 0, 0, 0);
	ft_fill_point(&end, jmax + 1, imax + 1, kmax + 1);
	ft_cycle_cube(NULL, &ft_del_each_parts, &start, &end);
	if (vis)
	{
		free(vis->pic.addr);
		free(vis->pic.z_buffer);
		ft_del_vektor(&(vis->points));
		ft_del_lines(&(vis->lines));
		free(vis);
	}
	if (msg_error)
		ft_putendl_fd(msg_error, 2);
	ft_del_variable();
}




void	ft_create_stable_level_of_water(void *param, int j, int i, int k)
{
	int num;

	num = PARTS_COUNT;

	if (j == J0 || i == IMAX || i == I0 || k == K0 || k == KMAX)
	{
		map[j][i][k] = OBSTACLES;
		//ft_create_new_water_in_cell((void *)(&num), j, i, k);
	}
	//else if (i == 20 && !(k % 3))
	//	map[j][i][k] = OBSTACLES;
	/*else if (map[j][i][k] == EMPTY && j < 20 && j > J0 + 2// TEST_WATER_LEVEL + 8
	&& ((i > 25))
	&& k > TEST_WATER_WALL + K0 +10 && k < TEST_WATER_WALL + 30)
		map[j][i][k] = WATER;*/
	/*else if (map[j][i][k] == EMPTY && j < 20 && j > J0 + 2// TEST_WATER_LEVEL + 8
	&& ((i < 15))
	&& k > TEST_WATER_WALL + K0 +10 && k < TEST_WATER_WALL + 30)
		map[j][i][k] = BLOB;*/
	//else
	//	map[j][i][k] = EMPTY;
	//else if (map[j][i][k] == OBSTACLES || j == 1)


}


void	ft_create_first_water(void)
{

	t_point start;
	t_point end;

	ft_fill_point(&start, J0, I0, K0);
	ft_fill_point(&end, JMAX, IMAX, KMAX);
	ft_cycle_cube(NULL, &ft_create_stable_level_of_water, &start, &end);
}





int main(int ac, char **av)
{
	//ft_check();
	//return (0);
	/*double i = -2.0;
	while( i < 3.0){
		printf("%lf_%lf\n", i, ft_derivative_kernel_function2(0.5, i));
		i += 0.1;
	}
	exit(0);*/

	vis = ft_memalloc(sizeof(t_vis));
	ft_create_xyz(vis);
	vis->ang.z = M_PI;
	//vis->is_rotate_or_csale = TRUE;


	if (!(ground = ft_read_ground_from_file3("demo.txt")))
		return(0);


	/*int i;
	int k;

	k = 0;
	while (k < KMAX)
	{
		i = 0;
		while (i < IMAX)
		{
			printf("%d ", ground[k][i]);
			i++;
		}
		printf("\n");
		k++;
	}*/

	/*int **comlex_ground;

	comlex_ground = ft_create_complex_ground_from_simple(ground);
	k = 0;
	while (k < KMAX * (1 + ADD_POINT))
	{
		i = 0;
		while (i < IMAX * (1 + ADD_POINT))
		{
			printf("%d ", comlex_ground[k][i]);
			i++;
		}
		printf("\n");
		k++;
	}*/




	//создаем все массивы, проставляем начальные значения скоростей и давления
	//инициализируем все глобальные переменные
	ft_initialization_of_global_variable();


	//заполняем 3д карту с 2д рельефа
	ft_fill_map_from_ground(ground);


	/*int num = PARTS_COUNT;
	map[2][2][2] = WATER;
	ft_create_new_water_in_cell((void *)(&num), 2, 2, 2);
	map[2][3][2] = WATER;
	ft_create_new_water_in_cell((void *)(&num), 2, 3, 2);
	map[2][2][3] = WATER;
	ft_create_new_water_in_cell((void *)(&num), 2, 2, 3);
	map[2][3][3] = WATER;
	ft_create_new_water_in_cell((void *)(&num), 2, 3, 3);
	map[3][2][2] = WATER;
	ft_create_new_water_in_cell((void *)(&num), 3, 2, 2);
	map[4][2][2] = WATER;
	ft_create_new_water_in_cell((void *)(&num), 4, 2, 2);*/
	//ft_create_first_water();
	/*int num = PARTS_COUNT;
	map[4][2][2] = WATER;
	ft_create_new_water_in_cell((void *)(&num), 4, 2, 2);*/
	//ft_init_first_value();
	//ft_solver();

	//////
	ft_create_first_water();
	//////

	//создаем модель для 3д карты
	ft_create_points_in_cells(vis);
	//ft_create_relief(vis, comlex_ground);

	//srand(time(NULL));

	//ft_create_points_in_cells(vis);


	//
	ft_create_new_area_of_water(&g_parts, &((t_point){2, 2, 2}), &((t_point){20, 15, 38}), WATER);
	//

	//while (1)
	//	ft_solve_and_move_parts();


	/*t_part *p = parts[20][16][16];
	int n = 0;
	while (p)
	{
		n++;
		p = p->next;
	}
	ft_putnbr(n);
	exit(0);*/
	////////
	//ft_init_first_value_of_part_parameters();
	////////
	//ft_solve_and_move_parts();
	//создаем имейдж и z-буфер
	ft_create_img(vis);
	//g_clock = clock();

	mlx_hook(vis->win, 2, 0, deal_key, (void *)vis);
	mlx_loop_hook(vis->mlx, loop_hook, (void *)vis);
	mlx_loop(vis->mlx);

	return (0);
}
