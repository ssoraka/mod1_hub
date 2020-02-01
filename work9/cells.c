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
/*
t_point	ft_get_indexs(int n)
{
	t_point jik;

	ft_fill_point(&jik, 0, 0, 0);
	if (n < 0 || n > CELL_COUNT)
		return (jik);
	jik.y = n / ((IMAX + 1) * (KMAX + 1));
	jik.x = n / (KMAX + 1);
	jik.z = n % (KMAX + 1);
	return (jik);
}*/

/*
t_cell	*ft_get_elem_cube_arr(int j, int i, int k)
{
	if (i > IMAX + 1 || j > JMAX + 1 || k > KMAX + 1)
		return (NULL);
	return (g_cell + (j * (IMAX + 2) * (KMAX + 2) + i * (KMAX + 2) + k) * sizeof(g_cell));
}*/



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
	if (j <= (ground[k - 1][i - 1] * JMAX * MAP_KOEF) / MAP_HEIGTH2 + 1)
		return (TRUE);
	return (FALSE);
}



t_cell	*ft_create_cells_from_ground(int **ground)
{
	t_cell *cells;
	int n;

	cells = ft_memalloc(sizeof(t_cell) * (CELL_COUNT + 1));
	if (!cells)
		return (NULL);
	n = 0;
	while (n <= CELL_COUNT)
	{
		cells[n].parts = ft_create_arr(sizeof(t_part *), 32, NULL);
		if (!cells[n].parts)
			ft_del_all_print_error("need more memory");
		cells[n].obstacle = ft_is_cell_obstacle(ground, n);
		n++;
	}
	return (cells);
}

void	ft_del_cell(t_cell **cells)
{
	int n;

	if (!cells || !(*cells))
		return ;
	n = 0;
	while (n < CELL_COUNT + 1)
	{
		ft_del_arr(&((*cells)[n].parts));
		n++;
	}
	ft_memdel((void **)cells);
}


void	ft_clear_cells(t_cell *cells)
{
	int n;

	if (!cells)
		return ;
	n = 0;
	while (n <= CELL_COUNT)
	{
		cells[n].parts->elems_used = 0;
		n++;
	}
}
