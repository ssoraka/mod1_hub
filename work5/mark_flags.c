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

int		ft_set_flag_for(char map)
{
	int flag;

	flag = C_B;
	if (map == WATER)
		flag = C_F;
	else if (map == EMPTY)
		flag = C_A;
	return(flag);
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
	flags[j][i][k] = ft_set_flag_for(map[j][i][k]);
	ft_is_water_surround(j, i, k);
	ft_is_empty_surround(j, i, k);
}


void	ft_mark_top_down(void *param, int j, int i, int k)
{
	j = 0;
	flags[j][i][k] = ft_set_flag_for(OBSTACLES);
	if (map[j + 1][i][k] == WATER)
		flags[j][i][k] |= B_N;
	j = jmax + 1;
	flags[j][i][k] = ft_set_flag_for(OBSTACLES);
	if (map[j - 1][i][k] == WATER)
		flags[j][i][k] |= B_S;
}

void	ft_mark_left_right(void *param, int j, int i, int k)
{
	i = 0;
	flags[j][i][k] = ft_set_flag_for(OBSTACLES);
	if (map[j][i + 1][k] == WATER)
		flags[j][i][k] |= B_E;
	i = imax + 1;
	flags[j][i][k] = ft_set_flag_for(OBSTACLES);
	if (map[j][i - 1][k] == WATER)
		flags[j][i][k] |= B_W;
}

void	ft_mark_in_out(void *param, int j, int i, int k)
{
	k = 0;
	flags[j][i][k] = ft_set_flag_for(OBSTACLES);
	if (map[j][i][k + 1] == WATER)
		flags[j][i][k] |= B_O;
	k = jmax + 1;
	flags[j][i][k] = ft_set_flag_for(OBSTACLES);
	if (map[j][i][k - 1] == WATER)
		flags[j][i][k] |= B_I;
}

void	ft_fill_boundary_flags(void)
{
	t_point start;
	t_point end;

	ft_fill_point(&start, 1, 1, 1);
	ft_fill_point(&end, 1, imax, kmax);
	ft_cycle_cube(NULL, &ft_mark_top_down, &start, &end);
	ft_fill_point(&end, jmax, 1, kmax);
	ft_cycle_cube(NULL, &ft_mark_left_right, &start, &end);
	ft_fill_point(&end, jmax, imax, 1);
	ft_cycle_cube(NULL, &ft_mark_in_out, &start, &end);
}


void	ft_remark_flags()
{
	t_point start;
	t_point end;

	ft_fill_point(&start, 1, 1, 1);
	ft_fill_point(&end, jmax, imax, kmax);
	ft_cycle_cube(NULL, &ft_mark_cell, &start, &end);
	//надо проставить на 6-и гранях символ препятствия
	ft_fill_boundary_flags();
}
