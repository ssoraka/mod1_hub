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


char		str[10][10] =
{
	{'1', '1', '1', '1', '0', '1', '1', '1', '1', '1'},
	{'1', '1', '1', '1', '0', '1', '1', '1', '1', '1'},
	{'1', '1', '1', '1', '0', '1', '1', '0', '1', '1'},
	{'1', '1', '1', '1', '0', '1', '1', '0', '1', '1'},
	{'1', '1', '1', '1', '0', '1', '1', '0', '1', '1'},
	{'1', '1', '1', '1', '0', '1', '1', '0', '1', '1'},
	{'1', '1', '1', '1', '0', '1', '1', '0', '1', '1'},
	{'1', '1', '1', '1', '0', '1', '1', '0', '1', '1'},
	{'1', '1', '1', '1', '1', '1', '1', '0', '1', '1'},
	{'1', '1', '1', '1', '1', '1', '1', '0', '1', '1'}
};



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




int main(int argc, char **argv)
{

	char **map;



	map = ft_create_map(10, 10);


	t_fluid *fluid;
	fluid = ft_initialization(map, 10, 10);


	ft_print_flags(fluid, fluid->flags);
	ft_print_fluid(fluid, fluid->speed_u);
	ft_print_fluid(fluid, fluid->press_p);

	REAL t;


	t = 0.0;



	while (t < 10)
	{

		ft_solver(fluid);
		//ft_print_fluid(fluid, fluid->speed_u);
		ft_print_fluid(fluid, fluid->speed_u);
		ft_print_fluid(fluid, fluid->speed_v);
		t = t + fluid->deltat;
	}

	ft_print_fluid(fluid, fluid->speed_u);
	return (0);
}
