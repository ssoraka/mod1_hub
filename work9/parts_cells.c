/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parts.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/08 14:28:01 by ssoraka           #+#    #+#             */
/*   Updated: 2019/10/08 14:28:02 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_mod1.h"

//ft_for_each_ptr2(g_parts, &ft_add_part_in_cell,(void *)g_cells);


t_cell	*ft_get_cell_by_part(t_part *part, t_cell *cell)
{
	int j;
	int i;
	int k;

	if (part->type == EMPTY)
		return (NULL);
	j = (int)(part->pos.abs.y / delta.y) + 0;
	i = (int)(part->pos.abs.x / delta.x) + 0;
	k = (int)(part->pos.abs.z / delta.z) + 0;
	return (&cell[ft_get_index(j, i, k)]);
}

void	ft_set_part_to_cell(t_part *part, t_cell *cell)
{
	int j;
	int i;
	int k;

	j = (int)(part->pos.abs.y / delta.y) + 0;
	i = (int)(part->pos.abs.x / delta.x) + 0;
	k = (int)(part->pos.abs.z / delta.z) + 0;
	ft_fill_point(&(part->jik), j, i, k);
}

#define COEFF_SPEED2 0.5

void	ft_come_back_part3(t_part *part, t_cell *cell)
{
	int j;
	int i;
	int k;

	j = (int)(part->pos.abs.y / delta.y) + 0;
	i = (int)(part->pos.abs.x / delta.x) + 0;
	k = (int)(part->pos.abs.z / delta.z) + 0;
	if (cell[ft_get_index(part->jik.y, i, part->jik.z)].obstacle)
	{
		if (part->jik.x < i)
			part->pos.abs.x = i - 0.001;
		else
			part->pos.abs.x = part->jik.x;
		part->speed.x = -part->speed.x * COEFF_SPEED2;
		i = part->jik.x;
	}
	if (cell[ft_get_index(j, part->jik.x, part->jik.z)].obstacle)
	{
		if (part->jik.y < j)
			part->pos.abs.y = j - 0.001;
		else
			part->pos.abs.y = part->jik.y;
		part->speed.y = -part->speed.y * COEFF_SPEED2;
		j = part->jik.y;
	}
	if (cell[ft_get_index(part->jik.y, part->jik.x, k)].obstacle)
	{
		if (part->jik.z < k)
			part->pos.abs.z = k - 0.001;
		else
			part->pos.abs.z = part->jik.z;
		part->speed.z = -part->speed.z * COEFF_SPEED2;
		k = part->jik.z;
	}

}


int		ft_cant_add_part_in_cell(void *p_i, void *g_cell)
{
	t_cell *cell;
	t_part *part;

	part = *((t_part **)p_i);
	cell = ft_get_cell_by_part(part, (t_cell *)g_cell);

	if (part->type != OBSTCL)
	{
		if (cell && cell->obstacle)
		{
			ft_come_back_part3(part, (t_cell *)g_cell);
			cell = ft_get_cell_by_part(part, (t_cell *)g_cell);
		}
		if (!cell)
			part->type = EMPTY;
		//в таблице еще этого нет
		//if (cell->obstacle)
		//	part->type = OBSTCL;
	}
	if (part->type == EMPTY)
		return (1);
	ft_set_part_to_cell(part, cell);
	if (!ft_arr_add(&(cell->parts), p_i))
		ft_del_all_print_error("need more memory");
	return (0);
}








void	ft_find_neighbors(void *p_i, int j, int i, int k)
{
	t_cell *cell;

	cell = &g_cell[ft_get_index(j, i, k)];
	if (!cell->parts->elems_used)
		return;
	ft_for_each_ptr(cell->parts, &ft_try_add_part_as_neighbor,(void *)p_i);
}


void	ft_new_neighbors3(void *p_i, void *param)
{
	t_part *part;
	t_point end;
	t_point start;

	part = (t_part *)p_i;
	if (part->type == EMPTY)
		return ;
	part->neigh->elems_used = 0;
	ft_fill_point(&start, part->jik.y - 1, part->jik.x - 1, part->jik.z - 1);
	ft_fill_point(&end, part->jik.y + 1, part->jik.x + 1, part->jik.z + 1);
	ft_cycle_cube((void *)p_i, &ft_find_neighbors, &start, &end);
}
