/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keys.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/27 18:12:08 by ssoraka           #+#    #+#             */
/*   Updated: 2019/10/27 18:12:08 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_mod1.h"


int		ft_get_index(int j, int i, int k)
{
	if (i < I0 || i > IMAX || j < J0 || j > JMAX || k < K0 || k > KMAX)
		return (0);
	return (j * (IMAX + 2) * (KMAX + 2) + i * (KMAX + 2) + k);
}

int		ft_is_cell_obstacle(int **ground, int cell_number)
{
	int j;
	int i;
	int k;

	j = cell_number / ((IMAX + 2) * (KMAX + 2));
	i = cell_number % ((IMAX + 2) * (KMAX + 2)) / (KMAX + 2);
	k = cell_number % (KMAX + 2);
	if (j == J0 || i == IMAX || i == I0 || k == K0 || k == KMAX)
		return (TRUE);
	if (j < J0 || j > JMAX || i > IMAX || i < I0 || k < K0 || k > KMAX)
		return (FALSE);
	//if (i == 15 && k == 15)
	//	return (TRUE);
	//хотел исправить ground[k - 1][i - 1] на ground[k][i],
	//но надо модуль map.c переписать
	#ifdef EMPTY_MAP
	if (j <= (ground[k - 1][i - 1] * JMAX * MAP_KOEF) / MAP_HEIGTH2 + 1)
		return (TRUE);
	#endif
	return (FALSE);
}



void	ft_fill_cells_from_ground(t_cell *cells, int **ground)
{
	int n;

	n = 0;
	while (n <= CELL_COUNT)
	{
		cells[n].obstacle = ft_is_cell_obstacle(ground, n);
		n++;
	}
}
