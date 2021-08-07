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

int	ft_get_index(int j, int i, int k)
{
	if (i < I0 || i > IMAX || j < J0 || j > JMAX || k < K0 || k > KMAX)
		return (0);
	return (j * (IMAX + 2) * (KMAX + 2) + i * (KMAX + 2) + k);
}

t_point	ft_get_index_d3(int cell_number)
{
	t_point	cell;

	cell.y = cell_number / ((IMAX + 2) * (KMAX + 2));
	cell.x = cell_number % ((IMAX + 2) * (KMAX + 2)) / (KMAX + 2);
	cell.z = cell_number % (KMAX + 2);
	return (cell);
}

int	ft_is_cell_obstacle(int **ground, int cell_number)
{
	t_point	cell;
	int		j;
	int		i;
	int		k;

	cell = ft_get_index_d3(cell_number);
	j = cell.y;
	i = cell.x;
	k = cell.z;
	if (j == J0 || i == IMAX || i == I0 || k == K0 || k == KMAX)
		return (TRUE);
	if (j < J0 || j > JMAX || i > IMAX || i < I0 || k < K0 || k > KMAX)
		return (FALSE);
	if (j <= (int)ft_return_heigth(ground[k][i]))
		return (TRUE);
	return (FALSE);
}

void	ft_fill_cells_from_ground(t_arr *cells, int **ground)
{
	t_cell	*cell;
	t_iter	iter;
	int		n;

	n = 0;
	iter = get_arr_iter(cells);
	while (iter.get_next_elem(&iter))
	{
		cell = (t_cell *)iter.value;
		cell->obstacle = ft_is_cell_obstacle(ground, n);
		n++;
	}
	ft_cycle_cube(cells, ft_mark_extern_cell,
		&((t_point){J0, I0, K0}),
		&(t_point){JMAX, IMAX, KMAX});
}

void	ft_move_water_cell(t_arr *cells, t_param *param)
{
	t_point	p;
	t_point	start;
	t_point	end;
	t_cell	*cell;
	t_iter	iter;

	p = param->water;
	ft_fill_point(&start,
		p.y - param->brush,
		p.x - param->brush,
		p.z - param->brush);
	ft_fill_point(&end,
		p.y + param->brush,
		p.x + param->brush,
		p.z + param->brush);
	iter = get_arr_iter(cells);
	while (iter.get_next_elem(&iter))
	{
		cell = (t_cell *)iter.value;
		cell->water = FALSE;
	}
	ft_cycle_cube(cells->elems, ft_mark_cell_as_water, &start, &end);
	ft_cycle_cube(cells, ft_mark_extern_cell, &start, &end);
}
