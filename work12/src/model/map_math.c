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

void	ft_update_max_min_value(t_point *max, t_point *min, t_point *p)
{
	max->y = ft_max(max->y, p->y);
	max->x = ft_max(max->x, p->x);
	max->z = ft_max(max->z, p->z);
	min->y = ft_min(min->y, p->y);
	min->x = ft_min(min->x, p->x);
	min->z = ft_min(min->z, p->z);
}

int	ft_find_points(t_map *map, char *str)
{
	int	i;

	i = 0;
	while (*str && i < MAX_POINT)
	{
		if (!ft_get_nums_from_string(&str, map->p + i))
			return (FALSE);
		ft_update_max_min_value(&map->p_max, &map->p_min, map->p + i);
		i++;
	}
	map->count = i;
	if (*str)
		return (FALSE);
	return (TRUE);
}

void	ft_create_first_and_last_points(t_map *map)
{
	t_dpoint	d;
	double		delta;

	d.y = (map->p_max.y - map->p_min.y) / ((JMAX + J0) * MAP_KOEF);
	d.x = (map->p_max.x - map->p_min.x) / ((IMAX + I0) * MAP_KOEF);
	d.z = (map->p_max.z - map->p_min.z) / ((KMAX + K0) * MAP_KOEF);
	delta = (d.x >= d.y) * d.x + (d.x < d.y) * d.y;
	delta = (delta >= d.z) * delta + (delta < d.z) * d.z;
	delta = (delta >= 1.0) * delta + (delta < 1.0) * 1.0;
	map->delta = delta;
	ft_fill_dpoint(&(map->first),
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
		dist = pow(map->p[num].x - (map->first.x + map->delta * i), 2)
			+ pow(map->p[num].z - (map->first.z + map->delta * k), 2);
		all_dist += 1.0 / (1.0 + dist);
		e += exp2(-dist / (sigma * 2.0)) * map->p[num].y / (1.0 + dist);
		num++;
	}
	map->arr[k][i] += (int)((e * ft_sigmoida(i, k)) * ((JMAX + J0) * 100)
			/ all_dist / (map->p_max.y - map->p_min.y));
}
