/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   solver_3d.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/05 16:29:13 by ssoraka           #+#    #+#             */
/*   Updated: 2019/10/05 16:29:13 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_mod1.h"


REAL	ft_time_control()
{
	REAL deltat;
	REAL tmp;

	deltat = T_INFINITY;
	if (max_u && dx / max_u < deltat)
		deltat = dx / max_u;
	if (max_v && dy / max_v < deltat)
		deltat = dy / max_v;
	if (max_w && dz / max_w < deltat)
		deltat = dz / max_w;
	tmp = renolds / (1.0 / dx2 + 1.0 / dy2 + 1.0 / dz2) / 2.0;
	if (tmp < deltat)
		deltat = tmp;
	deltat *= TAU;
	if (!deltat || deltat > T_DELTA || deltat < 0)
	deltat = T_DELTA;
	//printf("%lf\n", deltat);
	return (deltat);
}


int		ft_is_water(int flag)
{
	if (flag & C_F)
		return (TRUE);
	return (FALSE);
}

int		ft_is_obstacle(int flag)
{
	if (ft_is_water(flag))
		return (FALSE);
	return (TRUE);
}

int		ft_is_surface(int flag)
{
	if (ft_is_water(flag) && (flag & C_R))
		return (TRUE);
	return (FALSE);
}

int		ft_is_interior_water(int flag)
{
	if (ft_is_water(flag) && !ft_is_surface(flag))
		return (TRUE);
	return (FALSE);
}

int		ft_is_empty(int flag)
{
	if (flag == C_A)
		return (TRUE);
	return (FALSE);
}




void	ft_clear_all_params(void *param, int j, int i, int k)
{
	//надо уточнить, в каких квадратах не нужно зачищать скорости
	if (map[j][i][k] != WATER)
	{
		press_p[j][i][k] = 0.0;
		if (map[j][i + 1][k] != WATER)
			speed_u[j][i][k] = 0.0;
		if (map[j + 1][i][k] != WATER)
			speed_v[j][i][k] = 0.0;
		if (map[j][i][k + 1] != WATER)
			speed_w[j][i][k] = 0.0;
	}
	flow_f[j][i][k] = 0.0;
	flow_g[j][i][k] = 0.0;
	flow_h[j][i][k] = 0.0;
	lapl_u[j][i][k] = 0.0;
	lapl_v[j][i][k] = 0.0;
	lapl_w[j][i][k] = 0.0;
	rhs[j][i][k] = 0.0;
	tmp[j][i][k] = 0.0;
}


void	ft_clear_old_cell(void)
{

	t_point start;
	t_point end;

	ft_fill_point(&start, 1, 1, 1);
	ft_fill_point(&end, jmax, imax, kmax);
	//удаляем все парамтры клеток, не являющихся водными
	ft_cycle_cube(NULL, &ft_clear_all_params, &start, &end);
}




void	ft_surface_speed_and_pressure(void *param, int j, int i, int k)
{
	if (!flags_surface[j][i][k])
		return ;
	press_p[j][i][k] = 0.0;
	/*
	//надо бы это все допилить...
	ft_surface_speed_u(j, i, k);
	ft_surface_speed_v(j, i, k);
	ft_surface_speed_w(j, i, k);
	*/
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








void	ft_solver(t_fluid *fluid)
{

	//deltat = T_DELTA;
	deltat = ft_time_control();
	//проставляем флаги воды, воздуха и земли по карте
	ft_remark_flags();
	//удаляем все парамтры клеток, не являющихся водными
	ft_clear_old_cell();

	//считаем потоки для внутренних клеток воды, не граничащих с воздухом
	ft_set_uvp_surface(fluid);

	//определяем скорости в клетках на границе
	ft_setbcond(fluid);
	//считаем изменение потоков для внутренних клеток воды, не граничащих с воздухом
	ft_comp_fg(fluid);
	//определяем скорости в клетках на границе

	//ft_comp_rhs(fluid);

	//считаем давление для внутренних клеток воды, не граничащих с воздухом
	ft_poisson();

	//считаем скорости для внутренних клеток воды, не граничащих с воздухом
	ft_adap_uvw();
	//считаем потоки для внутренних клеток воды, не граничащих с воздухом
	//ft_set_uvp_surface(fluid);
	//стираем воду с карты, далее ее восстановят частицы
	ft_clear_map_from_water();
}
