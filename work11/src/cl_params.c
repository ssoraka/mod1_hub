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

#include "../includes/ft_mod1.h"

void	init_cl_properties()
{
//	{WATER, PART_H, 120.0, PART_MASS_0, 220000000.0, 1000.0, 0000.0, 00.0},
//	{WATER2, PART_H, 120.0, PART_MASS_0 * 0.50, 22000.0, 500.0, 0000.0, 00.0},
//	{MAGMA, PART_H, 120.0, PART_MASS_0 * 1.0, 220000000.0, 1000.0, 00.0, -10.0},
//	{MAGMA2, PART_H, 120.0, PART_MASS_0 * 10.0, 2200000000.0, 10000.0, 2000.0, 0.0},
//	{BLOB, PART_H, 120.0, PART_MASS_0, 220000000.0, 1000.0, 0.0, -20.0},
//	{OBSTCL, PART_H, 120.0, PART_MASS_0, 220000000.0, 1000.0, 0000.0, 00.0}

//	{FLUID, COLOR, RADIUS2},
//	{WATER, WATER_COLOR, 3},
//	{WATER2, 0xFFFFFF, 3},
//	{MAGMA, 0xFF0000, 5},
//	{MAGMA2, OBSTACLES_TOP_COLOR, 5},
//	{BLOB, 0xFFFFFF, 3},
//	{OBSTCL, OBSTACLES_TOP_COLOR, 5},
	g_prop.g = (t_dpoint){CONST_GY, CONST_GX, CONST_GZ};
	g_prop.f[WATER] = (t_fluid){WATER_COLOR, 3, PART_H, 120.0, PART_MASS_0, 220000000.0, 1000.0, 0000.0};
	g_prop.f[MAGMA] = (t_fluid){WATER_COLOR, 3, PART_H, 120.0, PART_MASS_0 * 10.0, 2200000000.0, 10000.0, 2000.0};
}