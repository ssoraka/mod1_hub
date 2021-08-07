/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/17 10:38:09 by ssoraka           #+#    #+#             */
/*   Updated: 2019/10/17 10:38:10 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_mod1.h"

t_real	ft_return_heigth(t_real value)
{
	value = value * (JMAX - J0) * HEIGTH_KOEF / ((JMAX + J0) * 100) + 1;
	return (value);
}

char	*ft_read_string_from_file(char *name)
{
	int		fd;
	char	*str;
	char	*tmp;

	str = NULL;
	fd = open(name, O_RDWR);
	if (get_next_line(fd, &str) == -1)
	{
		free(str);
		close(fd);
		return (NULL);
	}
	tmp = NULL;
	while (get_next_line(fd, &tmp) > 0)
	{
		str = ft_strjoin_free(str, tmp, TRUE, TRUE);
		tmp = NULL;
	}
	close(fd);
	return (str);
}

void	ft_fill_map_arr(t_map *map)
{
	int	i;
	int	k;

	k = 0;
	while (k <= KMAX + 1)
	{
		i = 0;
		while (i <= IMAX + 1)
		{
			if (map->p_max.y != map->p_min.y)
				map->arr[k][i] = -map->p_min.y * ((JMAX + J0) * 100)
					/ (map->p_max.y - map->p_min.y);
			if (k > K0 && k < KMAX && i > I0 && i < IMAX)
				ft_superposition(map, i, k);
			i++;
		}
		k++;
	}
}

int	**ft_create_map(t_map *map)
{
	map->arr = NULL;
	ft_create_first_and_last_points(map);
	map->arr = (int **)ft_mem_arr_new(KMAX + 2, IMAX + 2, sizeof(int));
	if (!map->arr)
		return (NULL);
	ft_fill_map_arr(map);
	return (map->arr);
}

int	**ft_read_ground_from_file(char *name)
{
	char	*str;
	char	**arr;
	int		**ground;
	t_map	map;

	ground = NULL;
	str = NULL;
	arr = NULL;
	ft_bzero((void *)&map, sizeof(t_map));
	ft_fill_point(&(map.p_min), 0, 0x7FFFFFFF, 0x7FFFFFFF);
	str = ft_read_string_from_file(name);
	if (str && ft_find_points(&map, str))
		ground = ft_create_map(&map);
	free(str);
	ft_mem_arr_free((void ***)&arr);
	return (ground);
}
