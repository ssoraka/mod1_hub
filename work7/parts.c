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
	t_point p;
	int xyz[3];
	int ijk[3];
	int parts_count;
	int n;

	parts_count = *((int *)param);
	n = parts_count * parts_count * parts_count;
	while (n > 0)
	{
		n--;
		ijk[0] = n / (parts_count * parts_count);
		ijk[2] = n % parts_count;
		ijk[1] = (n % (parts_count * parts_count)) / parts_count;

		xyz[0] = delta.x / parts_count * ijk[0] + delta.x / parts_count / 2 + delta.x * (i - 1);
		xyz[1] = delta.y / parts_count * ijk[1] + delta.y / parts_count / 2 + delta.y * (j - 1);
		xyz[2] = delta.z / parts_count * ijk[2] + delta.z / parts_count / 2 + delta.z * (k - 1);
		ft_fill_point(&p, xyz[1], xyz[0], xyz[2]);
		if (!ft_add_vektor2((void *)&(parts[j][i][k]), &p, WATER_COLOR))
			ft_del_all_print_error(MSG_ERROR1);
		//printf("%d_%d_%d_%d\n", n, ijk[1], ijk[0], ijk[2]);
	}
}

void	ft_replace_part(t_part **prev, t_part *part, t_part ****parts)
{
	int i;
	int j;
	int k;

	if (*prev == part)
	{
		*prev = part->next;
	}
	else
	{
		(*prev)->next = part->next;
	}
	i = part->jik.x;
	j = part->jik.y;
	k = part->jik.z;
	part->next = parts[j][i][k];
	parts[j][i][k] = part;
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
