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

typedef struct		s_cell
{
	int				obstacle;
	int				index[27];
}					t_cell;

__kernel void clear_cell(__global t_cell *cell)
{
	// получаем текущий id.
	int cell_gid;
	int i;

	cell_gid = get_global_id(0);
	if (cell[cell_gid].obstacle)
		return;
	i = 0;
	while (i < 27)
	{
		cell[cell_gid].arr[i] = -1;
		i++;
	}
}
