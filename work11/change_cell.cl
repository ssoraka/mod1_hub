/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kernel.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/25 17:13:40 by ssoraka           #+#    #+#             */
/*   Updated: 2019/08/25 17:13:40 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_cl.h"

#define COEFF_SPEED2 0.5

int		ft_get_cell_index(int j, int i, int k);

int		ft_get_cell_index(int j, int i, int k)
{
	if (i < I0 || i > IMAX || j < J0 || j > JMAX || k < K0 || k > KMAX)
		return (0);
	return (j * (IMAX + 2) * (KMAX + 2) + i * (KMAX + 2) + k);
}


__kernel void change_cell(__global t_part *part, __global t_cell *cell, __global t_ipart *ipart)
{
	int gid;
	t_dpoint pos;
	t_dpoint speed;
	t_point old_c;
	int j;
	int i;
	int k;

	gid = get_global_id(0);
	pos = part[gid].pos;
	speed = part[gid].speed;

	j = (int)pos.y;
	i = (int)pos.x;
	k = (int)pos.z;

	if (cell[ft_get_cell_index(j, i, k)].obstacle != 0)
	{
		old_c = part[gid].jik;

		if (cell[ft_get_cell_index(j, i, k)].obstacle != 0
		&& cell[ft_get_cell_index(old_c.y, i, old_c.z)].obstacle != 0)
		{
			if (old_c.x < i)
				pos.x = i - 0.001;
			else
				pos.x = old_c.x;
			speed.x = -speed.x * COEFF_SPEED2;
			i = old_c.x;
		}
		if (cell[ft_get_cell_index(j, i, k)].obstacle != 0
		&& cell[ft_get_cell_index(j, old_c.x, old_c.z)].obstacle != 0)
		{
			if (old_c.y < j)
				pos.y = j - 0.001;
			else
				pos.y = old_c.y;
			speed.y = -speed.y * COEFF_SPEED2;
			j = old_c.y;
		}
		if (cell[ft_get_cell_index(j, i, k)].obstacle != 0
		&& cell[ft_get_cell_index(old_c.y, old_c.x, k)].obstacle != 0)
		{
			if (old_c.z < k)
				pos.z = k - 0.001;
			else
				pos.z = old_c.z;
			speed.z = -speed.z * COEFF_SPEED2;
			k = old_c.z;
		}
		part[gid].speed = speed;
		part[gid].pos = pos;
	}
	part[gid].jik = (t_point){j, i, k};

	ipart[gid].pos = part[gid].pos;
	ipart[gid].type = part[gid].type;
}
