/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mark_flags.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/05 18:41:00 by ssoraka           #+#    #+#             */
/*   Updated: 2019/10/05 18:41:01 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_mod1.h"

void	ft_set_flag_for(int j, int i, int k)
{
	if (map[j][i][k] == WATER)
		flags[j][i][k] = C_F;
	else if (map[j][i][k] == OBSTACLES)
		flags[j][i][k] = C_B;
	else
		flags[j][i][k] = C_A;
}

void	ft_is_water_surround(int j, int i, int k)
{
	//if (ft_is_water(flags[j][i][k]))
	//{
	if (map[j + 1][i][k] == WATER)
		flags[j][i][k] |= B_N;
	if (map[j - 1][i][k] == WATER)
		flags[j][i][k] |= B_S;
	if (map[j][i + 1][k] == WATER)
		flags[j][i][k] |= B_E;
	if (map[j][i - 1][k] == WATER)
		flags[j][i][k] |= B_W;
	if (map[j][i][k + 1] == WATER)
		flags[j][i][k] |= B_O;
	if (map[j][i][k - 1] == WATER)
		flags[j][i][k] |= B_I;
	//}
}

void	ft_is_empty_surround(int j, int i, int k)
{
	flags_surface[j][i][k] = 0;
	if (ft_is_water(flags[j][i][k]))
	{
		if (map[j + 1][i][k] == EMPTY)
			flags_surface[j][i][k] |= C_N;
		if (map[j - 1][i][k] == EMPTY)
			flags_surface[j][i][k] |= C_S;
		if (map[j][i + 1][k] == EMPTY)
			flags_surface[j][i][k] |= C_E;
		if (map[j][i - 1][k] == EMPTY)
			flags_surface[j][i][k] |= C_W;
		if (map[j][i][k + 1] == EMPTY)
			flags_surface[j][i][k] |= C_O;
		if (map[j][i][k - 1] == EMPTY)
			flags_surface[j][i][k] |= C_I;
		if (flags[j][i][k] & SURF_MASK)
			flags[j][i][k] |= C_R;
	}
	else
		flags_surface[j][i][k] = 0;
}

void	ft_mark_cell(void *param, int j, int i, int k)
{
	ft_set_flag_for(j, i, k);
	ft_is_water_surround(j, i, k);
	ft_is_empty_surround(j, i, k);
}


void	ft_remark_flags()
{
	t_point start;
	t_point end;

	ft_fill_point(&start, 1, 1, 1);
	ft_fill_point(&end, jmax, imax, kmax);
	ft_cycle_cube(NULL, &ft_mark_cell, &start, &end);
	//надо проставить на 6-и гранях символ препятствия
}
