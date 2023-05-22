/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ssoraka <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/20 10:41:12 by ssoraka           #+#    #+#             */
/*   Updated: 2019/04/20 11:33:44 by ssoraka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	end_line(char **line, char **str)
{
	char	*temp;
	char	*temp2;

	if ((*str)[0] == '\0')
	{
		*line = NULL;
		ft_strdel(str);
		return (0);
	}
	temp2 = ft_strchr(*str, '\n');
	if (temp2 != 0)
	{
		temp = *str;
		temp2[0] = '\0';
		*line = ft_strdup(temp);
		*str = ft_strdup(temp2 + 1);
		free(temp);
		if ((*str)[0] == '\0')
			ft_strdel(str);
		return (1);
	}
	*line = *str;
	*str = NULL;
	return (1);
}

int	get_next_line(const int fd, char **line)
{
	char		buf[BUFF_SIZE + 1];
	static char	*str[10240];
	char		*temp;
	int			ret;

	if (fd < 0 || read(fd, NULL, 0) < 0 || !line || fd > 10240)
		return (-1);
	if (!(str[fd]))
		str[fd] = ft_strnew(0);
	if (ft_strchr(str[fd], '\n') == 0)
	{
		buf[0] = '\0';
		while (!(ft_strchr(buf, '\n')))
		{
			ret = read(fd, buf, BUFF_SIZE);
			if (ret == 0)
				break ;
			buf[ret] = '\0';
			temp = ft_strjoin(str[fd], buf);
			free(str[fd]);
			str[fd] = temp;
		}
	}
	return (end_line(line, &str[fd]));
}
