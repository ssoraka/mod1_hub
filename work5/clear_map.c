/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clear_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/06 14:16:16 by ssoraka           #+#    #+#             */
/*   Updated: 2019/10/06 14:16:16 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_mod1.h"

void	ft_clear_cell_from_water(void *param, int j, int i, int k)
{
	if (map[j][i][k] == WATER)
		map[j][i][k] = EMPTY;
}

void	ft_clear_map_from_water(void)
{
	t_point start;
	t_point end;

	ft_fill_point(&start, 1, 1, 1);
	ft_fill_point(&end, jmax, imax, kmax);
	//удаляем маркировку водных клеток
	ft_cycle_cube(NULL, &ft_clear_cell_from_water, &start, &end);
}
