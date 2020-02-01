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
# define DELTA_H 0.78


void ft_del_parts(void *ptr)
{
	t_part *part;

	part = (t_part *)(*((void **)ptr));
	if (part)
	{
		ft_del_arr(&(part->neigh));
		free(part);
	}
}


t_arr	*ft_init_all_clear_parts(void)
{
	t_arr *parts;

	parts = ft_create_arr(sizeof(t_part *), 16, &ft_del_parts);
	if (!parts)
		ft_del_all_print_error("need more memory");
	return (parts);
}

void	ft_fill_part(t_part *part, t_dpoint *p, int type)
{
	if (!part || !p)
		return ;
	ft_fill_point(&(part->jik), (int)p->y + 0, (int)p->x + 0, (int)p->z + 0);
	ft_fill_dpoint(&(part->pos.abs), p->y, p->x, p->z);
	part->type = type;
	ft_fill_param_of_part(part, NULL);
	if (type == WATER)
		part->pos.color = WATER_COLOR;
	else if (type == BLOB)
		part->pos.color = 0xFFFFFF;
	else if (type == MAGMA)
		part->pos.color = 0xFF0000;
	else
		part->pos.color = OBSTACLES_TOP_COLOR;
}


void	ft_create_new_water_in_area(t_arr **p_arr, t_dpoint *start, t_point *end, int type)
{
	REAL i;
	REAL k;
	t_part *part;
	t_dpoint pos;

	i = start->x;
	while (i < end->x + 1 && i < IMAX + 1)
	{
		k = start->z;
		while (k < end->z + 1 && k < KMAX + 1)
		{
			if (!g_cell[ft_get_index((int)(start->y), (int)i, (int)k)].obstacle)
			//if (cell_type == WATER || cell_type == BLOB)
			{
				ft_fill_dpoint(&pos, start->y, i, k);
				part = ft_new_part(&pos, type);
				ft_arr_add(p_arr, (void *)(&part));
			}
			k++;
		}
		i++;
	}
}


void	ft_create_new_area_of_water(t_arr **parts, t_point *start, t_point *end, int type)
{
	t_dpoint tmp;
	int n;

	n = 0;
	tmp.y = start->y + 0.5;
	while (tmp.y < end->y + 1 && tmp.y < JMAX + 1)
	{
		if (n % 2)
			ft_fill_dpoint(&tmp, tmp.y, start->x + 0.2, start->z + 0.2);
		else
			ft_fill_dpoint(&tmp, tmp.y, start->x + 0.7, start->z + 0.7);
		ft_create_new_water_in_area(parts, &tmp, end, type);
		tmp.y += DELTA_H;
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
