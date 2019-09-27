/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/25 17:13:40 by ssoraka           #+#    #+#             */
/*   Updated: 2019/08/25 17:13:40 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_mod1.h"

void	ft_print_fluid(t_fluid *fluid, REAL **arr)
{
	int i;
	int j;

	j = 0;
	while (j <= fluid->jmax + 1)
	{
		printf("%d\t", j);
		i = 0;
		while (i <= fluid->imax + 1)
		{
			printf("%+.1le ", arr[j][i]);
			i++;
		}
		printf("\n");
		j++;
	}
}


void	ft_print_flags(t_fluid *fluid, int **arr)
{
	int i;
	int j;

	j = 0;
	while (j <= fluid->jmax + 1)
	{
		printf("%d\t", fluid->jmax + 1 - j);
		i = 0;
		while (i <= fluid->imax + 1)
		{
			printf("%3d  ", arr[j][i]);
			i++;
		}
		printf("\n");
		j++;
	}
}

void	ft_arr_set(REAL **arr, int columns, REAL value)
{
	int i;

	while (*arr)
	{
		i = 0;
		while (i <= columns)
		{
			(*arr)[i] = value;
			i++;
		}
		arr++;
	}
}


void	ft_fill_iterations(t_iter *iter, int j, int jmax, int i, int imax)
{
	iter->j = j;
	iter->i = i;
	iter->jmax = jmax;
	iter->imax = imax;
}


void	ft_iteration_i(void *ptr, void (*f)(void *, int, int), t_iter *iter)
{
	int i;

	i = iter->i;
	while (i <= iter->imax)
	{
		f(ptr, iter->j, i);
		i++;
	}
}

void	ft_iteration_j(void *ptr, void (*f)(void *, int, int), t_iter *iter)
{
	int j;

	j = iter->j;
	while (j <= iter->jmax)
	{
		f(ptr, j, iter->i);
		j++;
	}
}


void	ft_iteration(void *ptr, void (*f)(void *, int, int), t_iter *iter)
{
	int i;
	int j;

	j = iter->j;
	while (j <= iter->jmax)
	{
		i = iter->i;
		while (i <= iter->imax)
		{
			f(ptr, j, i);
			i++;
		}
		j++;
	}
}


void	ft_fill_watercell_in_center(void *ptr, int j, int i)
{
	t_fluid *fluid;

	fluid = (t_fluid *)ptr;

	if (fluid->map[j][i] == WATER)
	{
		fluid->press_p[j][i] = 0.0;// - P_CONST;
		//if (i < 3)
			fluid->speed_u[j][i] = U_CONST;
		//fluid->speed_v[j][i] = U_CONST;

	}
	//if (fluid->map[j][i] == WATER || (fluid->map[j][i] == SURF))
	//	fluid->parts[j][i] = PARTS_COUNT;
}






void	ft_pressure_in_obstacle(REAL **press, int j, int i, int flag)
{
	if (flag & B_N && flag & B_E)
		press[j][i] = (press[j + 1][i] + press[j][i + 1]) / 2;
	else if (flag & B_N && flag & B_W)
		press[j][i] = (press[j + 1][i] + press[j][i - 1]) / 2;
	else if (flag & B_S && flag & B_E)
		press[j][i] = (press[j - 1][i] + press[j][i + 1]) / 2;
	else if (flag & B_S && flag & B_W)
		press[j][i] = (press[j - 1][i] + press[j][i - 1]) / 2;
	else if (flag & B_N)
		press[j][i] = press[j + 1][i];
	else if (flag & B_E)
		press[j][i] = press[j][i + 1];
	else if (flag & B_S)
		press[j][i] = press[j - 1][i];
	else if (flag & B_W)
		press[j][i] = press[j][i - 1];
}










/*
**граничные условия на верхней стенке
*/
void	ft_top_boundary(void *ptr, int j, int i)
{
	t_fluid *fluid;

	fluid = (t_fluid *)ptr;
	fluid->press_p[j + 1][i] = fluid->press_p[j][i];
	fluid->tmp[j + 1][i] = fluid->press_p[j][i];
	fluid->speed_u[j + 1][i] = -fluid->speed_u[j][i];
	fluid->speed_v[j][i] = 0;
	fluid->flow_g[j][i] = fluid->speed_v[j][i];
}
/*
**граничные условия на нижней стенке
*/
void	ft_down_boundary(void *ptr, int j, int i)
{
	t_fluid *fluid;

	fluid = (t_fluid *)ptr;
	fluid->press_p[0][i] = fluid->press_p[1][i];
	fluid->tmp[0][i] = fluid->press_p[1][i];
	fluid->speed_u[0][i] = -fluid->speed_u[1][i];
	fluid->speed_v[0][i] = 0;
	fluid->flow_g[0][i] = fluid->speed_v[0][i];
}
/*
**граничные условия на левой стенке
*/
void	ft_left_boundary(void *ptr, int j, int i)
{
	t_fluid *fluid;

	fluid = (t_fluid *)ptr;
	fluid->press_p[j][0] = fluid->press_p[j][1];
	fluid->tmp[j][0] = fluid->press_p[j][1];
	//fluid->speed_u[j][1] = U_CONST;
	//fluid->speed_u[j][0] = -fluid->speed_u[j][1];
	fluid->speed_u[j][0] = 0;
	fluid->speed_u[j][1] = 0;
	fluid->speed_v[j][0] = -fluid->speed_v[j][1];
	fluid->flow_f[j][0] = fluid->speed_u[j][0];
}
/*
**граничные условия на правой стенке
*/
void	ft_right_boundary(void *ptr, int j, int i)
{
	t_fluid *fluid;

	fluid = (t_fluid *)ptr;
	fluid->press_p[j][i + 1] = fluid->press_p[j][i];
	fluid->tmp[j][i + 1] = fluid->press_p[j][i];
	//fluid->speed_u[j][i] = -fluid->speed_u[j][i - 1];
	fluid->speed_u[j][i] = 0;
	fluid->speed_v[j][i + 1] = -fluid->speed_v[j][i];
	fluid->flow_f[j][i] = fluid->speed_u[j][i];
}


void	ft_init_first_value(t_fluid *fluid)
{
	t_iter iter;

	//ставим давление, скорости во внутренних клетках воды
	ft_fill_iterations(&iter, 1, fluid->jmax, 1, fluid->imax);
	ft_iteration((void *)fluid, &ft_fill_watercell_in_center, &iter);
	//ft_fill_boundary_value(fluid);
	//fluid->speed_u[3][3] = 2;
}

/*
void	ft_fill_boundary_value(t_fluid *fluid)
{
	t_iter iter;

	//граничные условия на поверхности жидкости
	ft_fill_iterations(&iter, 1, fluid->jmax, 1, fluid->imax);
	//ft_iteration((void *)fluid, &ft_fill_surface_in_center, &iter);
	//граничные условия на стенках
	ft_fill_iterations(&iter, 0, fluid->jmax + 1, 0, 0);
	ft_iteration_j(fluid, &ft_left_boundary, &iter);
	ft_fill_iterations(&iter, 0, fluid->jmax + 1, fluid->imax, fluid->imax);
	ft_iteration_j(fluid, &ft_right_boundary, &iter);
	ft_fill_iterations(&iter, 0, 0, 0, fluid->imax + 1);
	ft_iteration_i(fluid, &ft_down_boundary, &iter);
	ft_fill_iterations(&iter, fluid->jmax, fluid->jmax, 0, fluid->imax + 1);
	ft_iteration_i(fluid, &ft_top_boundary, &iter);
	//граничные условия на стенках препятствий внутри жидкости
	ft_fill_iterations(&iter, 1, fluid->jmax, 1, fluid->imax);
	ft_iteration((void *)fluid, &ft_fill_obstacle_in_center, &iter);
}*/








void	ft_residual_pressure_center(void *ptr, int j, int i)
{
	REAL r_it;
	t_fluid *fluid;

	fluid = (t_fluid *)ptr;
	if (!ft_is_interior_water(fluid->flags[j][i]))
		return ;
	r_it =
	((fluid->press_p[j][i + 1] - fluid->press_p[j][i]) -
	(fluid->press_p[j][i] - fluid->press_p[j][i - 1])) / DELTA_X2 +
	((fluid->press_p[j + 1][i] - fluid->press_p[j][i]) -
	(fluid->press_p[j][i] - fluid->press_p[j - 1][i])) / DELTA_Y2 + fluid->rhs[j][i];
	r_it = r_it * r_it;
	fluid->eps += r_it;
	fluid->count++;
}


/*
**	функция нужна для расчета давлений на клетках, рядом с крайними
*/
void	ft_check_params(t_fluid *fluid, int *ewns, int i, int j)
{
	ewns[0] = 1;
	ewns[1] = 1;
	ewns[2] = 1;
	ewns[3] = 1;
	if (i == 1)
		ewns[0] = 0;
	else if (i == fluid->imax)
		ewns[1] = 0;
	if (j == 1)
		ewns[2] = 0;
	else if (j == fluid->jmax)
		ewns[3] = 0;
}


void	ft_residual_pressure_boundary(void *ptr, int j, int i)
{
	int ewns[4];
	REAL r_it;
	t_fluid *fluid;

	fluid = (t_fluid *)ptr;
	if (!ft_is_interior_water(fluid->flags[j][i]))
		return ;
	ft_check_params(fluid, ewns, i, j);
	r_it =
	(ewns[0] * (fluid->press_p[j][i + 1] - fluid->press_p[j][i]) -
	ewns[1] * (fluid->press_p[j][i] - fluid->press_p[j][i - 1])) / DELTA_X2 +
	(ewns[2] * (fluid->press_p[j + 1][i] - fluid->press_p[j][i]) -
	ewns[3] * (fluid->press_p[j][i] - fluid->press_p[j - 1][i])) / DELTA_Y2 + fluid->rhs[j][i];
	r_it = r_it * r_it;
	fluid->eps += r_it;
	fluid->count++;
}


REAL	ft_residual_pressure(t_fluid *fluid)
{
	t_iter iter;

	fluid->eps = 0;
	fluid->count = 0;
	ft_fill_iterations(&iter, 2, fluid->jmax - 1, 2, fluid->imax - 1);
	ft_iteration((void *)fluid, &ft_residual_pressure_center, &iter);
	ft_fill_iterations(&iter, 1, fluid->jmax, 1, 1);
	ft_iteration_j((void *)fluid, &ft_residual_pressure_boundary, &iter);
	ft_fill_iterations(&iter, 1, fluid->jmax, fluid->imax, fluid->imax);
	ft_iteration_j((void *)fluid, &ft_residual_pressure_boundary, &iter);
	ft_fill_iterations(&iter, 1, 1, 1, fluid->imax);
	ft_iteration_i((void *)fluid, &ft_residual_pressure_boundary, &iter);
	ft_fill_iterations(&iter, fluid->jmax, fluid->jmax, 1, fluid->imax);
	ft_iteration_i((void *)fluid, &ft_residual_pressure_boundary, &iter);
	if (fluid->count)
		fluid->eps = sqrt(fluid->eps) / fluid->count;
	return (fluid->eps);
}



void	ft_boundary_aproximation_press(void *ptr, int j, int i)
{
	int ewns[4];
	t_fluid *fluid;

	fluid = (t_fluid *)ptr;
	if (!ft_is_interior_water(fluid->flags[j][i]))
		return ;
	ft_check_params(fluid, ewns, i, j);
	fluid->tmp[j][i] = (1 - W_CONST) * fluid->press_p[j][i] +
	W_CONST / ((REAL)(ewns[0] + ewns[1]) / DELTA_X2 +
	(REAL)(ewns[2] + ewns[3]) / DELTA_Y2) *
	((ewns[0] * fluid->press_p[j][i + 1] + ewns[1] *
	fluid->tmp[j][i - 1])  / DELTA_X2 + (ewns[2] * fluid->press_p[j + 1][i] +
	ewns[3] * fluid->tmp[j - 1][i]) / DELTA_Y2 -
	fluid->rhs[j][i]);
}

void	ft_aproximation_press(void *ptr, int j, int i)
{
	t_fluid *fluid;

	fluid = (t_fluid *)ptr;
	//
/*
	fluid->tmp[j][i] = (1 - W_CONST) * fluid->press_p[j][i] + W_CONST *
	((fluid->press_p[j][i + 1] + fluid->tmp[j][i - 1])  * dy2 +
	(fluid->press_p[j + 1][i] + fluid->tmp[j - 1][i]) * dx2 -
	fluid->rhs[j][i] * dx2 * dy2) / (2 * (dx2 + dy2));
*/
	fluid->tmp[j][i] = fluid->press_p[j][i];
	if (!ft_is_interior_water(fluid->flags[j][i]))
		return ;
	/*fluid->tmp[j][i] =
	((fluid->press_p[j][i + 1] + fluid->tmp[j][i - 1])  * DELTA_Y2 +
	(fluid->press_p[j + 1][i] + fluid->tmp[j - 1][i]) * DELTA_X2 -
	fluid->rhs[j][i] * DELTA_X2 * DELTA_Y2) / (2 * (DELTA_X2 + DELTA_Y2));*/



	fluid->tmp[j][i] =
	(-(fluid->press_p[j][i + 1] / (DELTA_X2)
	+ fluid->press_p[j][i - 1] / (DELTA_X2)
	+ fluid->press_p[j + 1][i] / (DELTA_Y2)
	+ fluid->press_p[j - 1][i] / (DELTA_Y2))
	+ (1.0 / fluid->deltat) * (
	(fluid->flow_f[j][i] - fluid->flow_f[j][i - 1]) / DELTA_X
	+ (fluid->flow_g[j][i] - fluid->flow_g[j - 1][i]) / DELTA_Y)) / (-2 / (DELTA_X2) - 2 / (DELTA_Y2));



}



REAL	**ft_new_pressure(t_fluid *fluid)
{
	t_iter iter;

	//заполняем столбец 1 и строку 1
	/*ft_fill_iterations(&iter, 1, fluid->jmax, 1, 1);
	ft_iteration_j((void *)fluid, &ft_boundary_aproximation_press, &iter);
	ft_fill_iterations(&iter, 1, 1, 1, fluid->imax);
	ft_iteration_i((void *)fluid, &ft_boundary_aproximation_press, &iter);*/

	ft_fill_iterations(&iter, 1, fluid->jmax, 1, fluid->imax);
	ft_iteration((void *)fluid, &ft_aproximation_press, &iter);
	//заполняем столбец imax и строку jmax
	//ft_fill_iterations(&iter, 1, fluid->jmax, fluid->imax, fluid->imax);
	/*ft_iteration_j(fluid, &ft_boundary_aproximation_press, &iter);
	ft_fill_iterations(&iter, fluid->jmax, fluid->jmax, 1, fluid->imax);
	ft_iteration_i(fluid, &ft_boundary_aproximation_press, &iter);*/
	return (fluid->tmp);
}

void	ft_successive_overrelaxation(t_fluid *fluid)
{
	int it;
	REAL **new;

	it = 0;
	fluid->eps = T_INFINITY;
	/////исправить!!!!
	while (it < MAX_ITERATIONS)// && fluid->eps >= TOLERANCE)
	{
		//вычисляем давление приблизительно
		new = ft_new_pressure(fluid);
		fluid->tmp = fluid->press_p;
		fluid->press_p = new;
		ft_arr_set(fluid->tmp, fluid->imax + 1, 0.0);
		//смотрим разброс и повторяем расчет, в случае необходимости
		//fluid->eps = ft_residual_pressure(fluid);
		//printf("%lf\n", fluid->eps);
		it++;
	}
}



void	ft_fill_boundary_pressure(t_fluid *fluid)
{
	int j;
	int i;

	j = 1;
	while (j <= fluid->jmax)
	{
		fluid->press_p[j][0] = fluid->press_p[j][1];
		fluid->press_p[j][fluid->imax + 1] = fluid->press_p[j][fluid->imax];
		j++;
	}
	i = 1;
	while (i <= fluid->imax)
	{
		fluid->press_p[0][i] = fluid->press_p[1][i];
		fluid->press_p[fluid->jmax + 1][i] = fluid->press_p[fluid->jmax][i];
		i++;
	}
}

void	ft_fill_obstacles_pressure(void *ptr, int j, int i)
{
	t_fluid *fluid;

	fluid = (t_fluid *)ptr;
	if (fluid->map[j][i] != OBSTACLES)
		return ;
	if (fluid->map[j][i + 1] == WATER)
		fluid->press_p[j][i] = fluid->press_p[j][i + 1];
	else if (fluid->map[j][i - 1] == WATER)
		fluid->press_p[j][i] = fluid->press_p[j][i - 1];
	else if (fluid->map[j + 1][i] == WATER)
		fluid->press_p[j][i] = fluid->press_p[j + 1][i];
	else if (fluid->map[j - 1][i] == WATER)
		fluid->press_p[j][i] = fluid->press_p[j - 1][i];
	if (fluid->map[j][i - 1] == WATER && fluid->map[j + 1][i] == WATER)
		fluid->press_p[j][i] = (fluid->press_p[j][i - 1] + fluid->press_p[j + 1][i]) / 2;
	if (fluid->map[j][i + 1] == WATER && fluid->map[j + 1][i] == WATER)
		fluid->press_p[j][i] = (fluid->press_p[j][i + 1] + fluid->press_p[j + 1][i]) / 2;
	if (fluid->map[j][i - 1] == WATER && fluid->map[j - 1][i] == WATER)
		fluid->press_p[j][i] = (fluid->press_p[j][i - 1] + fluid->press_p[j - 1][i]) / 2;
	if (fluid->map[j][i + 1] == WATER && fluid->map[j - 1][i] == WATER)
		fluid->press_p[j][i] = (fluid->press_p[j][i + 1] + fluid->press_p[j - 1][i]) / 2;
}


void	ft_poisson(t_fluid *fluid)
{
	t_iter iter;

	//ft_print_fluid(fluid, fluid->press_p);
	ft_fill_iterations(&iter, 1, fluid->jmax, 1, fluid->imax);
	ft_iteration((void *)fluid, &ft_fill_obstacles_pressure, &iter);
	ft_fill_boundary_pressure(fluid);
	ft_successive_overrelaxation(fluid);
}

REAL	ft_laplasian(REAL **speed, int j, int i)
{
	REAL lapl;

	//сумма двойных дифференциалов
	lapl = ((speed[j][i + 1] - 2 * speed[j][i] + speed[j][i - 1]) / DELTA_X2 +
	(speed[j + 1][i] -	2 * speed[j][i] + speed[j - 1][i]) / DELTA_Y2) / CONST_RE;
	return (lapl);
}

void	ft_flow_f(void *ptr, int j, int i)
{
	t_fluid *fluid;

	fluid = (t_fluid *)ptr;
	if ((fluid->map)[j][i] != WATER || (fluid->map)[j][i + 1] != WATER)
	 	return ;
	//поток горизонтальный
	//fluid->lapl_u[j][i] = ft_laplasian(fluid->speed_u, j, i);
	//fluid->speed_u[j][i] = fluid->flow_f[j][i] + (fluid->lapl_u[j][i] + CONST_GX) * fluid->deltat;

	REAL cx;
	REAL cy;

	cx = fluid->deltat / DELTA_X2 / CONST_RE;
	cy = fluid->deltat / DELTA_Y2 / CONST_RE;

	fluid->speed_u[j][i] = (
	cx * fluid->speed_u[j][i + 1] +
	cx * fluid->speed_u[j][i - 1] +
	cy * fluid->speed_u[j + 1][i] +
	cy * fluid->speed_u[j - 1][i] +
	fluid->flow_f[j][i] + fluid->deltat * CONST_GX) / (1 + cx * 2 + cy * 2);


}

void	ft_flow_g(void *ptr, int j, int i)
{
	t_fluid *fluid;

	fluid = (t_fluid *)ptr;
	if ((fluid->map)[j][i] != WATER || (fluid->map)[j + 1][i] != WATER)
		return ;
	//поток вертикальный
	//fluid->lapl_v[j][i] = ft_laplasian(fluid->speed_v, j, i);
	//fluid->speed_v[j][i] = fluid->flow_g[j][i] + (fluid->lapl_v[j][i] + CONST_GY) * fluid->deltat;

	REAL cx;
	REAL cy;

	cx = fluid->deltat / DELTA_X2 / CONST_RE;
	cy = fluid->deltat / DELTA_Y2 / CONST_RE;

	fluid->speed_v[j][i] = (
	cx * fluid->speed_v[j][i + 1] +
	cx * fluid->speed_v[j][i - 1] +
	cy * fluid->speed_v[j + 1][i] +
	cy * fluid->speed_v[j - 1][i] +
	fluid->flow_g[j][i] + fluid->deltat * CONST_GY) / (1 + cx * 2 + cy * 2);

}


void	ft_fill_flows(void *ptr, int j, int i)
{
	t_fluid *fluid;

	fluid = (t_fluid *)ptr;
	fluid->flow_f[j][i] = fluid->speed_u[j][i];
	fluid->flow_g[j][i] = fluid->speed_v[j][i];
}

void	ft_recalc_flows(t_fluid *fluid)
{
	t_iter iter;

	ft_fill_iterations(&iter, 1, fluid->jmax, 1, fluid->imax - 1);
	ft_iteration(fluid, &ft_flow_f, &iter);
	ft_fill_iterations(&iter, 1, fluid->jmax - 1, 1, fluid->imax);
	ft_iteration(fluid, &ft_flow_g, &iter);
}


void	ft_right_hand_side(void *ptr, int j, int i)
{
	t_fluid *fluid;

	fluid = (t_fluid *)ptr;
	if ((fluid->map)[j][i] != WATER || (fluid->map)[j - 1][i] != WATER || (fluid->map)[j][i - 1] != WATER)
		return ;
	fluid->rhs[j][i] = ((fluid->flow_f[j][i] - fluid->flow_f[j][i - 1]) / DELTA_X +
	(fluid->flow_g[j][i] - fluid->flow_g[j - 1][i]) / DELTA_Y) / fluid->deltat;
}


void	ft_comp_fg(t_fluid *fluid)
{
	t_iter iter;
	int i;

	//сохраняем начальные скорости в потоках
	ft_fill_iterations(&iter, 0, fluid->jmax + 1, 0, fluid->imax + 1);
	ft_iteration((void *)fluid, &ft_fill_flows, &iter);

	//вычисляем потоки и двойные дифференциалы
	i = 0;
	while (i < 6) //надо уточнить необходимость 6-и итераций...
	{
		ft_recalc_flows(fluid);
		i++;
	}

	//сохраняем начальные скорости в потоках
	ft_fill_iterations(&iter, 0, fluid->jmax + 1, 0, fluid->imax + 1);
	ft_iteration((void *)fluid, &ft_fill_flows, &iter);
}

void	ft_comp_rhs(t_fluid *fluid)
{
	t_iter iter;

	ft_fill_iterations(&iter, 1, fluid->jmax, 1, fluid->imax);
	ft_iteration(fluid, &ft_right_hand_side, &iter);
}

/*
void	ft_max_speed(t_fluid *fluid, REAL speed_u, REAL speed_v)
{
	if (speed_u < 0)
		speed_u = -speed_u;
	if (fluid->max_u < speed_u)
		fluid->max_u = speed_u;
	if (speed_v < 0)
		speed_v = -speed_v;
	if (fluid->max_v < speed_v)
		fluid->max_v = speed_v;
}
*/

void	ft_speed_u(void *ptr, int j, int i)
{
	t_fluid *fluid;

	fluid = (t_fluid *)ptr;
	if ((fluid->map)[j][i] != WATER || (fluid->map)[j][i + 1] != WATER)
		return;
	fluid->speed_u[j][i] = fluid->flow_f[j][i] - fluid->deltat / DELTA_X *
	(fluid->press_p[j][i + 1] - fluid->press_p[j][i]);
	//ft_max_speed(fluid, fluid->speed_u[j][i], 0);
}

void	ft_speed_v(void *ptr, int j, int i)
{
	t_fluid *fluid;

	fluid = (t_fluid *)ptr;
	if ((fluid->map)[j][i] != WATER || (fluid->map)[j + 1][i] != WATER)
		return;
	fluid->speed_v[j][i] = fluid->flow_g[j][i] - fluid->deltat / DELTA_Y *
	(fluid->press_p[j + 1][i] - fluid->press_p[j][i]);
	//ft_max_speed(fluid, 0, fluid->speed_v[j][i]);
}

void	ft_adap_uv(t_fluid *fluid)
{
	t_iter iter;

	fluid->max_u = 0.0;
	fluid->max_v = 0.0;
	//вычисляем горизонтальные скороти
	ft_fill_iterations(&iter, 1, fluid->jmax, 1, fluid->imax - 1);
	ft_iteration((void *)fluid, &ft_speed_u, &iter);
	//вычисляем горизонтальные скороти
	ft_fill_iterations(&iter, 1, fluid->jmax - 1, 1, fluid->imax);
	ft_iteration((void *)fluid, &ft_speed_v, &iter);
}


REAL	ft_time_control(mytype dx, mytype dy, REAL max_u, REAL max_v)
{
	REAL deltat;
	REAL tmp;

	deltat = T_INFINITY;
	if (max_u < 0)
		max_u = -max_u;
	if (max_v < 0)
		max_v = -max_v;
	if (max_u && (REAL)dx / max_u < deltat)
		deltat = dx / max_u;
	if (max_v && (REAL)dy / max_v < deltat)
		deltat = dy / max_v;
	tmp = (REAL)CONST_RE / (1.0 / ((REAL)(dx * dx)) + 1.0 / ((REAL)(dy * dy))) / 2.0;
	if (tmp < deltat)
		deltat = tmp;
	deltat *= TAU;
	if (!deltat)
	deltat = T_DELTA;
	return (deltat);
}





int		ft_near_water_mask(t_fluid *fluid, int j, int i)
{
	char **map;
	int **flags;
	int mask;

	map = fluid->map;
	flags = fluid->flags;
	mask = 0;
	if (j > 1 && map[j - 1][i] == WATER)
		mask |= B_S;
	if (j < fluid->jmax && map[j + 1][i] == WATER)
		mask |= B_N;
	if (i > 1 && map[j][i - 1] == WATER)
		mask |= B_W;
	if (i < fluid->imax && map[j][i + 1] == WATER)
		mask |= B_E;
	return (mask);
}



int		ft_is_water(int flag)
{
	if (flag & C_F)
		return (TRUE);
	return (FALSE);
}

int		ft_is_surface(int flag)
{
	if (ft_is_water(flag) && (flag & C_S))
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


int		ft_change_flag(int flag, int mask)
{
	int		new_flag;

	new_flag = (!(flag & mask)) & mask;
	flag = flag & (!mask);
	flag = flag | new_flag;
	return (flag);
}




int		ft_set_flag_for(int map_value)
{
	int flag;

	if (map_value == WATER)
		flag = C_F;
	else if (map_value == OBSTACLES)
		flag = C_B;
	else
		flag = C_E;
	return (flag);
}


void	ft_mark_cell(void *ptr, int j, int i)
{
	t_fluid *fluid;

	fluid = (t_fluid *)ptr;
	fluid->flags[j][i] = ft_set_flag_for(fluid->map[j][i]);
}

void	ft_set_flag_to_obstacles(void *ptr, int j, int i)
{
	t_fluid *fluid;

	fluid = (t_fluid *)ptr;
	fluid->flags[j][i] = ft_set_flag_for(OBSTACLES);
}

void	ft_clear_water_flag(void *ptr, int j, int i)
{
	t_fluid *fluid;

	fluid = (t_fluid *)ptr;
	fluid->flags[j][i] = ft_set_flag_for(EMPTY);
}


void	ft_remark_flags(t_fluid *fluid)
{
	t_iter iter;

	//проставляем флаги воды, воздуха и земли
	ft_fill_iterations(&iter, 1, fluid->jmax, 1, fluid->imax);
	ft_iteration((void *)fluid, &ft_mark_cell, &iter);
}


int		**ft_create_flags(t_fluid *fluid)
{
	t_iter iter;

	fluid->flags = (int **)ft_mem_arr_new(fluid->jmax + 2, fluid->imax + 2, sizeof(int));
	if (!fluid->flags)
		return (NULL);

	//проставляем флаги по периметру, как препятствия, надо будет это еще проверить...
	ft_fill_iterations(&iter, 0, 0, 0, fluid->imax + 1);
	ft_iteration_i((void *)fluid, &ft_set_flag_to_obstacles, &iter);
	ft_fill_iterations(&iter, fluid->jmax + 1, fluid->jmax + 1, 0, fluid->imax + 1);
	ft_iteration_i((void *)fluid, &ft_set_flag_to_obstacles, &iter);
	ft_fill_iterations(&iter, 1, fluid->jmax, 0, 0);
	ft_iteration_j((void *)fluid, &ft_set_flag_to_obstacles, &iter);
	ft_fill_iterations(&iter, 1, fluid->jmax, fluid->imax + 1, fluid->imax + 1);
	ft_iteration_j((void *)fluid, &ft_set_flag_to_obstacles, &iter);
	return (fluid->flags);
}




t_fluid	*ft_initialization(char **map, int jmax, int imax)
{
	t_fluid *fluid;

	fluid = (t_fluid *)ft_memalloc(sizeof(t_fluid));
	fluid->imax = imax;
	fluid->jmax = jmax;
	//printf("%d_%d\n", jmax, imax);
	fluid->map = map;
	fluid->flags = ft_create_flags(fluid);
	fluid->flags_surface = (int **)ft_mem_arr_new(jmax + 2, imax + 2, sizeof(int));
	fluid->tmp = (REAL **)ft_mem_arr_new(jmax + 2, imax + 2, sizeof(REAL));
	fluid->speed_v = (REAL **)ft_mem_arr_new(jmax + 2, imax + 2, sizeof(REAL));
	fluid->speed_u = (REAL **)ft_mem_arr_new(jmax + 2, imax + 2, sizeof(REAL));
	fluid->press_p = (REAL **)ft_mem_arr_new(jmax + 2, imax + 2, sizeof(REAL));
	fluid->lapl_u = (REAL **)ft_mem_arr_new(jmax + 2, imax + 2, sizeof(REAL));
	fluid->lapl_v = (REAL **)ft_mem_arr_new(jmax + 2, imax + 2, sizeof(REAL));
	fluid->flow_f = (REAL **)ft_mem_arr_new(jmax + 2, imax + 2, sizeof(REAL));
	fluid->flow_g = (REAL **)ft_mem_arr_new(jmax + 2, imax + 2, sizeof(REAL));
	fluid->rhs = (REAL **)ft_mem_arr_new(jmax + 2, imax + 2, sizeof(REAL));
	fluid->max_u = U_CONST * 5;
	fluid->max_v = U_CONST * 5;
	ft_init_first_value(fluid);
	return (fluid);
}


void	ft_clear_speed_u(void *ptr, int j, int i)
{
	t_fluid *fluid;

	fluid = (t_fluid *)ptr;
	if (fluid->map[j][i] == EMPTY && fluid->map[j][i + 1] == EMPTY)
		fluid->speed_u[j][i] = 0.0;
}

void	ft_clear_speed_v(void *ptr, int j, int i)
{
	t_fluid *fluid;

	fluid = (t_fluid *)ptr;
	if (fluid->map[j][i] == EMPTY && fluid->map[j + 1][i] == EMPTY)
		fluid->speed_v[j][i] = 0.0;
}


void	ft_clear_all_params(void *ptr, int j, int i)
{
	t_fluid *fluid;

	fluid = (t_fluid *)ptr;
	if (fluid->map[j][i] == EMPTY)
	{
		fluid->speed_u[j][i] = 0.0;
		fluid->speed_v[j][i] = 0.0;
		fluid->press_p[j][i] = 0.0;
	}
	fluid->flow_f[j][i] = 0.0;
	fluid->flow_g[j][i] = 0.0;
	fluid->lapl_u[j][i] = 0.0;
	fluid->lapl_v[j][i] = 0.0;
	fluid->rhs[j][i] = 0.0;
	fluid->tmp[j][i] = 0.0;
}



void	ft_clear_surface(void *ptr, int j, int i)
{
	((int **)ptr)[j][i] = 0;
}


void	ft_update_surface(void *ptr, int j, int i)
{
	t_fluid *fluid;

	fluid = (t_fluid *)ptr;
	if (fluid->map[j][i] == WATER)
	{
		if (fluid->map[j][i - 1] == EMPTY)
			fluid->flags_surface[j][i] |= C_W;
		if (fluid->map[j][i + 1] == EMPTY)
			fluid->flags_surface[j][i] |= C_E;
		if (fluid->map[j - 1][i] == EMPTY)
			fluid->flags_surface[j][i] |= C_S;
		if (fluid->map[j + 1][i] == EMPTY)
			fluid->flags_surface[j][i] |= C_N;
	}
}

	//магическая функция с условиями!!!
void	ft_surface_speed_and_pressure(void *ptr, int j, int i)
{
	t_fluid *fluid;

	fluid = (t_fluid *)ptr;
	if (!fluid->flags_surface[j][i])
		return ;

	fluid->press_p[j][i] = 0.0;

	if (fluid->flags_surface[j][i] == C_N)
	{
		fluid->speed_v[j][i] = fluid->speed_v[j - 1][i];
		if (fluid->map[j + 1][i - 1] == EMPTY)
			fluid->speed_u[j + 1][i - 1] = fluid->speed_u[j][i - 1];
	}
	else if (fluid->flags_surface[j][i] == C_S)
	{
		fluid->speed_v[j - 1][i] = fluid->speed_v[j][i];
		if (fluid->map[j - 1][i - 1] == EMPTY)
			fluid->speed_u[j - 1][i - 1] = fluid->speed_u[j][i - 1];
	}
	else if (fluid->flags_surface[j][i] == C_E)
	{
		fluid->speed_u[j][i] = fluid->speed_u[j][i - 1];
		if (fluid->map[j - 1][i + 1] == EMPTY)
			fluid->speed_v[j - 1][i + 1] = fluid->speed_v[j - 1][i];
	}
	else if (fluid->flags_surface[j][i] == C_W)
	{
		fluid->speed_u[j][i - 1] = fluid->speed_u[j][i];
		if (fluid->map[j - 1][i - 1] == EMPTY)
			fluid->speed_v[j - 1][i - 1] = fluid->speed_v[j - 1][i];
	}
	else if (fluid->flags_surface[j][i] == C_NE)
	{
		fluid->speed_u[j][i] = fluid->speed_u[j][i - 1];
		fluid->speed_v[j][i] = fluid->speed_v[j - 1][i];
		if (fluid->map[j + 1][i - 1] == EMPTY)
			fluid->speed_u[j + 1][i - 1] = fluid->speed_u[j][i - 1];
		if (fluid->map[j + 1][i + 1] == EMPTY)
		{
			fluid->speed_u[j + 1][i] = fluid->speed_u[j][i];
			fluid->speed_v[j][i + 1] = fluid->speed_v[j][i];
		}
		if (fluid->map[j - 1][i + 1] == EMPTY)
			fluid->speed_v[j - 1][i + 1] = fluid->speed_v[j - 1][i];
	}
	else if (fluid->flags_surface[j][i] == C_NW)
	{
		fluid->speed_u[j][i - 1] = fluid->speed_u[j][i];
		fluid->speed_v[j][i] = fluid->speed_v[j - 1][i];
		if (fluid->map[j + 1][i - 1] == EMPTY)
		{
			fluid->speed_u[j + 1][i - 1] = fluid->speed_u[j][i - 1];
			fluid->speed_v[j][i - 1] = fluid->speed_v[j][i];
		}
		if (fluid->map[j - 1][i - 1] == EMPTY)
			fluid->speed_v[j - 1][i - 1] = fluid->speed_v[j - 1][i];
	}
	else if (fluid->flags_surface[j][i] == C_SW)
	{
		fluid->speed_u[j][i - 1] = fluid->speed_u[j][i];
		fluid->speed_v[j - 1][i] = fluid->speed_v[j][i];
		if (fluid->map[j - 1][i - 1] == EMPTY)
		{
			fluid->speed_u[j - 1][i - 1] = fluid->speed_u[j][i - 1];
			fluid->speed_v[j - 1][i - 1] = fluid->speed_v[j - 1][i];
		}
	}
	else if (fluid->flags_surface[j][i] == C_SE)
	{
		fluid->speed_u[j][i] = fluid->speed_u[j][i - 1];
		fluid->speed_v[j - 1][i] = fluid->speed_v[j][i];
		if (fluid->map[j - 1][i - 1] == EMPTY)
			fluid->speed_u[j - 1][i - 1] = fluid->speed_u[j][i - 1];
		if (fluid->map[j - 1][i + 1] == EMPTY)
		{
			fluid->speed_u[j - 1][i] = fluid->speed_u[j][i];
			fluid->speed_v[j - 1][i + 1] = fluid->speed_v[j - 1][i];
		}
	}
}


void	ft_set_uvp_surface(t_fluid *fluid)
{
	t_iter iter;

	//зачищаем весь массив флагов для границы воды.
	ft_fill_iterations(&iter, 0, fluid->jmax + 1, 0, fluid->imax + 1);
	ft_iteration((void *)(fluid->flags_surface), &ft_clear_surface, &iter);

	//заполняем флаги для границы воды.
	ft_fill_iterations(&iter, 1, fluid->jmax, 1, fluid->imax);
	ft_iteration((void *)fluid, &ft_update_surface, &iter);

	//проставляем граничные условия скоростей для границы воды.
	ft_fill_iterations(&iter, 1, fluid->jmax, 1, fluid->imax);
	ft_iteration((void *)fluid, &ft_surface_speed_and_pressure, &iter);
}

void	ft_clear_old_cell(t_fluid *fluid)
{
	t_iter iter;

	//удаляем горизонтальные скорости между пустыми клетками
	ft_fill_iterations(&iter, 1, fluid->jmax, 1, fluid->imax - 1);
	ft_iteration((void *)fluid, &ft_clear_speed_u, &iter);

	//удаляем вертикальные скорости между пустыми клетками
	ft_fill_iterations(&iter, 1, fluid->jmax - 1, 1, fluid->imax);
	ft_iteration((void *)fluid, &ft_clear_speed_v, &iter);

	//зачищаем ваще все
	ft_fill_iterations(&iter, 1, fluid->jmax, 1, fluid->imax);
	ft_iteration((void *)fluid, &ft_clear_all_params, &iter);

}


void	ft_fill_boundary_speed_uv(t_fluid *fluid)
{
	int j;
	int i;

	j = 0;
	while (j <= fluid->jmax + 1)
	{
		fluid->speed_u[j][0] = 0.0;
		fluid->speed_v[j][0] = -fluid->speed_v[j][1];

		fluid->speed_u[j][fluid->imax] = 0.0;
		//fluid->speed_u[j][fluid->imax + 1] = 0.0;
		fluid->speed_v[j][fluid->imax + 1] = -fluid->speed_v[j][fluid->imax];
		j++;
	}
	i = 0;
	while (i <= fluid->imax + 1)
	{
		fluid->speed_u[0][i] = -fluid->speed_u[1][i];
		fluid->speed_v[0][i] = 0.0;

		//fluid->speed_u[fluid->jmax][i] = 0.0;
		fluid->speed_u[fluid->jmax + 1][i] = -fluid->speed_u[fluid->jmax][i];
		fluid->speed_v[fluid->jmax][i] = 0.0;
		i++;
	}
}

void	ft_speed_u_in_obstacle2(REAL **speed, int j, int i, char **map)
{
	if (map[j + 1][i] == WATER)
	{
		speed[j][i] = -speed[j + 1][i];
		speed[j][i - 1] = -speed[j + 1][i - 1];//хз, возможно лишняя строка
	}
	if (map[j - 1][i] == WATER)
	{
		speed[j][i] = -speed[j - 1][i];
		speed[j][i - 1] = -speed[j - 1][i - 1];//хз, возможно лишняя строка
	}
	if (map[j][i + 1] == WATER)
		speed[j][i] = 0.0;
	if (map[j][i - 1] == WATER)
		speed[j][i - 1] = 0.0;
}

void	ft_speed_v_in_obstacle2(REAL **speed, int j, int i, char **map)
{
	if (map[j][i + 1] == WATER)
	{
		speed[j][i] = -speed[j][i + 1];
		speed[j - 1][i] = -speed[j - 1][i + 1];//хз, возможно лишняя строка
	}
	if (map[j][i - 1] == WATER)
	{
		speed[j][i] = -speed[j][i - 1];
		speed[j - 1][i] = -speed[j - 1][i - 1];//хз, возможно лишняя строка
	}
	if (map[j + 1][i] == WATER)
		speed[j][i] = 0.0;
	if (map[j - 1][i] == WATER)
		speed[j - 1][i] = 0.0;
	//if (i == 2)
	//	speed[j][i] = U_CONST;
}


void	ft_obstacles_speed_uv(void *ptr, int j, int i)
{
	t_fluid *fluid;

	fluid = (t_fluid *)ptr;
	if (fluid->map[j][i] != OBSTACLES)
		return ;
	if (!(fluid->flags[j][i] & WATER_MASK))
		return ;
	ft_speed_u_in_obstacle2(fluid->speed_u, j, i, fluid->map);
	ft_speed_v_in_obstacle2(fluid->speed_v, j, i, fluid->map);
}



void	ft_fill_obstacles_speed_uv(t_fluid *fluid)
{
	t_iter iter;

	ft_fill_iterations(&iter, 1, fluid->jmax, 1, fluid->imax);
	ft_iteration((void *)fluid, &ft_obstacles_speed_uv, &iter);
}


void	ft_setbcond(t_fluid *fluid)
{

	ft_fill_boundary_speed_uv(fluid);
	ft_fill_obstacles_speed_uv(fluid);

}


void	ft_clear_water(void *ptr, int j, int i)
{
	t_fluid *fluid;

	fluid = (t_fluid *)ptr;
	if (fluid->map[j][i] == WATER)
		fluid->map[j][i] = EMPTY;
}



void	ft_clear_map_from_water(t_fluid *fluid)
{
	t_iter iter;

	ft_fill_iterations(&iter, 1, fluid->jmax, 1, fluid->imax);
	ft_iteration((void *)fluid, &ft_clear_water, &iter);
}



void	ft_solver(t_fluid *fluid)
{

	fluid->deltat = T_DELTA;
	//проставляем флаги воды, воздуха и земли по карте
	ft_remark_flags(fluid);

	ft_clear_old_cell(fluid);

	//fluid->deltat = ft_time_control(DELTA_X, DELTA_Y, fluid->max_u, fluid->max_v);
	//считаем потоки для внутренних клеток воды, не граничащих с воздухом
	ft_set_uvp_surface(fluid);
	//определяем скорости в клетках на границе
	ft_setbcond(fluid);
	//считаем изменение потоков для внутренних клеток воды, не граничащих с воздухом
	ft_comp_fg(fluid);
	//определяем скорости в клетках на границе

	//ft_comp_rhs(fluid);

	//считаем давление для внутренних клеток воды, не граничащих с воздухом
	ft_poisson(fluid);

	//считаем скорости для внутренних клеток воды, не граничащих с воздухом
	ft_adap_uv(fluid);

	//стираем воду с карты, далее ее восстановят частицы
	ft_clear_map_from_water(fluid);
}


/*
	получаем карту
	создаем частицы по карте
	инициализируем начальные значения

	{
		все считаем
		стираем карту
		двигаем частицы, восстанавливаем карту
	}


*/
