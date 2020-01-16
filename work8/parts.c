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

# define MAX_NEIGHBORS 32
# define DELTA_H 0.800

int ppp = 0;

void ft_del_parts(void *ptr)
{
	t_part *part;

	part = (t_part *)(*((void **)ptr));
	if (part)
	{
		ft_cut_part(part);
		ft_del_arr(&(part->neigh));
		free(part);
	}
}


t_arr	*ft_init_all_clear_parts(void)
{
	t_arr *parts;
	int parts_count;

	parts_count = (int)(10);
	parts = ft_create_arr(sizeof(t_part *), parts_count, &ft_del_parts);
	if (!parts)
		ft_del_all_print_error("need more memory");
	return (parts);
}

void	ft_fill_part(t_part *part, t_dpoint *p, int type)
{
	if (!part || !p)
		return ;
	part->type = type;
	ft_fill_point(&(part->jik), (int)p->y + 0, (int)p->x + 0, (int)p->z + 0);
	ft_fill_dpoint(&(part->pos.abs), p->y, p->x, p->z);
	ft_fill_param_of_part(part, NULL);
	if (type == WATER)
		part->pos.color = WATER_COLOR;
	else
		part->pos.color = OBSTACLES_TOP_COLOR;
}

void	ft_insert_part(t_part *part)
{
	t_part *tmp;

	if (!part)
		return ;
	tmp = parts[part->jik.y][part->jik.x][part->jik.z];
	part->next = tmp;
	if (tmp)
		tmp->prev = part;
	parts[part->jik.y][part->jik.x][part->jik.z] = part;
	part->prev = NULL;
}

void	ft_cut_part(t_part *part)
{
	t_part **tmp;

	if (!part)
		return ;
	tmp = &(parts[part->jik.y][part->jik.x][part->jik.z]);
	if (!part->prev && !part->next && part != *tmp)
		return ;
	if (!part->prev)
		*tmp = part->next;
	else
		part->prev->next = part->next;

	if (part->next)
		part->next->prev = part->prev;

	part->prev = NULL;
	part->next = NULL;
}

void	ft_create_new_water_in_area(t_arr **p_arr, t_dpoint *start)
{
	REAL i;
	REAL k;
	int cell_type;
	t_part *part;
	t_dpoint pos;

	//ft_bzero((void *)&part, sizeof(t_part));
	i = start->x;
	while (i <= IMAX)
	{
		k = start->z;
		while (k <= KMAX)
		{
			cell_type = map[(int)(start->y)][(int)i][(int)k];
			if (cell_type != EMPTY)
			{
				ft_fill_dpoint(&pos, start->y, i, k);
				//ft_fill_part(&part, &pos, cell_type);
				part = ft_new_part(&pos, cell_type);
				ft_insert_part(part);
				ft_arr_add(p_arr, (void *)(&part));

				//parts[(int)(start->y)][(int)i][(int)k] = ft_arr_add(p_arr, (void *)&part);
			}
			k++;
		}
		i++;
	}
}

void	ft_create_new_area_of_water(t_arr **parts)
{
	t_dpoint start;
	int n;

	n = 0;
	start.y = J0 + 0.5;
	while (start.y <= JMAX)
	{
		if (n % 2)
			ft_fill_dpoint(&start, start.y, I0 + 0.2, K0 + 0.2);
		else
			ft_fill_dpoint(&start, start.y, I0 + 0.7, K0 + 0.7);
		ft_create_new_water_in_area(parts, &start);
		start.y += DELTA_H;
		n++;
	}
}

t_part	*ft_new_part(t_dpoint *p, int type)
{
	t_part *tmp;

	if ((tmp = (t_part *)ft_memalloc(sizeof(t_part))))
	{
		ft_fill_part(tmp, p, type);
		tmp->neigh = ft_create_arr(sizeof(t_neigh), MAX_NEIGHBORS, NULL);
		if (!tmp->neigh)
			ft_memdel((void **)(&tmp));
	}
	if (!tmp)
		ft_del_all_print_error("need more memory2");
	return (tmp);
}

t_part	*ft_add_part(void *ptr, t_dpoint *p, int type)
{
	t_part *tmp;
	t_part **begin;

	begin = (t_part **)ptr;
	tmp = ft_new_part(p, type);
	if (!tmp)
		return (NULL);
	tmp->next = *begin;
	*begin = tmp;
	return (tmp);
}


void	ft_del_part(t_part **begin)
{
	t_part *tmp;

	tmp = *begin;
	while (tmp)
	{
		*begin = tmp->next;
		free(tmp);
		tmp = *begin;
	}
	*begin = NULL;
}

void	ft_replace_part(t_part *prev, t_part **part, t_part ****parts)
{
	int i;
	int j;
	int k;

	prev->next = (*part)->next;
	i = (*part)->jik.x;
	j = (*part)->jik.y;
	k = (*part)->jik.z;
	(*part)->next = parts[j][i][k];
	parts[j][i][k] = (*part);
	*part = prev;
}

void	ft_replace_part2(t_part *part)
{
	int i;
	int j;
	int k;

	i = part->jik.x;
	j = part->jik.y;
	k = part->jik.z;
	if (i < I0 || j < J0 || k < K0 || i > IMAX || j > JMAX || k > KMAX)
	{
		part->type = EMPTY;
		part->pos.color = 0xFFFFFF;
		ft_fill_dpoint(&(part->pos.abs), JMAX + 1, IMAX + 1, KMAX + 1);
		ft_fill_point(&(part->jik), JMAX + 1, IMAX + 1, KMAX + 1);
	}
	ft_insert_part(part);
}

int		ft_part_change_cell(t_part *part)
{
	int i;
	int j;
	int k;

	i = (int)(part->pos.abs.x / delta.x) + 0;
	j = (int)(part->pos.abs.y / delta.y) + 0;
	k = (int)(part->pos.abs.z / delta.z) + 0;
	if (i != part->jik.x || j != part->jik.y || k != part->jik.z)
	{
		ft_cut_part(part);
		ft_fill_point(&(part->jik), j, i, k);
		return (TRUE);
	}
	return (FALSE);
}
