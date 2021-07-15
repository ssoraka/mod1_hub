/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_program.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/16 18:33:15 by ssoraka           #+#    #+#             */
/*   Updated: 2020/02/16 18:33:15 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_mod1.h"

int		is_read_programm(char *buffer, char *filename)
{
	int fd;
	int ret;

	if ((fd = open(filename, O_RDONLY)) == -1)
		return (FALSE);
	if (read(fd, NULL, 0) == -1)
		return (FALSE);
	ret = read(fd, buffer, PROGRAMM_SIZE);
	buffer[ret] = '\0';
	close(fd);
	return (TRUE);
}
