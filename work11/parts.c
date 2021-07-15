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



t_arr	*ft_init_all_clear_parts(void)
{
	t_arr *parts;

	parts = ft_create_arr(sizeof(t_part), 16, NULL);
	if (!parts)
		ft_del_all("need more memory");
	return (parts);
}

void	ft_fill_part(t_part *part, t_dpoint *p, int type)
{
	if (!part || !p)
		return ;
	ft_fill_point(&(part->jik), (int)p->y + 0, (int)p->x + 0, (int)p->z + 0);
	ft_fill_dpoint(&(part->pos), p->y, p->x, p->z);
	part->type = type;
	part->speed.y = g_param[part->type][F_Y_SPEED];
}


void	ft_create_new_water_in_area(t_arr *p_arr, t_dpoint *start, t_point *end, int type)
{
	REAL i;
	REAL k;
	t_part part;
	t_dpoint pos;
	t_cell *cell;

	i = start->x;
	while (i < end->x + 1 && i < IMAX + 1)
	{
		k = start->z;
		while (k < end->z + 1 && k < KMAX + 1)
		{
			cell = ft_arr_get(g_cell, ft_get_index((int)(start->y), (int)i, (int)k));
			if (ft_get_index((int)(start->y), (int)i, (int)k) && !cell->obstacle)
			{
				ft_fill_dpoint(&pos, start->y, i, k);
				ft_fill_part(&part, &pos, type);
				ft_arr_add(p_arr, (void *)&part);
			}
			k++;
		}
		i++;
	}
}


void	ft_create_new_area_of_water(t_arr *parts, t_point *start, t_point *end, int type)
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

void	ft_fill_interface(t_arr *parts, t_arr *iparts)
{
	t_part *p;
	t_ipart ip;
	t_iter iter;

	iter = get_arr_iter(parts);
	while ((p = (t_part *)iter.get_next_elem(&iter)))
	{
		ip.pos = p->pos;
		ip.type = p->type;
		ft_arr_add(iparts, (void *)&ip);
	}
}

int		ft_del_unused_part(void *elem, void *param)
{
	if (((t_part *)elem)->type == 0)
		return (TRUE);
	return (FALSE);
}
