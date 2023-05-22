/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cells.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/27 18:12:08 by ssoraka           #+#    #+#             */
/*   Updated: 2019/10/27 18:12:08 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_mod1.h"

int	ft_is_need_print_cell(t_arr *cells, int j, int i, int k)
{
	t_cell	*cell;

	cell = cells->elems;
	if (j <= J0 || j >= JMAX || i <= I0 || i >= IMAX || k <= K0 || k >= KMAX)
		return (FALSE);
	if (j == JMAX - 1 || i == I0 + 1 || i == IMAX - 1
		|| k == K0 + 1 || k == KMAX - 1)
		return (TRUE);
	if (cell[ft_get_index(j + 1, i, k)].obstacle
		&& cell[ft_get_index(j - 1, i, k)].obstacle
		&& cell[ft_get_index(j, i + 1, k)].obstacle
		&& cell[ft_get_index(j, i - 1, k)].obstacle
		&& cell[ft_get_index(j, i, k + 1)].obstacle
		&& cell[ft_get_index(j, i, k - 1)].obstacle)
		return (FALSE);
	if (cell[ft_get_index(j + 1, i, k)].water
		&& cell[ft_get_index(j - 1, i, k)].water
		&& cell[ft_get_index(j, i + 1, k)].water
		&& cell[ft_get_index(j, i - 1, k)].water
		&& cell[ft_get_index(j, i, k + 1)].water
		&& cell[ft_get_index(j, i, k - 1)].water)
		return (FALSE);
	return (TRUE);
}

void	ft_print_all_cell(t_arr *cells, t_pict *pic, t_param *param)
{
	t_cell	*cell;
	t_point	jik;
	t_iter	iter;
	int		i;
	t_prop	prop;

	ft_memset((void *)pic->index, 0, pic->count_byte);
	i = 0;
	prop = set_param(DEFAULT_POINT | (GRADIENT * param->grad), i,
			OBSTACLES_FRONT_COLOR);
	iter = get_arr_iter(cells);
	while (iter.get_next_elem(&iter))
	{
		cell = (t_cell *)iter.value;
		prop.index = i;
		jik = ft_get_index_d3(i);
		if (cell->obstacle && cell->surface == 1
			&& param->is_smooth_relief != SMOOTH_EARTH)
			print_one_cell(jik, pic, param, prop);
		i++;
	}
}

void	ft_print_water_cell(t_arr *cells, t_pict *pic, t_param *param)
{
	t_cell	*cell;
	t_point	jik;
	t_iter	iter;
	int		i;
	t_prop	prop;

	i = 0;
	prop = set_param(DEFAULT_IMAGE, 0, WATER_COLOR);
	iter = get_arr_iter(cells);
	while (iter.get_next_elem(&iter))
	{
		cell = (t_cell *)iter.value;
		if (cell->water && cell->surface)
		{
			jik = ft_get_index_d3(i);
			print_one_cell(jik, pic, param, prop);
		}
		i++;
	}
}
