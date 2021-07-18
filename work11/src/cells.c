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

#include "../includes/ft_mod1.h"

#define OBSTACLE 1
#define DYNAMIC_OBSTACLE 2

#define KEY_SPACE 49
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
	t_cell	*cell;
	t_iter	iter;
	int n;

	n = 0;
	iter = get_arr_iter(cells);
	while ((cell = (t_cell *)iter.get_next_elem(&iter)))
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


void	ft_print_one_edge(t_vektr **p, t_pict *pic, t_param *param, t_plgn *plgn)
{
	if (ft_vekt_cos(plgn->rot_n, (t_dpoint){0.0, 0.0, 1.0}) <= 0.0)
		return ;
	plgn->p[0] = p[0];
	plgn->p[1] = p[1];
	plgn->p[2] = p[2];
	plgn->p[3] = p[3];
	if (!ft_need_print_traing(plgn->p, pic))
		return ;
	ft_print_plgn(plgn, pic, param->grad);
	plgn->p[0] = p[3];
	plgn->p[1] = p[2];
	plgn->p[2] = p[0];
	ft_print_plgn(plgn, pic, param->grad);
}



void	ft_fill_points_cell3(t_vektr *p8, t_vektr **p)
{
	p[0] = &p8[0];
	p[1] = &p8[3];
	p[2] = &p8[2];
	p[3] = &p8[1];

	p[4] = &p8[4];
	p[5] = &p8[7];
	p[6] = &p8[3];
	p[7] = &p8[0];

	p[8] = &p8[0];
	p[9] = &p8[1];
	p[10] = &p8[5];
	p[11] = &p8[4];

	p[12] = &p8[4];
	p[13] = &p8[5];
	p[14] = &p8[6];
	p[15] = &p8[7];


	p[16] = &p8[1];
	p[17] = &p8[2];
	p[18] = &p8[6];
	p[19] = &p8[5];

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

void	ft_fill_plgns_cell4(t_plgn *plgn, t_param *param, int color)
{
	t_dpoint p;

	plgn[0].cos = -param->cos.z > 0.0 ? -param->cos.z : 0.0;
	plgn[1].cos = -param->cos.x > 0.0 ? -param->cos.x : 0.0;
	plgn[2].cos = -param->cos.y > 0.0 ? -param->cos.y : 0.0;
	plgn[3].cos = param->cos.z > 0.0 ? param->cos.z : 0.0;
	plgn[4].cos = param->cos.x > 0.0 ? param->cos.x : 0.0;
	plgn[5].cos = param->cos.y > 0.0 ? param->cos.y : 0.0;

	p = param->oxyz.oz;
	plgn[0].rot_n = (t_dpoint){-p.y, -p.x, -p.z};
	plgn[3].rot_n = p;
	p = param->oxyz.ox;
	plgn[1].rot_n = (t_dpoint){-p.y, -p.x, -p.z};
	plgn[4].rot_n = p;
	p = param->oxyz.oy;
	plgn[2].rot_n = (t_dpoint){-p.y, -p.x, -p.z};
	plgn[5].rot_n = p;

	plgn[0].color = color;
	plgn[1].color = color;
	plgn[2].color = color;
	plgn[3].color = color;
	plgn[4].color = color;
	plgn[5].color = color;
}


#define EDGE 4
#define EDGE_COUNT 6

void	ft_print_one_cell(t_point cell, t_pict *pic, t_param *param, int color)
{
	t_vektr *p[EDGE * EDGE_COUNT];
	t_vektr p8[8];
	t_plgn plgn[EDGE_COUNT];
	int i;

	ft_fill_points_cell2(cell, p8, param);
	ft_fill_points_cell3(p8, p);
	ft_fill_plgns_cell4(plgn, param, color);

	i = 0;
	while (i < EDGE_COUNT)
	{
		ft_print_one_edge(p + i * 4, pic, param, &plgn[i]);
		i++;
	}
}

void	ft_print_all_cell(t_arr *cells, t_pict *pic, t_param *param)
{
	t_cell	*cell;
	t_point	jik;
	t_iter	iter;
	int i;

	ft_memset((void *)pic->index, 0, pic->count_byte);
	i = 0;
	iter = get_arr_iter(cells);
	while ((cell = (t_cell *)iter.get_next_elem(&iter)))
	{
		pic->cell = i;
		jik = ft_get_index_d3(i);
		if (((cell->obstacle == 1 && !param->is_smooth_relief)
		|| cell->obstacle == DYNAMIC_OBSTACLE)
		&& ft_is_need_print_cell(cells, jik.y, jik.x, jik.z))
			ft_print_one_cell(jik, pic, param, OBSTACLES_FRONT_COLOR);
		i++;
	}
	pic->cell = 0;
}

void	ft_print_water_cell(t_arr *cells, t_pict *pic, t_param *param)
{
	t_cell *cell;
	t_point jik;
	t_iter	iter;
	int i;

	i = 0;
	iter = get_arr_iter(cells);
	while ((cell = (t_cell *)iter.get_next_elem(&iter)))
	{
		if (cell->water)
		{
			jik = ft_get_index_d3(i);
			ft_print_one_cell(jik, pic, param, WATER_COLOR);
		}
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

void	ft_mark_cell_as_water(void *ptr, int j, int i, int k)
{
	int cell_number;
	t_cell *cell;

	cell_number = ft_get_index(j, i, k);
	if (!cell_number)
		return ;
	cell = (t_cell *)ptr;
	if (!cell[cell_number].obstacle)
		cell[cell_number].water = TRUE;
}

int		ft_change_obstacles(t_arr *cells, int cell_number, int button, t_param *param)
{
	t_point p;
	t_point start;
	t_point end;

	p = ft_get_index_d3(cell_number);
	ft_fill_point(&start, p.y - param->brush, p.x - param->brush, p.z - param->brush);
	ft_fill_point(&end, p.y + param->brush, p.x + param->brush, p.z + param->brush);
	if (button == RIGHT_MOUSE)
		ft_cycle_cube(cells->elems, ft_del_cell, &start, &end);
	else if (button == LEFT_MOUSE)
		ft_cycle_cube(cells->elems, ft_add_cell, &start, &end);
	else
		return (FALSE);
	return (TRUE);
}

void	ft_move_water_cell(t_arr *cells, t_param *param)
{
	t_point	p;
	t_point	start;
	t_point	end;
	t_cell	*cell;
	t_iter	iter;

	p = param->water;
	ft_fill_point(&start, p.y - param->brush, p.x - param->brush, p.z - param->brush);
	ft_fill_point(&end, p.y + param->brush, p.x + param->brush, p.z + param->brush);
	iter = get_arr_iter(cells);
	while ((cell = (t_cell *)iter.get_next_elem(&iter)))
		cell->water = FALSE;
	ft_cycle_cube(cells->elems, ft_mark_cell_as_water, &start, &end);
}
