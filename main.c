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

typedef struct		s_fluid
{
	int				imax;
	int				jmax;
	mytype			dx;
	mytype			dy;
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


void	ft_fill_watercell(t_fluid *fluid)
{
	int i;
	int j;

	j = 1;
	while (j <= fluid->jmax)
	{
		i = 1;
		while (i <= fluid->imax)
		{
			if ((fluid->map)[j][i])
			{
				fluid->press_p[j][i] = P_CONST;
				fluid->speed_u[j][i] = U_CONST;
				fluid->speed_v[j][i] = U_CONST;
			}
			i++;
		}
		j++;
	}
}

void	ft_differential(t_fluid *fluid, REAL deltat)
{
	int i;
	int j;

	j = 1;
	while (j <= fluid->jmax)
	{
		i = 1;
		while (i <= fluid->imax)
		{// граничные значения проставлены в ft_fill_boundary
			//сумма двойных дифференциалов
			fluid->d2u_dx2_d2v_dy2[j][i] = (fluid->speed_u[j][i + 1] -
			2 * fluid->speed_u[j][i] + fluid->speed_u[j][i - 1]) / (dx * dx) +
			(fluid->speed_v[j + 1][i] -	2 * fluid->speed_v[j][i] +
			fluid->speed_v[j - 1][i]) / (dy * dy);
			//потоки горизонтальный и вертикальный
			if (i < fluid->imax)
				fluid->flow_f[j][i] = fluid->speed_u[j][i] +
				deltat / CONST_RE * (fluid->d2u_dx2_d2v_dy2[j][i]);
			if (j < fluid->jmax)
				fluid->flow_g[j][i] = fluid->speed_v[j][i] +
				deltat / CONST_RE * (fluid->d2u_dx2_d2v_dy2[j][i]);
			i++;
		}
		j++;
	}
}


void	ft_right_hand_side(t_fluid *fluid, REAL deltat)
{
	int i;
	int j;

	j = 1;
	while (j <= fluid->jmax)
	{
		i = 1;
		while (i <= fluid->imax)
		{// граничные значения проставлены в ft_fill_boundary
			fluid->rhs[j][i] = ((fluid->flow_f[j][i] - fluid->flow_f[j][i - 1]) / dx +
			(fluid->flow_g[j][i] - fluid->flow_g[j - 1][i]) / dy) / deltat;

			i++;
		}
		j++;
	}
}

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



/*
**	вроде нормально, но это ппц
**	есть вариант выбирать максимальную разницу, это проще, чем сумма квадратов
*/
REAL ft_residual_pressure(t_fluid *fluid)
{
	int i;
	int j;
	int ewns[4];
	REAL r_it;
	REAL eps;

	eps = 0.0;
	j = 1;
	while (j <= fluid->jmax)
	{
		i = 1;
		while (i <= fluid->imax)
		{
			ft_check_params(fluid, ewns, i, j);
			r_it = (ewns[0] * (fluid->press_p[j][i + 1] - fluid->press_p[j][i]) -
			ewns[1] * (fluid->press_p[j][i] - fluid->press_p[j][i - 1])) / (dx * dx) +
			(ewns[2] * (fluid->press_p[j + 1][i] - fluid->press_p[j][i]) -
			ewns[3] * (fluid->press_p[j][i] - fluid->press_p[j - 1][i])) / (dy * dy) + fluid->rhs[j][i];
			eps += r_it * r_it;
			i++;
		}
		j++;
	}
	eps = eps / fluid->jmax / fluid->imax;
	return (eps);
}


void	ft_successive_overrelaxation(t_fluid *fluid)
{
	int i;
	int j;
	int it;
	int ewns[4];
	REAL eps;

	//надо заполнить p0i, pi0, p0imax, pjmax0
	//и свапнуть tmp и press_p

	eps = TOLERANCE;
	it = 0;
	while (it < MAX_ITERATIONS && eps < TOLERANCE)
	{
		j = 1;
		while (j <= fluid->jmax)
		{
			i = 1;
			while (i <= fluid->imax)
			{// граничные значения проставлены в ft_fill_boundary
				ft_check_params(fluid, ewns, i, j);
				fluid->press_p[j][i] = (1 - W_CONST) * fluid->tmp[j][i] + W_CONST / ((ewns[0] + ewns[1]) / (dx * dx) + (ewns[2] + ewns[3]) / (dy * dy)) *
				((ewns[0] * fluid->tmp[j][i + 1] + ewns[1] * fluid->press_p[j][i - 1])  / (dx * dx) + (ewns[2] * fluid->tmp[j + 1][i] + ewns[3] * fluid->press_p[j - 1][i]) / (dy * dy) - fluid->rhs[j][i]);
				i++;
			}
			j++;
		}
		eps = ft_residual_pressure(fluid);
		it++;
	}
}



void	ft_fill_boundary(t_fluid *fluid)
{
	int i;
	int j;

	i = 0;//граничные условия на горизонтальных стенках
	while (i <= fluid->imax + 1)
	{
		fluid->press_p[0][i] = fluid->press_p[1][i];
		fluid->press_p[jmax + 1][i] = fluid->press_p[jmax][i];
		fluid->speed_u[0][i] = -fluid->speed_u[1][i];
		fluid->speed_u[jmax + 1][i] = -fluid->speed_u[jmax][i];
		fluid->speed_v[0][i] = 0;
		fluid->speed_v[jmax][i] = 0;
		fluid->flow_g[0][i] = fluid->speed_v[0][i];
		fluid->flow_g[jmax][i] = fluid->speed_v[jmax][i];
		i++;
	}
	j = 0;//граничные условия на вертикальных стенках
	while (j <= fluid->jmax + 1)
	{
		fluid->press_p[j][0] = fluid->press_p[j][1];
		fluid->press_p[j][imax + 1] = fluid->press_p[j][imax];
		fluid->speed_u[j][0] = 0;
		fluid->speed_u[j][imax] = 0;
		fluid->speed_v[j][0] = -fluid->speed_v[j][1];
		fluid->speed_v[j][imax + 1] = -fluid->speed_v[j][imax];
		fluid->flow_f[j][0] = fluid->speed_u[j][0];
		fluid->flow_f[j][imax] = fluid->speed_u[j][imax];
		j++;
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


void	ft_fill_obstacle(t_fluid *fluid)
{
	int i;
	int j;

	j = 1;
	while (j <= fluid->jmax)
	{
		i = 1;
		while (i <= fluid->imax)
		{//если это препятствие и рядом есть вода
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
			i++;
		}
		j++;
	}
}


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
	REAL deltat;
	REAL max_u;
	REAL max_v;

	max_u = U_CONST * 5;
	max_v = U_CONST * 5;
	t = 0.0;
	//ставим одинаковое давление и скорость в клетках воды
	ft_fill_watercell(fluid);
	//while (t < T_END)
	//{
		//ставим давление и скорость в граничных клетках воды
		ft_fill_obstacle(fluid);
		ft_fill_boundary(fluid);

		deltat = ft_time_control(dx, dy, max_u, max_v);
		ft_differential(fluid, deltat);
		ft_right_hand_side(fluid, deltat);
		ft_successive_overrelaxation(fluid);

		// set boundary values for u and v
		t = t + deltat;
	//}

	return (0);
}
