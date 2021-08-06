/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cell_change.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/27 18:12:08 by ssoraka           #+#    #+#             */
/*   Updated: 2019/10/27 18:12:08 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_mod1.h"

void	ft_del_cell(void *ptr, int j, int i, int k)
{
	int		cell_number;
	t_cell	*cell;

	if (i == I0 || i == IMAX || j == J0 || j == JMAX || k == K0 || k == KMAX)
		return ;
	cell_number = ft_get_index(j, i, k);
	if (!cell_number)
		return ;
	cell = (t_cell *)ptr;
	cell[cell_number].obstacle = FALSE;
	cell[cell_number].surface = FALSE;
}

void	ft_add_cell(void *ptr, int j, int i, int k)
{
	int		cell_number;
	t_cell	*cell;

	cell_number = ft_get_index(j, i, k);
	if (!cell_number)
		return ;
	cell = (t_cell *)ptr;
	cell[cell_number].obstacle = DYNAMIC_OBSTACLE;
}

void	ft_mark_extern_cell(void *ptr, int j, int i, int k)
{
	int		cell_number;
	t_arr	*cells;

	cell_number = ft_get_index(j, i, k);
	if (!cell_number)
		return ;
	cells = (t_arr *)ptr;
	((t_cell *)ft_arr_get(cells, cell_number))->surface
	= ft_is_need_print_cell(cells, j, i, k);
}

void	ft_mark_cell_as_water(void *ptr, int j, int i, int k)
{
	int		cell_number;
	t_cell	*cell;

	cell_number = ft_get_index(j, i, k);
	if (!cell_number)
		return ;
	cell = (t_cell *)ptr;
	if (!cell[cell_number].obstacle)
		cell[cell_number].water = TRUE;
}

int	ft_change_obstacles(t_arr *cells, int cell_numb, int button, t_param *param)
{
	t_point	p;
	t_point	start;
	t_point	end;

	p = ft_get_index_d3(cell_numb);
	ft_fill_point(&start,
		p.y - param->brush,
		p.x - param->brush,
		p.z - param->brush);
	ft_fill_point(&end,
		p.y + param->brush,
		p.x + param->brush,
		p.z + param->brush);
	if (button == RIGHT_BUTTON)
		ft_cycle_cube(cells->elems, ft_del_cell, &start, &end);
	else if (button == LEFT_BUTTON)
		ft_cycle_cube(cells->elems, ft_add_cell, &start, &end);
	else
		return (FALSE);
	ft_cycle_cube(cells, ft_mark_extern_cell, &start, &end);
	return (TRUE);
}
