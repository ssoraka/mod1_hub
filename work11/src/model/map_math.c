/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_math.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/17 10:38:09 by ssoraka           #+#    #+#             */
/*   Updated: 2019/10/17 10:38:10 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_mod1.h"
#include <math.h>

/*
** тут надо сделать проверку точек на интовые значения...
*/
int	ft_find_point(t_map *map, char **args)
{
	int	i;

	if (args[0][0] != '(' || args[2][ft_strlen(args[2]) - 1] != ')')
		return (FALSE);
	i = map->count;
	map->p[i].y = ft_atoi(args[2]);
	map->p[i].x = ft_atoi(args[0] + 1);
	map->p[i].z = ft_atoi(args[1]);
	map->p_max.y = ft_max(map->p_max.y, map->p[i].y);
	map->p_max.x = ft_max(map->p_max.x, map->p[i].x);
	map->p_max.z = ft_max(map->p_max.z, map->p[i].z);
	map->p_min.y = ft_min(map->p_min.y, map->p[i].y);
	map->p_min.x = ft_min(map->p_min.x, map->p[i].x);
	map->p_min.z = ft_min(map->p_min.z, map->p[i].z);
	return (TRUE);
}

int	ft_find_points(t_map *map, char **arr)
{
	char	**tmp;
	int		valid;

	map->count = 0;
	while (arr[map->count] && map->count < MAX_POINT)
	{
		valid = FALSE;
		tmp = ft_strsplit(arr[map->count], ',');
		if (tmp && tmp[0] && tmp[1] && tmp[2] && !tmp[3]
			&& ft_find_point(map, tmp))
			valid = TRUE;
		ft_mem_arr_free((void ***)&tmp);
		if (!valid)
			return (FALSE);
		(map->count)++;
	}
	return (TRUE);
}

void	ft_create_first_and_last_points(t_map *map)
{
	int	delta_y;
	int	delta_x;
	int	delta_z;
	int	delta;

	delta_y = (map->p_max.y - map->p_min.y) / ((JMAX + J0) * MAP_KOEF);
	delta_x = (map->p_max.x - map->p_min.x) / ((IMAX + I0) * MAP_KOEF);
	delta_z = (map->p_max.z - map->p_min.z) / ((KMAX + K0) * MAP_KOEF);
	delta = ft_max(delta_x, delta_y);
	delta = ft_max(delta, delta_z);
	delta = ft_max(delta, 1);
	map->delta = delta;
	ft_fill_point(&(map->first),
		((map->p_min.y + map->p_max.y) - delta * (JMAX + J0)) / 2,
		((map->p_min.x + map->p_max.x) - delta * (IMAX + I0)) / 2,
		((map->p_min.z + map->p_max.z) - delta * (KMAX + K0)) / 2);
}

t_real	ft_sigmoida(int i, int k)
{
	t_real	answer;
	int		x;

	x = ft_min(i - I0, IMAX - i);
	x = ft_min(x, KMAX - k);
	x = ft_min(x, k - K0);
	x = -x * x;
	answer = 1 - exp2((t_real)x);
	return (answer);
}

/*
** 	2.0 - отвечает за пологость, 0,8 - защита от касания гор потолка
*/
void	ft_superposition(t_map *map, int i, int k)
{
	double	e;
	double	dist;
	double	all_dist;
	double	sigma;
	int		num;

	e = 0;
	all_dist = 0;
	num = 0;
	sigma = map->delta * map->delta * ft_max(IMAX, KMAX);
	while (num < map->count)
	{
		dist = ft_power(map->p[num].x - (map->first.x + map->delta * i), 2)
			+ ft_power(map->p[num].z - (map->first.z + map->delta * k), 2);
		all_dist += 1.0 / (1.0 + dist);
		e += exp2(-dist / (sigma * 2.0)) * map->p[num].y / (1.0 + dist);
		num++;
	}
	map->arr[k][i] += (int)((e * ft_sigmoida(i, k)) * ((JMAX + J0) * 100)
			/ all_dist / (map->p_max.y - map->p_min.y));
}
