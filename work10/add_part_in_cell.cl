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

int		ft_get_cell_index(int j, int i, int k);
int		ft_get_arr_index(t_dpoint pos);


int		ft_get_cell_index(int j, int i, int k)
{
	if (i < I0 || i > IMAX || j < J0 || j > JMAX || k < K0 || k > KMAX)
		return (0);
	return (j * (IMAX + 2) * (KMAX + 2) + i * (KMAX + 2) + k);
}

int		ft_get_arr_index(t_dpoint pos)
{
	int j;
	int i;
	int k;

	j = (int)((pos.y - (int)pos.y) / 0.35);
	i = (int)((pos.x - (int)pos.x) / 0.35);
	k = (int)((pos.z - (int)pos.z) / 0.35);

	return (j * (3 * 3) + i * (3) + k);
}

__kernel void add_part_in_cell(__global t_part *part, __global t_cell_map *cell_map)
{
	int gid;
	int c_gid;
	t_dpoint pos;
	t_point jik;

	gid = get_global_id(0);
	pos = part[gid].pos;
	jik = part[gid].jik;

	c_gid = ft_get_cell_index(jik.y, jik.x, jik.z);
	cell_map[c_gid].index[ft_get_arr_index(pos)] = gid;
	cell_map[c_gid].full = 1;
}
