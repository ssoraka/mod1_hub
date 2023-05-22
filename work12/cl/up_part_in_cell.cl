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

#include "./includes/ft_cl.h"

__kernel void up_part_in_cell(__global t_cell_map *cell)
{
	int c_gid;
	int i;
	int last;

	c_gid = get_global_id(0);
	if (cell[c_gid].obstacle != 0 || cell[c_gid].full == 0)
	{
		cell[c_gid].full = 0;
		return;
	}

	last = 0;
	i = 0;
	while (i < CELL_MAP_SIZE)
	{
		if (cell[c_gid].index[i] != -1)
		{
			if (i != last)
			{
				cell[c_gid].index[last] = cell[c_gid].index[i];
				cell[c_gid].index[i] = -1;
			}
			last++;
		}
		i++;
	}
}
