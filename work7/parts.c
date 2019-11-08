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

void	ft_create_new_water_in_cell(void *param, int j, int i, int k)
{
	t_dpoint p;
	int ijk[3];
	int parts_count;
	int n;

	parts_count = PARTS_COUNT;//*((int *)param);
	n = parts_count * parts_count * parts_count;
	while (n > 0)
	{
		n--;
		ijk[0] = n / (parts_count * parts_count);
		ijk[2] = n % parts_count;
		ijk[1] = (n % (parts_count * parts_count)) / parts_count;

		p.x = delta.x / parts_count * ijk[0] + delta.x / parts_count / 2 + delta.x * (i - 1);
		p.y = delta.y / parts_count * ijk[1] + delta.y / parts_count / 2 + delta.y * (j - 1);
		p.z = delta.z / parts_count * ijk[2] + delta.z / parts_count / 2 + delta.z * (k - 1);
		if (!ft_add_part((void *)&(parts[j][i][k]), &p, *((int *)param)))
			ft_del_all_print_error(MSG_ERROR1);
		ft_fill_point(&((parts[j][i][k])->jik), j, i, k);
		//printf("%d_%d_%d_%d\n", n, ijk[1], ijk[0], ijk[2]);
	}
}

t_part	*ft_new_part(t_dpoint *p, int type)
{
	t_part *tmp;

	tmp = (t_part *)ft_memalloc(sizeof(t_part));
	if (!tmp)
		return (NULL);
	tmp->type = type;
	ft_fill_point(&(tmp->pos.otn), (int)p->y, (int)p->x, (int)p->z);
	ft_fill_dpoint(&(tmp->pos.abs), p->y, p->x, p->z);
	ft_fill_param_of_part(tmp, NULL);
	if (type == WATER)
		tmp->pos.color = WATER_COLOR;
	else
		tmp->pos.color = OBSTACLES_TOP_COLOR;
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

int		ft_part_change_cell(t_part *part)
{
	int i;
	int j;
	int k;

	i = (int)(part->pos.abs.x / delta.x) + 1;
	j = (int)(part->pos.abs.y / delta.y) + 1;
	k = (int)(part->pos.abs.z / delta.z) + 1;
	if (i != part->jik.x || j != part->jik.y || k != part->jik.z)
	{
		ft_fill_point(&(part->jik), j, i, k);
		return (TRUE);
	}
	return (FALSE);
}
