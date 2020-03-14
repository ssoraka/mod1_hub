/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keys.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/27 18:12:08 by ssoraka           #+#    #+#             */
/*   Updated: 2019/10/27 18:12:08 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_mod1.h"

#define OBSTACLE 1
#define DYNAMIC_OBSTACLE 2

#define RIGHT_MOUSE 2
#define LEFT_MOUSE 1

int		ft_get_index(int j, int i, int k)
{
	if (i < I0 || i > IMAX || j < J0 || j > JMAX || k < K0 || k > KMAX)
		return (0);
	return (j * (IMAX + 2) * (KMAX + 2) + i * (KMAX + 2) + k);
}

t_point	ft_get_index_d3(int cell_number)
{
	t_point cell;

	cell.y = cell_number / ((IMAX + 2) * (KMAX + 2));
	cell.x = cell_number % ((IMAX + 2) * (KMAX + 2)) / (KMAX + 2);
	cell.z = cell_number % (KMAX + 2);
	return (cell);
}

int		ft_is_cell_obstacle(int **ground, int cell_number)
{
	t_point cell;
	int j;
	int i;
	int k;

	cell = ft_get_index_d3(cell_number);
	j = cell.y;
	i = cell.x;
	k = cell.z;
	if (j == J0 || i == IMAX || i == I0 || k == K0 || k == KMAX)
		return (OBSTACLE);

	// if (j == J0 + 1 && i == I0 + 1 && k == K0 + 1)
	// 	return (OBSTACLE);
	// return (FALSE);

	if (j < J0 || j > JMAX || i > IMAX || i < I0 || k < K0 || k > KMAX)
		return (FALSE);
	//хотел исправить ground[k - 1][i - 1] на ground[k][i],
	//но надо модуль map.c переписать
	#ifdef EMPTY_MAP
	if (j <= (int)ft_return_heigth(ground[k][i]))
		return (OBSTACLE);
	#endif
	if (i == 25 && j < 7)
		return (DYNAMIC_OBSTACLE);
	return (FALSE);
}



void	ft_fill_cells_from_ground(t_arr *cells, int **ground)
{
	t_cell *cell;
	int n;

	n = 0;
	while ((cell = ft_arr_get_next(cells)))
	{
		cell->obstacle = ft_is_cell_obstacle(ground, n);
		n++;
	}
}

int		ft_is_need_print_cell(t_arr *cells, int j, int i, int k)
{
	t_cell *cell;

	cell = cells->elems;
	if (j <= J0 || j >= JMAX || i <= I0 || i >= IMAX || k <= K0 || k >= KMAX)
		return (FALSE);
	if (j == JMAX - 1 || i == I0 + 1 || i == IMAX - 1 || k == K0 + 1 || k == KMAX - 1)
		return (TRUE);
	if (cell[ft_get_index(j + 1, i, k)].obstacle
	&& cell[ft_get_index(j - 1, i, k)].obstacle
	&& cell[ft_get_index(j, i + 1, k)].obstacle
	&& cell[ft_get_index(j, i - 1, k)].obstacle
	&& cell[ft_get_index(j, i, k + 1)].obstacle
	&& cell[ft_get_index(j, i, k - 1)].obstacle)
		return (FALSE);
	return (TRUE);
}


void	ft_print_one_edge(t_vektr **p, t_pict *pic, t_param *param, REAL cos)
{
	t_plgn plgn;

	plgn.color = OBSTACLES_FRONT_COLOR;
	plgn.p[0] = p[0];
	plgn.p[1] = p[1];
	plgn.p[2] = p[2];
	plgn.p[3] = p[3];
	if (!ft_need_print_traing(plgn.p, pic))
		return ;
	ft_print_plgn(&plgn, pic, param->grad, cos);
	plgn.p[0] = p[3];
	plgn.p[1] = p[2];
	plgn.p[2] = p[0];
	ft_print_plgn(&plgn, pic, param->grad, cos);
}



void	ft_fill_points_cell3(t_vektr *p8, t_vektr **p)
{
	p[0] = &p8[0];
	p[1] = &p8[1];
	p[2] = &p8[2];
	p[3] = &p8[3];

	p[4] = &p8[0];
	p[5] = &p8[4];
	p[6] = &p8[7];
	p[7] = &p8[3];

	p[8] = &p8[0];
	p[9] = &p8[4];
	p[10] = &p8[5];
	p[11] = &p8[1];

	p[12] = &p8[4];
	p[13] = &p8[5];
	p[14] = &p8[6];
	p[15] = &p8[7];


	p[16] = &p8[1];
	p[17] = &p8[5];
	p[18] = &p8[6];
	p[19] = &p8[2];

	p[20] = &p8[3];
	p[21] = &p8[7];
	p[22] = &p8[6];
	p[23] = &p8[2];
}

void	ft_fill_points_cell2(t_point cell, t_vektr *p, t_param *param)
{
	ft_fill_dpoint(&p[0].abs, cell.y, cell.x, cell.z);
	ft_fill_dpoint(&p[1].abs, cell.y, cell.x + 1, cell.z);
	ft_fill_dpoint(&p[2].abs, cell.y + 1, cell.x + 1, cell.z);
	ft_fill_dpoint(&p[3].abs, cell.y + 1, cell.x, cell.z);
	ft_fill_dpoint(&p[4].abs, cell.y, cell.x, cell.z + 1);
	ft_fill_dpoint(&p[5].abs, cell.y, cell.x + 1, cell.z + 1);
	ft_fill_dpoint(&p[6].abs, cell.y + 1, cell.x + 1, cell.z + 1);
	ft_fill_dpoint(&p[7].abs, cell.y + 1, cell.x, cell.z + 1);

	p[0].color = ft_grad_color(p[0].abs.y, JMAX - J0, COLOR_UP, COLOR_DOWN);
	p[1].color = p[0].color;
	p[2].color = ft_grad_color(p[2].abs.y, JMAX - J0, COLOR_UP, COLOR_DOWN);
	p[3].color = p[2].color;
	p[4].color = p[0].color;
	p[5].color = p[0].color;
	p[6].color = p[2].color;
	p[7].color = p[2].color;

	ft_change_points4(param, &p[0]);
	ft_change_points4(param, &p[1]);
	ft_change_points4(param, &p[2]);
	ft_change_points4(param, &p[3]);
	ft_change_points4(param, &p[4]);
	ft_change_points4(param, &p[5]);
	ft_change_points4(param, &p[6]);
	ft_change_points4(param, &p[7]);
}

#define EDGE 4
#define EDGE_COUNT 6

void	ft_print_one_cell(t_point cell, t_pict *pic, t_param *param)
{
	t_vektr *p[EDGE * EDGE_COUNT];
	t_vektr p8[8];
	int i;

	ft_fill_points_cell2(cell, p8, param);
	ft_fill_points_cell3(p8, p);
	ft_print_one_edge(p, pic, param, -param->cos.z);
	ft_print_one_edge(p + 12, pic, param, param->cos.z);
	ft_print_one_edge(p + 4, pic, param, -param->cos.x);
	ft_print_one_edge(p + 16, pic, param, param->cos.x);
	ft_print_one_edge(p + 8, pic, param, -param->cos.y);
	ft_print_one_edge(p + 20, pic, param, param->cos.y);
	i = 0;
	while (i < 2)
	{
		//ft_print_one_edge(p + i * 12, pic, param, OBSTACLES_FRONT_COLOR);
		//ft_print_one_edge(p + i * 12 + 4, pic, param, OBSTACLES_TOP_COLOR);
		//ft_print_one_edge(p + i * 12 + 8, pic, param, OBSTACLES_RIGHT_COLOR);
		i++;
	}
}

void	ft_print_all_cell(t_arr *cells, t_pict *pic, t_param *param)
{
	t_cell *cell;
	t_point jik;
	int i;

	ft_memset((void *)pic->index, 0, pic->count_byte);
	i = 0;
	while ((cell = ft_arr_get_next(cells)))
	{
		pic->cell = i;
		jik = ft_get_index_d3(i);
		if (((cell->obstacle == 1 && !param->is_smooth_relief)
		|| cell->obstacle == DYNAMIC_OBSTACLE)
		&& ft_is_need_print_cell(cells, jik.y, jik.x, jik.z))
			ft_print_one_cell(jik, pic, param);
		i++;
	}
	pic->cell = 0;
}

void	ft_del_cell(void *ptr, int j, int i, int k)
{
	int cell_number;
	t_cell *cell;

	if (i == I0 || i == IMAX || j == J0 || j == JMAX || k == K0 || k == KMAX)
		return ;
	cell_number = ft_get_index(j, i, k);
	if (!cell_number)
		return ;
	cell = (t_cell *)ptr;
	cell[cell_number].obstacle = FALSE;
}

void	ft_add_cell(void *ptr, int j, int i, int k)
{
	int cell_number;
	t_cell *cell;

	cell_number = ft_get_index(j, i, k);
	if (!cell_number)
		return ;
	cell = (t_cell *)ptr;
	cell[cell_number].obstacle = DYNAMIC_OBSTACLE;
}

int		ft_change_obstacles(t_arr *cell, int cell_number, int button, t_param *param)
{
	t_point p;
	t_point start;
	t_point end;
	t_cell *cells;

	cells = cell->elems;
	p = ft_get_index_d3(cell_number);
	ft_fill_point(&start, p.y - param->brush, p.x - param->brush, p.z - param->brush);
	ft_fill_point(&end, p.y + param->brush, p.x + param->brush, p.z + param->brush);
	if (button == RIGHT_MOUSE)
		ft_cycle_cube((void *)cells, ft_del_cell, &start, &end);
	else if (button == LEFT_MOUSE)
		ft_cycle_cube((void *)cells, ft_add_cell, &start, &end);
	else
		return (FALSE);
	return (TRUE);
}
