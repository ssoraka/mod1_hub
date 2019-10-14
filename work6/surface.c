/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   surface.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/07 18:36:20 by ssoraka           #+#    #+#             */
/*   Updated: 2019/10/07 18:36:20 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_mod1.h"





void	ft_surface_speed_v(int j, int i, int k)
{
	if (flags_surface[j][i][k] & C_S)
		speed_v[j - 1][i][k] = speed_v[j][i][k];
	if (flags_surface[j][i][k] & C_N)
		speed_v[j][i][k] = speed_v[j - 1][i][k];
	if (flags_surface[j][i][k] & C_E)
		speed_v[j][i + 1][k] = speed_v[j][i][k];
	if (flags_surface[j][i][k] & C_W)
		speed_v[j][i - 1][k] = speed_v[j][i][k];
	if (flags_surface[j][i][k] & C_O)
		speed_v[j][i][k + 1] = speed_v[j][i][k];
	if (flags_surface[j][i][k] & C_I)
		speed_v[j][i][k - 1] = speed_v[j][i][k];
}


void	ft_surface_speed_u(int j, int i, int k)
{
	if (flags_surface[j][i][k] & C_N)
		speed_u[j + 1][i][k] = speed_u[j][i][k];
	if (flags_surface[j][i][k] & C_S)
		speed_u[j - 1][i][k] = speed_u[j][i][k];
	if (flags_surface[j][i][k] & C_E)
		speed_u[j][i][k] = speed_u[j][i - 1][k];
	if (flags_surface[j][i][k] & C_W)
		speed_u[j][i - 1][k] = speed_u[j][i][k];
	if (flags_surface[j][i][k] & C_O)
		speed_u[j][i][k + 1] = speed_u[j][i][k];
	if (flags_surface[j][i][k] & C_I)
		speed_u[j][i][k - 1] = speed_u[j][i][k];
}


void	ft_surface_speed_w(int j, int i, int k)
{
	if (flags_surface[j][i][k] & C_N)
		speed_w[j + 1][i][k] = speed_w[j][i][k];
	if (flags_surface[j][i][k] & C_S)
		speed_w[j - 1][i][k] = speed_w[j][i][k];
	if (flags_surface[j][i][k] & C_E)
		speed_w[j][i + 1][k] = speed_w[j][i][k];
	if (flags_surface[j][i][k] & C_W)
		speed_w[j][i - 1][k] = speed_w[j][i][k];
	if (flags_surface[j][i][k] & C_O)
		speed_w[j][i][k] = speed_w[j][i][k - 1];
	if (flags_surface[j][i][k] & C_I)
		speed_w[j][i][k - 1] = speed_w[j][i][k];
}


void	ft_surface_speed_and_pressure(void *param, int j, int i, int k)
{
	if (!flags_surface[j][i][k])
		return ;
	press_p[j][i][k] = 0.0;
	if (flags_surface[j][i][k] == SURF_MASK)
		return ;
	/*{
		speed_v[j][i][k] += deltat * gy;
		speed_u[j][i][k] += deltat * gx;
		speed_w[j][i][k] += deltat * gz;
		speed_v[j - 1][i][k] = speed_v[j][i][k];
		speed_u[j][i - 1][k] = speed_u[j][i][k];
		speed_w[j][i][k - 1] = speed_w[j][i][k];
		return ;
	}*/
	//надо бы это все допилить...
	ft_surface_speed_u(j, i, k);
	ft_surface_speed_v(j, i, k);
	ft_surface_speed_w(j, i, k);

}



void	ft_set_uvp_surface(void)
{
	t_point start;
	t_point end;

	ft_fill_point(&start, 1, 1, 1);
	ft_fill_point(&end, jmax, imax, kmax);
	//проставляем все значения для границы воды
	ft_cycle_cube(NULL, &ft_surface_speed_and_pressure, &start, &end);

}
