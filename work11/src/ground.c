/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ground.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/18 20:37:56 by ssoraka           #+#    #+#             */
/*   Updated: 2019/10/18 20:37:57 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_mod1.h"

typedef struct s_ground
{
	int				**complex;
	int				**simple;
}					t_ground;

int	ft_cube_interpolate(int *arr, t_real x)
{
	t_real	value;

	value = arr[1] + (-arr[0] + arr[2]) * (x / 2)
		+ (2 * arr[0] - 5 * arr[1] + 4 * arr[2] - arr[3]) * x * (x / 2)
		+ (-arr[0] + 3 * arr[1] - 3 * arr[2] + arr[3]) * x * x * (x / 2);
	return (value);
}

void	ft_bicube_interpolate_x2(void *param, int j, int i, int k)
{
	t_ground	*gr;
	int			n;
	int			tmp[4];

	gr = (t_ground *)param;
	gr->complex[k * (1 + ADD_POINT)][i * (1 + ADD_POINT)] = gr->simple[k][i];
	n = 1;
	while (n < ADD_POINT + 1)
	{
		if (i < IMAX)
			gr->complex[k * (1 + ADD_POINT)][i * (1 + ADD_POINT) + n]
				= ft_cube_interpolate(gr->simple[k] + i - 1,
					((t_real)n / (1 + ADD_POINT)));
		else
		{
			tmp[0] = gr->simple[k][i - 1];
			tmp[1] = gr->simple[k][i];
			tmp[2] = gr->simple[k][i + 1];
			tmp[3] = gr->simple[k][i + 1];
			gr->complex[k * (1 + ADD_POINT)][i * (1 + ADD_POINT) + n]
				= ft_cube_interpolate(tmp, ((t_real)n / (1 + ADD_POINT)));
		}
		n++;
	}
}

void	ft_bicube_interpolate_x(void *param, int j, int i, int k)
{
	t_ground	*gr;
	int			n;
	int			tmp[4];
	int			value;

	gr = (t_ground *)param;
	gr->complex[k * (1 + ADD_POINT)][i * (1 + ADD_POINT)] = gr->simple[k][i];
	if (i > IMAX)
		return ;
	n = 1;
	while (n < ADD_POINT + 1)
	{
		value = gr->simple[K0][I0];
		tmp[0] = gr->simple[k][i - 1 * (i != 0)];
		tmp[1] = gr->simple[k][i];
		tmp[2] = gr->simple[k][i + 1];
		tmp[3] = gr->simple[k][i + 1 + 1 * (i != IMAX)];
		if (i > 0 && i < IMAX)
			value = ft_cube_interpolate(tmp, ((t_real)n / (1 + ADD_POINT)));
		gr->complex[k * (1 + ADD_POINT)][i * (1 + ADD_POINT) + n] = value;
		n++;
	}
}

void	ft_bicube_interpolate_y(void *param, int j, int i, int k)
{
	t_ground	*ground;
	int			n;
	int			tmp[4];
	int			value;

	ground = (t_ground *)param;
	tmp[1] = ground->complex[k * (1 + ADD_POINT)][i];
	tmp[2] = ground->complex[(k + 1) *(1 + ADD_POINT)][i];
	tmp[0] = tmp[1];
	tmp[3] = tmp[2];
	if (k != 0)
		tmp[0] = ground->complex[(k - 1) *(1 + ADD_POINT)][i];
	if (k != KMAX)
		tmp[3] = ground->complex[(k + 2) *(1 + ADD_POINT)][i];
	n = 1;
	while (n < ADD_POINT + 1)
	{
		value = ground->simple[K0][I0];
		if (k > 0 && k < KMAX)
			value = ft_cube_interpolate(tmp, ((t_real)n / (1 + ADD_POINT)));
		ground->complex[k * (1 + ADD_POINT) + n][i] = value;
		n++;
	}
}

int	**ft_create_complex_ground_from_simple(int **simple_ground)
{
	t_point		start;
	t_point		end;
	t_ground	ground;

	ground.simple = simple_ground;
	ground.complex = (int **)ft_mem_arr_new((KMAX + 2) * (1 + ADD_POINT),
			(IMAX + 2) * (1 + ADD_POINT), sizeof(int));
	if (!ground.complex)
		return (NULL);
	ft_fill_point(&start, 0, 0, 0);
	ft_fill_point(&end, 0, IMAX + 1, KMAX + 1);
	ft_cycle_cube((void *)(&ground), &ft_bicube_interpolate_x, &start, &end);
	ft_fill_point(&start, 0, I0, 0);
	ft_fill_point(&end, 0, (I0 + ADD_POINT) * (IMAX + 1), KMAX);
	ft_cycle_cube((void *)(&ground), &ft_bicube_interpolate_y, &start, &end);
	return (ground.complex);
}
