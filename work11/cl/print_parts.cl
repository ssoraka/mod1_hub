/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kernel.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/25 17:13:40 by ssoraka           #+#    #+#             */
/*   Updated: 2019/08/25 17:13:40 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_cl.h"


__kernel	void print_parts(__global t_part *part, __global t_ipart *ipart)
{
	int gid;


	gid = get_global_id(0);
	printf("%d_%lf_%lf_%lf\n", gid, part[gid].density, part[gid].press, part[gid].pos.x);
	int n;
	n = 0;
	while (n < part[gid].n.count)
	{
		//printf("__%d_%lf\n", gid, part[gid].speed.x);
		n++;
	}

}
