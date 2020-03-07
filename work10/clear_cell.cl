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

__kernel void clear_cell(__global t_cell *cell)
{
	// получаем текущий id.
	int c_gid;
	int i;

	c_gid = get_global_id(0);
	if (cell[c_gid].obstacle != 0)
		return;
	i = 0;
	cell[c_gid].full = 0;
	while (i < 27)
	{
		cell[c_gid].index[i] = -1;
		i++;
	}
}
