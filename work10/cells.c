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
		return (TRUE);
	if (j < J0 || j > JMAX || i > IMAX || i < I0 || k < K0 || k > KMAX)
		return (FALSE);
	if (i == 25 && j < 7)
		return (TRUE);
	//хотел исправить ground[k - 1][i - 1] на ground[k][i],
	//но надо модуль map.c переписать
	#ifdef EMPTY_MAP
	if (j <= (int)ft_return_heigth(ground[k][i]))
		return (TRUE);
	#endif
	return (FALSE);
}



void	ft_fill_cells_from_ground(t_cell *cells, int **ground)
{
	int n;

	n = 0;
	while (n <= CELL_COUNT)
	{
		cells[n].obstacle = ft_is_cell_obstacle(ground, n);
		n++;
	}
}

int		ft_is_need_print_cell(t_cell *cell, int j, int i, int k)
{
	if (j <= J0 || j >= JMAX || i <= I0 || i >= IMAX || k <= K0 || k >= KMAX)
		return (FALSE);
	if (j == J0 + 1 || j == JMAX - 1 || i == I0 + 1 || i == IMAX - 1 || k == KMAX - 1)
		return (TRUE);
	if (g_cell[ft_get_index(j + 1, i, k)].obstacle
	&& g_cell[ft_get_index(j - 1, i, k)].obstacle
	&& g_cell[ft_get_index(j, i + 1, k)].obstacle
	&& g_cell[ft_get_index(j, i - 1, k)].obstacle
	&& g_cell[ft_get_index(j, i, k + 1)].obstacle
	&& g_cell[ft_get_index(j, i, k - 1)].obstacle)
		return (FALSE);
	return (TRUE);
}


void	ft_print_one_edge(t_vektr *p, t_pict *pic, t_param *param, int color)
{
	t_plgn plgn;

	plgn.color = color;
	plgn.p[0] = &(p[0]);
	plgn.p[1] = &(p[1]);
	plgn.p[2] = &(p[2]);
	plgn.p[3] = &(p[3]);
	if (!ft_need_print_traing(plgn.p, pic))
		return ;
	ft_print_plgn(&plgn, pic, param->grad);
	plgn.p[0] = &(p[3]);
	plgn.p[1] = &(p[2]);
	plgn.p[2] = &(p[0]);
	ft_print_plgn(&plgn, pic, param->grad);
}


void	ft_fill_points_cell(t_point cell, t_vektr *p)
{
	ft_fill_dpoint(&p[0].abs, cell.y, cell.x, cell.z);
	ft_fill_dpoint(&p[1].abs, cell.y, cell.x + 1, cell.z);
	ft_fill_dpoint(&p[2].abs, cell.y + 1, cell.x + 1, cell.z);
	ft_fill_dpoint(&p[3].abs, cell.y + 1, cell.x, cell.z);

	ft_fill_dpoint(&p[4].abs, cell.y, cell.x, cell.z);
	ft_fill_dpoint(&p[5].abs, cell.y, cell.x, cell.z + 1);
	ft_fill_dpoint(&p[6].abs, cell.y + 1, cell.x, cell.z + 1);
	ft_fill_dpoint(&p[7].abs, cell.y + 1, cell.x, cell.z);

	ft_fill_dpoint(&p[8].abs, cell.y, cell.x, cell.z);
	ft_fill_dpoint(&p[9].abs, cell.y, cell.x, cell.z + 1);
	ft_fill_dpoint(&p[10].abs, cell.y, cell.x + 1, cell.z + 1);
	ft_fill_dpoint(&p[11].abs, cell.y, cell.x + 1, cell.z);

	ft_fill_dpoint(&p[12].abs, cell.y, cell.x, cell.z + 1);
	ft_fill_dpoint(&p[13].abs, cell.y, cell.x + 1, cell.z + 1);
	ft_fill_dpoint(&p[14].abs, cell.y + 1, cell.x + 1, cell.z + 1);
	ft_fill_dpoint(&p[15].abs, cell.y + 1, cell.x, cell.z + 1);

	ft_fill_dpoint(&p[16].abs, cell.y, cell.x + 1, cell.z);
	ft_fill_dpoint(&p[17].abs, cell.y, cell.x + 1, cell.z + 1);
	ft_fill_dpoint(&p[18].abs, cell.y + 1, cell.x + 1, cell.z + 1);
	ft_fill_dpoint(&p[19].abs, cell.y + 1, cell.x + 1, cell.z);

	ft_fill_dpoint(&p[20].abs, cell.y + 1, cell.x, cell.z);
	ft_fill_dpoint(&p[21].abs, cell.y + 1, cell.x, cell.z + 1);
	ft_fill_dpoint(&p[22].abs, cell.y + 1, cell.x + 1, cell.z + 1);
	ft_fill_dpoint(&p[23].abs, cell.y + 1, cell.x + 1, cell.z);
}

#define EDGE 4
#define EDGE_COUNT 6

void	ft_print_one_cell(t_point cell, t_pict *pic, t_param *param)
{
	t_vektr p[EDGE * EDGE_COUNT];
	int i;

	ft_fill_points_cell(cell, p);
	i = 0;
	while (i < EDGE * EDGE_COUNT)
	{
		p[i].color = ft_grad_color(p[i].abs.y, JMAX - J0, COLOR_UP, COLOR_DOWN);
		ft_change_points4(param, &p[i]);
		i++;
	}
	i = 0;
	while (i < 2)
	{
		ft_print_one_edge(p + i * 12, pic, param, OBSTACLES_FRONT_COLOR);
		ft_print_one_edge(p + i * 12 + 4, pic, param, OBSTACLES_TOP_COLOR);
		ft_print_one_edge(p + i * 12 + 8, pic, param, OBSTACLES_RIGHT_COLOR);
		i++;
	}
}



void	ft_print_all_cell(t_cell *cell, t_pict *pic, t_param *param)
{
	t_point jik;
	int i;

	i = 0;
	while (i < CELL_COUNT)
	{
		jik = ft_get_index_d3(i);
		if (cell[i].obstacle
		&& ft_is_need_print_cell(cell, jik.y, jik.x, jik.z))
			ft_print_one_cell(jik, pic, param);
		i++;
	}
}
