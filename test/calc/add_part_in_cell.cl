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



int		ft_get_cell_index(t_dpoint pos)
{
	int j;
	int i;
	int k;

	j = (int)pos.y;
	i = (int)pos.x;
	k = (int)pos.z;
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

__kernel void add_part_in_cell(__global t_part* part, __global t_cell* cell)
{
	// получаем текущий id.
	int gid;
	int cell_id;

	gid = get_global_id(0);

	cell_id = ft_get_cell_index(part[gid].pos);
	cell[cell_id].arr[ft_get_arr_index(part[gid].pos)] = gid;

}
