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

#include "libft.h"

typedef double REAL;
typedef int mytype;

#define T_INFINITY 10000000000.0
#define P_CONST 100.0
#define U_CONST 2
#define W_CONST 2
#define CONST_RE 1000.0
#define CONST_GY 10
#define CONST_GX 0
#define T_END 100.0
#define TAU 0.7
#define MAX_ITERATIONS 10
#define TOLERANCE 10

#define C_B		0b000000
#define D_N		0b000001
#define D_S		0b000010
#define D_W		0b000100
#define D_E		0b001000
#define C_F		0b010000
#define C_A		0b100000


#define B_N		(D_S) //поменял для отображения
#define B_S		(D_N) //поменял для отображения
#define B_W		(D_W)
#define B_E		(D_E)
#define B_NE	(D_N + D_E)
#define B_SE	(D_S + D_E)
#define B_WE	(D_W + D_E)
#define B_NW	(D_N + D_W)
#define B_SW	(D_S + D_W)
#define B_EW	(D_E + D_W)
#define B_NS	(D_N + D_S)
#define B_WS	(D_W + D_S)
#define B_ES	(D_E + D_S)

mytype	dx = 100;
mytype	dy = 100;
int		imax = 10;
int		jmax = 10;


typedef struct		s_iter
{
	int				last;
	int				first;
}					t_iter;

typedef struct		s_fluid
{
	int				imax;
	int				jmax;
	mytype			dx;
	mytype			dy;
	REAL			eps;
	REAL			deltat;
	int				**map;
	int				**flags;
	REAL			**tmp;
	REAL			**speed_v;
	REAL			**speed_u;
	REAL			**press_p;
	REAL			**d2u_dx2_d2v_dy2;
	REAL			**flow_f;
	REAL			**flow_g;
	REAL			**rhs;
}					t_fluid;







#define CONST_GX 0

REAL		str[10][10] =
{
	{1, 1, 1, 1, 1, 1, 0, 1, 1, 1},
	{1, 1, 1, 1, 1, 1, 0, 1, 1, 1},
	{1, 1, 1, 1, 1, 1, 0, 1, 1, 1},
	{1, 1, 1, 1, 1, 1, 0, 1, 1, 1},
	{1, 1, 1, 1, 1, 1, 0, 1, 1, 1},
	{1, 1, 1, 1, 0, 1, 1, 1, 1, 1},
	{1, 1, 1, 1, 0, 1, 1, 1, 1, 0},
	{1, 1, 1, 1, 0, 1, 1, 0, 0, 0},
	{1, 1, 1, 1, 0, 1, 1, 0, 0, 0},
	{1, 1, 1, 1, 0, 1, 0, 0, 0, 0}
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
		while (i < columns)
		{
			(*arr)[i] = value;
			i++;
		}
		arr++;
	}
}


void	ft_fill_iterations(t_iter *i, int first, int last)
{
	i->first = first;
	i->last = last;
}


void	ft_iteration(t_fluid *fluid, void (*f)(t_fluid *, int, int), t_iter *j_it, t_iter *i_it)
{
	int i;
	int j;

	j = j_it->first;
	while (j <= j_it->last)
	{
		i = i_it->first;
		while (i <= i_it->last)
		{
			f(fluid, j, i);
			i++;
		}
		j++;
	}
}

void	ft_iteration_i(t_fluid *fluid, void (*f)(t_fluid *, int, int), int j, t_iter *i_it)
{
	int i;

	i = i_it->first;
	while (i <= i_it->last)
	{
		f(fluid, j, i);
		i++;
	}
}

void	ft_iteration_j(t_fluid *fluid, void (*f)(t_fluid *, int, int), t_iter *j_it, int i)
{
	int j;

	j = j_it->first;
	while (j <= j_it->last)
	{
		f(fluid, j, i);
		j++;
	}
}


void	ft_fill_watercell_in_center(t_fluid *fluid, int j, int i)
{
	if ((fluid->map)[j][i])
	{
		fluid->press_p[j][i] = P_CONST;
		fluid->speed_u[j][i] = U_CONST;
		fluid->speed_v[j][i] = U_CONST;
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


void	ft_fill_obstacle_in_center(t_fluid *fluid, int j, int i)
{
	if (!(fluid->map)[j][i] && fluid->flags[j][i])
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
void	ft_top_boundary(t_fluid *fluid, int j, int i)
{
	fluid->press_p[jmax + 1][i] = fluid->press_p[jmax][i];
	fluid->speed_u[jmax + 1][i] = -fluid->speed_u[jmax][i];
	fluid->speed_v[jmax][i] = 0;
	fluid->flow_g[jmax][i] = fluid->speed_v[jmax][i];
}
/*
**граничные условия на нижней стенке
*/
void	ft_down_boundary(t_fluid *fluid, int j, int i)
{
	fluid->press_p[0][i] = fluid->press_p[1][i];
	fluid->speed_u[0][i] = -fluid->speed_u[1][i];
	fluid->speed_v[0][i] = 0;
	fluid->flow_g[0][i] = fluid->speed_v[0][i];
}
/*
**граничные условия на левой стенке
*/
void	ft_left_boundary(t_fluid *fluid, int j, int i)
{
	fluid->press_p[j][0] = fluid->press_p[j][1];
	fluid->speed_u[j][0] = 0;
	fluid->speed_v[j][0] = -fluid->speed_v[j][1];
	fluid->flow_f[j][0] = fluid->speed_u[j][0];
}
/*
**граничные условия на правой стенке
*/
void	ft_right_boundary(t_fluid *fluid, int j, int i)
{
	fluid->press_p[j][imax + 1] = fluid->press_p[j][imax];
	fluid->speed_u[j][imax] = 0;
	fluid->speed_v[j][imax + 1] = -fluid->speed_v[j][imax];
	fluid->flow_f[j][imax] = fluid->speed_u[j][imax];
}


void	ft_inisialization(t_fluid *fluid)
{
	t_iter i;
	t_iter j;

	//ставим давление, скорости во внутренних клетках воды
	ft_fill_iterations(&i, 1, fluid->imax);
	ft_fill_iterations(&j, 1, fluid->jmax);
	ft_iteration(fluid, &ft_fill_watercell_in_center, &j, &i);
}



void	ft_fill_watercell(t_fluid *fluid)
{
	t_iter i;
	t_iter j;

	//ставим давление, скорости во внутренних клетках воды
	ft_fill_iterations(&i, 1, fluid->imax);
	ft_fill_iterations(&j, 1, fluid->jmax);
	ft_iteration(fluid, &ft_fill_obstacle_in_center, &j, &i);
	//граничные условия на стенках
	ft_fill_iterations(&j, 0, fluid->jmax + 1);
	ft_iteration_j(fluid, &ft_left_boundary, &j, 0);
	ft_iteration_j(fluid, &ft_right_boundary, &j, fluid->imax);
	ft_fill_iterations(&i, 0, fluid->imax + 1);
	ft_iteration_i(fluid, &ft_down_boundary, 0, &i);
	ft_iteration_i(fluid, &ft_top_boundary, fluid->jmax, &i);
}


void	ft_summ_of_differential(t_fluid *fluid, int j, int i)
{
	//сумма двойных дифференциалов
	fluid->d2u_dx2_d2v_dy2[j][i] = (fluid->speed_u[j][i + 1] -
	2 * fluid->speed_u[j][i] + fluid->speed_u[j][i - 1]) / (dx * dx) +
	(fluid->speed_v[j + 1][i] -	2 * fluid->speed_v[j][i] +
	fluid->speed_v[j - 1][i]) / (dy * dy);
	fluid->d2u_dx2_d2v_dy2[j][i] = fluid->deltat / CONST_RE * fluid->d2u_dx2_d2v_dy2[j][i];
}


void	ft_flow_f_and_flow_g(t_fluid *fluid, int j, int i)
{
	//сумма двойных дифференциалов
	ft_summ_of_differential(fluid, j, i);
	//потоки горизонтальный и вертикальный
	fluid->flow_f[j][i] = fluid->speed_u[j][i] + fluid->d2u_dx2_d2v_dy2[j][i];
	fluid->flow_g[j][i] = fluid->speed_v[j][i] + fluid->d2u_dx2_d2v_dy2[j][i];
}

void	ft_flow_f_or_flow_g(t_fluid *fluid, int j, int i)
{
	//сумма двойных дифференциалов
	ft_summ_of_differential(fluid, j, i);
	//потоки горизонтальный и вертикальный
	if (i < fluid->imax)
		fluid->flow_f[j][i] = fluid->speed_u[j][i] + fluid->d2u_dx2_d2v_dy2[j][i];
	if (j < fluid->jmax)
		fluid->flow_g[j][i] = fluid->speed_v[j][i] + fluid->d2u_dx2_d2v_dy2[j][i];
}



void	ft_right_hand_side(t_fluid *fluid, int j, int i)
{
	fluid->rhs[j][i] = ((fluid->flow_f[j][i] - fluid->flow_f[j][i - 1]) / dx +
	(fluid->flow_g[j][i] - fluid->flow_g[j - 1][i]) / dy) / fluid->deltat;
}



void	ft_residual_pressure2(t_fluid *fluid, int j, int i)
{
	REAL r_it;

	r_it =
	((fluid->press_p[j][i + 1] - fluid->press_p[j][i]) -
	(fluid->press_p[j][i] - fluid->press_p[j][i - 1])) / (dx * dx) +
	((fluid->press_p[j + 1][i] - fluid->press_p[j][i]) -
	(fluid->press_p[j][i] - fluid->press_p[j - 1][i])) / (dy * dy) + fluid->rhs[j][i];
	fluid->eps += r_it * r_it;
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


void	ft_residual_pressure_boundary(t_fluid *fluid, int j, int i)
{
	int ewns[4];
	REAL r_it;

	ft_check_params(fluid, ewns, i, j);
	r_it =
	(ewns[0] * (fluid->press_p[j][i + 1] - fluid->press_p[j][i]) -
	ewns[1] * (fluid->press_p[j][i] - fluid->press_p[j][i - 1])) / (dx * dx) +
	(ewns[2] * (fluid->press_p[j + 1][i] - fluid->press_p[j][i]) -
	ewns[3] * (fluid->press_p[j][i] - fluid->press_p[j - 1][i])) / (dy * dy) + fluid->rhs[j][i];
	fluid->eps += r_it * r_it;
}


REAL	ft_residual_pressure3(t_fluid *fluid)
{
	t_iter i;
	t_iter j;

	fluid->eps = 0;
	ft_fill_iterations(&i, 2, fluid->imax - 1);
	ft_fill_iterations(&j, 2, fluid->jmax - 1);
	ft_iteration(fluid, &ft_residual_pressure2, &j, &i);
	ft_fill_iterations(&j, 1, fluid->jmax);
	ft_iteration_j(fluid, &ft_residual_pressure_boundary, &j, 0);
	ft_iteration_j(fluid, &ft_residual_pressure_boundary, &j, fluid->imax);
	ft_fill_iterations(&i, 1, fluid->imax);
	ft_iteration_i(fluid, &ft_residual_pressure_boundary, 0, &i);
	ft_iteration_i(fluid, &ft_residual_pressure_boundary, fluid->jmax, &i);
	fluid->eps = fluid->eps / fluid->jmax / fluid->imax;
	return (fluid->eps);
}



void	ft_boundary_aproximation_press(t_fluid *fluid, int j, int i)
{
	int ewns[4];

	ft_check_params(fluid, ewns, i, j);
	fluid->tmp[j][i] = (1 - W_CONST) * fluid->press_p[j][i] +
	W_CONST / ((ewns[0] + ewns[1]) / (dx * dx) +
	(ewns[2] + ewns[3]) / (dy * dy)) *
	((ewns[0] * fluid->press_p[j][i + 1] + ewns[1] *
	fluid->tmp[j][i - 1])  / (dx * dx) + (ewns[2] * fluid->press_p[j + 1][i] +
	ewns[3] * fluid->tmp[j - 1][i]) / (dy * dy) -
	fluid->rhs[j][i]);
}

void	ft_aproximation_press(t_fluid *fluid, int j, int i)
{
	fluid->tmp[j][i] = (1 - W_CONST) * fluid->press_p[j][i] +
	W_CONST / ((2) / (dx * dx) + (2) / (dy * dy)) *
	((fluid->press_p[j][i + 1] + fluid->tmp[j][i - 1])  / (dx * dx) +
	(fluid->press_p[j + 1][i] + fluid->tmp[j - 1][i]) / (dy * dy) -
	fluid->rhs[j][i]);
}



REAL	**ft_new_pressure(t_fluid *fluid)
{
	t_iter i;
	t_iter j;

///////////////
//надо заполнить p0i, pi0, p0imax, pjmax0
///////////
	ft_fill_iterations(&i, 2, fluid->imax - 1);
	ft_fill_iterations(&j, 2, fluid->jmax - 1);
	ft_iteration(fluid, &ft_aproximation_press, &j, &i);
	ft_fill_iterations(&j, 1, fluid->jmax);
	ft_iteration_j(fluid, &ft_boundary_aproximation_press, &j, 0);
	ft_iteration_j(fluid, &ft_boundary_aproximation_press, &j, fluid->imax);
	ft_fill_iterations(&i, 1, fluid->imax);
	ft_iteration_i(fluid, &ft_boundary_aproximation_press, 0, &i);
	ft_iteration_i(fluid, &ft_boundary_aproximation_press, fluid->jmax, &i);
	return (fluid->tmp);
}

void	ft_successive_overrelaxation(t_fluid *fluid)
{
	int it;
	REAL **new;

	it = 0;
	fluid->eps = TOLERANCE;
	while (it < MAX_ITERATIONS && fluid->eps < TOLERANCE)
	{
		//вычисляем давление приблизительно
		new = ft_new_pressure(fluid);
		fluid->tmp = fluid->press_p;
		fluid->press_p = new;
		//смотрим разброс и повторяем расчет, в случае необходимости
		fluid->eps = ft_residual_pressure3(fluid);
		it++;
	}
}


void	ft_solver(t_fluid *fluid)
{
	t_iter i;
	t_iter j;

	//вычисляем потоки и двойные дифференциалы для всех внутренних клеток
	ft_fill_iterations(&i, 1, fluid->imax - 1);
	ft_fill_iterations(&j, 1, fluid->jmax - 1);
	ft_iteration(fluid, &ft_flow_f_and_flow_g, &j, &i);
	//вычисляем потоки и двойные дифференциалы для граничных клеток
	ft_fill_iterations(&i, 1, fluid->imax);
	ft_iteration_i(fluid, &ft_flow_f_or_flow_g, fluid->jmax, &i);
	ft_fill_iterations(&j, 1, fluid->jmax);
	ft_iteration_j(fluid, &ft_flow_f_or_flow_g, &j, fluid->imax);
	ft_iteration(fluid, &ft_right_hand_side, &j, &i);
	ft_successive_overrelaxation(fluid);
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

int		**ft_create_map(void)
{
	int **map;
	int i;
	int j;

	map = (int **)ft_mem_arr_new(jmax + 2, imax + 2, sizeof(int));
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



int		**ft_create_flags(int **map)
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
			if (j > 1 && map[j - 1][i])
				flags[j][i] += B_S;
			if (j < jmax && map[j + 1][i])
				flags[j][i] += B_N;
			if (i > 1 && map[j][i - 1])
				flags[j][i] += B_W;
			if (i < imax && map[j][i + 1])
				flags[j][i] += B_E;
			i++;
		}
		j++;
	}
	return (flags);
}



t_fluid	*ft_initialization(int **map, int **flags, int imax, int jmax)
{
	t_fluid *fluid;

	fluid = (t_fluid *)ft_memalloc(sizeof(t_fluid));
	fluid->imax = imax;
	fluid->jmax = jmax;
	fluid->map = map;
	fluid->flags = flags;
	fluid->tmp = (REAL **)ft_mem_arr_new(jmax + 2, imax + 2, sizeof(REAL));
	fluid->speed_v = (REAL **)ft_mem_arr_new(jmax + 2, imax + 2, sizeof(REAL));
	fluid->speed_u = (REAL **)ft_mem_arr_new(jmax + 2, imax + 2, sizeof(REAL));
	fluid->press_p = (REAL **)ft_mem_arr_new(jmax + 2, imax + 2, sizeof(REAL));
	fluid->d2u_dx2_d2v_dy2 = (REAL **)ft_mem_arr_new(jmax + 2, imax + 2, sizeof(REAL));
	fluid->flow_f = (REAL **)ft_mem_arr_new(jmax + 2, imax + 2, sizeof(REAL));
	fluid->flow_g = (REAL **)ft_mem_arr_new(jmax + 2, imax + 2, sizeof(REAL));
	fluid->rhs = (REAL **)ft_mem_arr_new(jmax + 2, imax + 2, sizeof(REAL));
	return (fluid);
}




int main(int argc, char **argv)
{

	int **map;
	int **flags;



	map = ft_create_map();
	flags = ft_create_flags(map);

	t_fluid *fluid;

	fluid = ft_initialization(map, flags, imax, jmax);

/*
**	инициальзация массивов для скоростей и давления
*/



	//ft_print_flags(fluid, fluid->flags);
	//ft_print_fluid(fluid, fluid->speed_u);





	REAL t;
	REAL max_u;
	REAL max_v;

	max_u = U_CONST * 5;
	max_v = U_CONST * 5;
	t = 0.0;
	//ставим одинаковое давление и скорость в клетках воды
	ft_inisialization(fluid);

	//while (t < T_END)
	//{

		ft_fill_watercell(fluid);
		fluid->deltat = ft_time_control(dx, dy, max_u, max_v);
		ft_solver(fluid);
		//ft_differential(fluid, fluid->deltat);
		//ft_right_hand_side(fluid, fluid->deltat);


		// set boundary values for u and v
		t = t + fluid->deltat;
	//}

	return (0);
}
