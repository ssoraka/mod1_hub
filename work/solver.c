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

//mytype	dx = 5;
//mytype	dy = 5;
//mytype	dx2 = 25;
//mytype	dy2 = 25;
//int		imax = 10;
//int		jmax = 10;




#define CONST_GX 0

char		str[10][10] =
{
	{'1', '1', '1', '1', '1', '1', '1', '1', '1', '1'},
	{'1', '1', '1', '1', '1', '1', '1', '1', '1', '1'},
	{'1', '1', '1', '1', '1', '1', '1', '1', '1', '1'},
	{'1', '1', '1', '1', '1', '1', '1', '1', '1', '1'},
	{'1', '1', '1', '1', '1', '1', '1', '1', '1', '1'},
	{'1', '1', '1', '1', '1', '1', '1', '1', '1', '1'},
	{'1', '1', '1', '1', '1', '1', '1', '1', '1', '1'},
	{'1', '1', '1', '1', '1', '1', '1', '1', '1', '1'},
	{'1', '1', '1', '1', '1', '1', '1', '1', '1', '1'},
	{'1', '1', '1', '1', '1', '1', '1', '1', '1', '1'}
};



void	ft_print_fluid(t_fluid *fluid, REAL **arr)
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
			printf("%2.0lf ", arr[j][i]);
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



//REAL differential(REAL **value, int delta)

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
	if ((fluid->map)[j][i] == '1')
	{
		fluid->press_p[j][i] = P_CONST;
		fluid->speed_u[j][i] = U_CONST;
		//fluid->speed_v[j][i] = U_CONST;
	}
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


void	ft_speed_u_in_obstacle(REAL **speed, int j, int i, int flag)
{
	if (flag & B_N)
	{
		speed[j][i] = -speed[j + 1][i];
		speed[j][i - 1] = -speed[j + 1][i - 1];//хз, возможно лишняя строка
	}
	if (flag & B_S)
	{
		speed[j][i] = -speed[j - 1][i];
		speed[j][i - 1] = -speed[j - 1][i - 1];//хз, возможно лишняя строка
	}
	if (flag & B_E)
		speed[j][i] = 0;
	if (flag & B_W)
		speed[j][i - 1] = 0;
}


void	ft_speed_v_in_obstacle(REAL **speed, int j, int i, int flag)
{
	if (flag & B_E)
	{
		speed[j][i] = -speed[j][i + 1];
		speed[j - 1][i] = -speed[j - 1][i + 1];//хз, возможно лишняя строка
	}
	if (flag & B_W)
	{
		speed[j][i] = -speed[j][i - 1];
		speed[j - 1][i] = -speed[j - 1][i - 1];//хз, возможно лишняя строка
	}
	if (flag & B_N)
		speed[j][i] = 0;
	if (flag & B_S)
		speed[j - 1][i] = 0;
}


void	ft_fill_obstacle_in_center(void *ptr, int j, int i)
{
	t_fluid *fluid;

	fluid = (t_fluid *)ptr;
	if ((fluid->map)[j][i] == '0' && fluid->flags[j][i])
	{
		//проставляю давления и скорости
		//есть недостаток: у каждой клеточкинужно указывать давления и скорости
		//на двух ребрах, я ставлю на 3, происходит дублирование
		//но пока не понятно, где эти флаги еще пригодятся...
		ft_pressure_in_obstacle(fluid->press_p, j, i, fluid->flags[j][i]);
		ft_speed_u_in_obstacle(fluid->speed_u, j, i, fluid->flags[j][i]);
		ft_speed_v_in_obstacle(fluid->speed_v, j, i, fluid->flags[j][i]);
	}
}



/*
**граничные условия на верхней стенке
*/
void	ft_top_boundary(void *ptr, int j, int i)
{
	t_fluid *fluid;
	int jmax;
	int imax;

	fluid = (t_fluid *)ptr;
	jmax = fluid->jmax;
	imax = fluid->imax;
	fluid->press_p[jmax + 1][i] = fluid->press_p[jmax][i];
	fluid->tmp[jmax + 1][i] = fluid->press_p[jmax][i];
	fluid->speed_u[jmax + 1][i] = -fluid->speed_u[jmax][i];
	fluid->speed_v[jmax][i] = 0;
	fluid->flow_g[jmax][i] = fluid->speed_v[jmax][i];
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
	fluid->speed_u[j][0] = 0;
	fluid->speed_v[j][0] = -fluid->speed_v[j][1];
	fluid->flow_f[j][0] = fluid->speed_u[j][0];
}
/*
**граничные условия на правой стенке
*/
void	ft_right_boundary(void *ptr, int j, int i)
{
	t_fluid *fluid;
	int jmax;
	int imax;

	fluid = (t_fluid *)ptr;
	jmax = fluid->jmax;
	imax = fluid->imax;
	fluid->press_p[j][imax + 1] = fluid->press_p[j][imax];
	fluid->tmp[j][imax + 1] = fluid->press_p[j][imax];
	fluid->speed_u[j][imax] = 0;
	fluid->speed_v[j][imax + 1] = -fluid->speed_v[j][imax];
	fluid->flow_f[j][imax] = fluid->speed_u[j][imax];
}


void	ft_inisialization(t_fluid *fluid)
{
	t_iter iter;

	//ставим давление, скорости во внутренних клетках воды
	ft_fill_iterations(&iter, 1, fluid->jmax, 1, fluid->imax);
	ft_iteration((void *)fluid, &ft_fill_watercell_in_center, &iter);
	//fluid->speed_u[3][3] = 2;
}

void	ft_fill_watercell(t_fluid *fluid)
{
	t_iter iter;

	//граничные условия на стенках
	ft_fill_iterations(&iter, 0, fluid->jmax + 1, 0, 0);
	ft_iteration_j(fluid, &ft_left_boundary, &iter);
	ft_fill_iterations(&iter, 0, fluid->jmax + 1, fluid->imax, fluid->imax);
	ft_iteration_j(fluid, &ft_right_boundary, &iter);
	ft_fill_iterations(&iter, 0, 0, 0, fluid->imax + 1);
	ft_iteration_i(fluid, &ft_down_boundary, &iter);
	ft_fill_iterations(&iter, fluid->jmax, fluid->jmax, 0, fluid->imax + 1);
	ft_iteration_i(fluid, &ft_top_boundary, &iter);
	//ставим давление, скорости во внутренних клетках воды
	ft_fill_iterations(&iter, 1, fluid->jmax, 1, fluid->imax);
	ft_iteration((void *)fluid, &ft_fill_obstacle_in_center, &iter);
}


REAL	ft_laplasian(REAL **speed, int j, int i)
{
	REAL lapl;

	//сумма двойных дифференциалов
	lapl = ((speed[j][i + 1] - 2 * speed[j][i] + speed[j][i - 1]) / DELTA_X2 +
	(speed[j + 1][i] -	2 * speed[j][i] + speed[j - 1][i]) / DELTA_Y2) / CONST_RE;
	return (lapl);
}


void	ft_flow_f_and_flow_g(void *ptr, int j, int i)
{
	t_fluid *fluid;

	fluid = (t_fluid *)ptr;
	fluid->lapl_u[j][i] = ft_laplasian(fluid->speed_u, j, i);
	fluid->lapl_v[j][i] = ft_laplasian(fluid->speed_v, j, i);
	//потоки горизонтальный и вертикальный
	fluid->flow_f[j][i] = fluid->speed_u[j][i] + (fluid->lapl_u[j][i] + CONST_GX) * fluid->deltat;
	fluid->flow_g[j][i] = fluid->speed_v[j][i] + (fluid->lapl_v[j][i] + CONST_GY) * fluid->deltat;
}

void	ft_flow_f_or_flow_g(void *ptr, int j, int i)
{
	t_fluid *fluid;

	fluid = (t_fluid *)ptr;
	//потоки горизонтальный или вертикальный
	if (i < fluid->imax)
	{
		fluid->lapl_u[j][i] = ft_laplasian(fluid->speed_u, j, i);
		fluid->flow_f[j][i] = fluid->speed_u[j][i] + (fluid->lapl_u[j][i] + CONST_GX) * fluid->deltat;
	}
	if (j < fluid->jmax)
	{
		fluid->lapl_v[j][i] = ft_laplasian(fluid->speed_v, j, i);
		fluid->flow_g[j][i] = fluid->speed_v[j][i] + fluid->lapl_v[j][i] + CONST_GY * fluid->deltat;
	}
}



void	ft_right_hand_side(void *ptr, int j, int i)
{
	t_fluid *fluid;

	fluid = (t_fluid *)ptr;
	fluid->rhs[j][i] = ((fluid->flow_f[j][i] - fluid->flow_f[j][i - 1]) / DELTA_X +
	(fluid->flow_g[j][i] - fluid->flow_g[j - 1][i]) / DELTA_Y) / fluid->deltat;
}



void	ft_residual_pressure_center(void *ptr, int j, int i)
{
	REAL r_it;
	t_fluid *fluid;

	fluid = (t_fluid *)ptr;
	r_it =
	((fluid->press_p[j][i + 1] - fluid->press_p[j][i]) -
	(fluid->press_p[j][i] - fluid->press_p[j][i - 1])) / DELTA_X2 +
	((fluid->press_p[j + 1][i] - fluid->press_p[j][i]) -
	(fluid->press_p[j][i] - fluid->press_p[j - 1][i])) / DELTA_Y2 + fluid->rhs[j][i];
	r_it = r_it * r_it;
	fluid->eps += r_it;
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
	ft_check_params(fluid, ewns, i, j);
	r_it =
	(ewns[0] * (fluid->press_p[j][i + 1] - fluid->press_p[j][i]) -
	ewns[1] * (fluid->press_p[j][i] - fluid->press_p[j][i - 1])) / DELTA_X2 +
	(ewns[2] * (fluid->press_p[j + 1][i] - fluid->press_p[j][i]) -
	ewns[3] * (fluid->press_p[j][i] - fluid->press_p[j - 1][i])) / DELTA_Y2 + fluid->rhs[j][i];
	r_it = r_it * r_it;
	fluid->eps += r_it;
}


REAL	ft_residual_pressure(t_fluid *fluid)
{
	t_iter iter;

	fluid->eps = 0;
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
	fluid->eps = sqrt(fluid->eps) / fluid->jmax / fluid->imax;
	return (fluid->eps);
}



void	ft_boundary_aproximation_press(void *ptr, int j, int i)
{
	int ewns[4];
	t_fluid *fluid;

	fluid = (t_fluid *)ptr;
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
	fluid->tmp[j][i] =
	((fluid->press_p[j][i + 1] + fluid->tmp[j][i - 1])  * DELTA_Y2 +
	(fluid->press_p[j + 1][i] + fluid->tmp[j - 1][i]) * DELTA_X2 -
	fluid->rhs[j][i] * DELTA_X2 * DELTA_Y2) / (2 * (DELTA_X2 + DELTA_Y2));

}



REAL	**ft_new_pressure(t_fluid *fluid)
{
	t_iter iter;

	//заполняем столбец 1 и строку 1
	ft_fill_iterations(&iter, 1, fluid->jmax, 1, 1);
	ft_iteration_j((void *)fluid, &ft_boundary_aproximation_press, &iter);
	ft_fill_iterations(&iter, 1, 1, 1, fluid->imax);
	ft_iteration_i((void *)fluid, &ft_boundary_aproximation_press, &iter);

	ft_fill_iterations(&iter, 2, fluid->jmax - 1, 2, fluid->imax - 1);
	ft_iteration((void *)fluid, &ft_aproximation_press, &iter);
	//заполняем столбец imax и строку jmax
	ft_fill_iterations(&iter, 1, fluid->jmax, fluid->imax, fluid->imax);
	ft_iteration_j(fluid, &ft_boundary_aproximation_press, &iter);
	ft_fill_iterations(&iter, fluid->jmax, fluid->jmax, 1, fluid->imax);
	ft_iteration_i(fluid, &ft_boundary_aproximation_press, &iter);
	return (fluid->tmp);
}

void	ft_successive_overrelaxation(t_fluid *fluid)
{
	int it;
	REAL **new;

	it = 0;
	fluid->eps = T_INFINITY;
	/////исправить!!!!
	while (it < MAX_ITERATIONS && fluid->eps >= TOLERANCE)
	{
		//вычисляем давление приблизительно
		new = ft_new_pressure(fluid);
		fluid->tmp = fluid->press_p;
		fluid->press_p = new;
		ft_arr_set(fluid->tmp, fluid->imax + 1, 0.0);
		//смотрим разброс и повторяем расчет, в случае необходимости
		fluid->eps = ft_residual_pressure(fluid);
		//printf("%lf\n", fluid->eps);
		it++;
	}
}


void	ft_flows(t_fluid *fluid)
{
	t_iter iter;

	//вычисляем потоки и двойные дифференциалы для всех внутренних клеток
	ft_fill_iterations(&iter, 1, fluid->jmax - 1, 1, fluid->imax - 1);
	ft_iteration((void *)fluid, &ft_flow_f_and_flow_g, &iter);
	//вычисляем потоки и двойные дифференциалы для граничных клеток

	ft_fill_iterations(&iter, fluid->jmax, fluid->jmax, 1, fluid->imax);
	ft_iteration_i(fluid, &ft_flow_f_or_flow_g, &iter);
	ft_fill_iterations(&iter, 1, fluid->jmax, fluid->imax, fluid->imax);
	ft_iteration_j(fluid, &ft_flow_f_or_flow_g, &iter);
	ft_fill_iterations(&iter, 1, fluid->jmax, 1, fluid->imax);
	ft_iteration(fluid, &ft_right_hand_side, &iter);
}


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


void	ft_speed_u_and_speed_v(void *ptr, int j, int i)
{
	t_fluid *fluid;

	fluid = (t_fluid *)ptr;
	fluid->speed_u[j][i] = fluid->flow_f[j][i] - fluid->deltat / DELTA_X *
	(fluid->press_p[j][i + 1] - fluid->press_p[j][i]);
	fluid->speed_v[j][i] = fluid->flow_g[j][i] - fluid->deltat / DELTA_Y *
	(fluid->press_p[j + 1][i] - fluid->press_p[j][i]);
	ft_max_speed(fluid, fluid->speed_u[j][i], fluid->speed_v[j][i]);
}

void	ft_speed_u_or_speed_v(void *ptr, int j, int i)
{
	t_fluid *fluid;

	fluid = (t_fluid *)ptr;
	if (i < fluid->imax)
		fluid->speed_u[j][i] = fluid->flow_f[j][i] - fluid->deltat / DELTA_X *
		(fluid->press_p[j][i + 1] - fluid->press_p[j][i]);
	if (j < fluid->jmax)
		fluid->speed_v[j][i] = fluid->flow_g[j][i] - fluid->deltat / DELTA_Y *
		(fluid->press_p[j + 1][i] - fluid->press_p[j][i]);
	ft_max_speed(fluid, fluid->speed_u[j][i], fluid->speed_v[j][i]);
}

void	ft_new_velocity(t_fluid *fluid)
{
	t_iter iter;

	fluid->max_u = 0.0;
	fluid->max_v = 0.0;
	//вычисляем потоки и двойные дифференциалы для всех внутренних клеток
	ft_fill_iterations(&iter, 1, fluid->jmax - 1, 1, fluid->imax - 1);
	ft_iteration((void *)fluid, &ft_speed_u_and_speed_v, &iter);
	ft_fill_iterations(&iter, fluid->jmax, fluid->jmax, 1, fluid->imax);
	ft_iteration_i(fluid, &ft_speed_u_or_speed_v, &iter);
	ft_fill_iterations(&iter, 1, fluid->jmax, fluid->imax, fluid->imax);
	ft_iteration_j(fluid, &ft_speed_u_or_speed_v, &iter);
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
	tmp = CONST_RE / (1 / ((REAL)(dx * dx)) + 1 / ((REAL)(dy * dy))) / 2;
	if (tmp < deltat)
		deltat = tmp;
	deltat *= TAU;
	return (deltat);
}

char	**ft_create_map(int jmax, int imax)
{
	char **map;
	int i;
	int j;

	map = (char **)ft_mem_arr_new(jmax + 2, imax + 2, sizeof(char));
	if (!map)
		return (NULL);
	//столбец i=0 j=0 фиктивные
	j = 1;
	while (j <= jmax)
	{
		i = 1;
		while (i <= imax)
		{
			map[j][i] = str[j - 1][i - 1];
			i++;
		}
		j++;
	}
	return (map);
}



int		**ft_create_flags(char **map, int jmax, int imax)
{
	int **flags;
	int i;
	int j;

	flags = (int **)ft_mem_arr_new(jmax + 2, imax + 2, sizeof(int));
	if (!flags)
		return (NULL);
	j = 1;
	while (j <= jmax)
	{
		i = 1;
		while (i <= imax)
		{
			//нули стоят по умолчанию, это символ препятствия
			//if (i == 0 || i == imax + 1 || j == 0 || j == jmax + 1)
			//	flags[j][i] = C_B;
			//if (map[j][i])
			//	flags[j][i] = C_F;
			if (j > 1 && map[j - 1][i] == '1')
				flags[j][i] += B_S;
			if (j < jmax && map[j + 1][i] == '1')
				flags[j][i] += B_N;
			if (i > 1 && map[j][i - 1] == '1')
				flags[j][i] += B_W;
			if (i < imax && map[j][i + 1] == '1')
				flags[j][i] += B_E;
			i++;
		}
		j++;
	}
	return (flags);
}



t_fluid	*ft_initialization(char **map, int jmax, int imax)
{
	t_fluid *fluid;

	fluid = (t_fluid *)ft_memalloc(sizeof(t_fluid));
	fluid->imax = imax;
	fluid->jmax = jmax;
	fluid->map = map;
	fluid->flags = ft_create_flags(map, jmax, imax);
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
	ft_inisialization(fluid);
	return (fluid);
}


void	ft_solver(t_fluid *fluid)
{
	ft_fill_watercell(fluid);
	fluid->deltat = 0.5;//ft_time_control(DELTA_X, DELTA_Y, fluid->max_u, fluid->max_v);
	ft_flows(fluid);
	ft_successive_overrelaxation(fluid);
	ft_new_velocity(fluid);
	//ft_fill_watercell(fluid);
}


/*
int main(int argc, char **argv)
{

	char **map;



	map = ft_create_map();


	t_fluid *fluid;
	fluid = ft_initialization(map, jmax, imax);



	ft_print_flags(fluid, fluid->flags);
	ft_print_fluid(fluid, fluid->speed_u);
	ft_print_fluid(fluid, fluid->press_p);

	REAL t;


	t = 0.0;



	while (t < 100)
	{

		ft_solver(fluid);
		//ft_print_fluid(fluid, fluid->speed_u);
		t = t + fluid->deltat;
	}

	ft_print_fluid(fluid, fluid->speed_u);
	return (0);
}
*/
