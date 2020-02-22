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



__kernel void up_part_in_cell(__global t_cell* cell)
{
	int cell_gid;
	int i;
	int last;

	cell_gid = get_global_id(0);
	if (cell[cell_id].obstacle)
		return;
	last = 0;
	i = 0;
	while (i < 27)
	{
		if (cell[cell_gid].arr[i] != -1)
		{
			cell[cell_gid].arr[last] = cell[cell_gid].arr[i];
			last++;
			cell[cell_gid].arr[i] = -1;
		}
		i++;
	}
}
