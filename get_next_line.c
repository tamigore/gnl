/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tamigore <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/21 14:14:16 by tamigore          #+#    #+#             */
/*   Updated: 2019/11/21 19:24:17 by tamigore         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static t_list	*ft_file(t_list **file, int fd)
{
	t_list	*tmp;

	tmp = *file;
	while (tmp)
	{
		if (tmp->fd == fd)
			return (tmp);
		tmp = tmp->next;
	}
	if (!(tmp = (t_list *)malloc(sizeof(t_list))))
		return (NULL);
	if (!(tmp->content = ft_strnew(1)))
		return (NULL);
	tmp->fd = fd;
	tmp->next = *file;
	*file = tmp;
	return (tmp);
}

static char		*ft_strnjoin(char *s1, char *s2, int r)
{
	char	*join;
	int		diff;
	char	*tmp;

	tmp = s1;
	diff = ft_strlen(s2) - r;
	if (!s1 || !s2 || !(join = ft_strnew(ft_strlen(s1) + ft_strlen(s2) - diff)))
		return (NULL);
	ft_strncat(join, s1, ft_strlen(s1));
	ft_strncat(join, s2, r);
	free(tmp);
	return (join);
}

static int		ft_strcncat(char **content, char *str)
{
	int		i;
	int		j;

	i = 0;
	j = 0;
	while (str[i])
	{
		if (str[i] == '\n')
			break ;
		i++;
	}
	if (!(*content = ft_strnew(i + 1)))
		return (-1);
	ft_strncat(*content, str, i);
	return (i);
}

static char		*ft_free(char *content, int r, char *buf)
{
	char	*tmp;

	tmp = content;
	if (!(content = ft_strdup(content + r)))
		return (NULL);
	free(tmp);
	free(buf);
	return (content);
}

int				get_next_line(int fd, char **line)
{
	static t_list	*file;
	t_list			*list;
	int				r;
	int				i;
	char			*buf;

	if (fd < 0 || !(buf = (char*)malloc(sizeof(char) * (BUFFER_SIZE + 1)))
		|| read(fd, buf, 0) == -1 || !(list = ft_file(&file, fd)) || !line)
		return (-1);
	while ((r = read(fd, buf, BUFFER_SIZE)) > 0)
	{
		buf[r] = '\0';
		if (!(list->content = ft_strnjoin(list->content, buf, r)))
			return (-1);
		if (ft_strchr(buf, '\n'))
			break ;
	}
	if ((i = ft_strcncat(line, list->content)) == -1)
		return (-1);
	r = (r < BUFFER_SIZE && !(ft_strchr(list->content, '\n'))) ? 0 : 1;
	if (!(list->content = ft_free(list->content, i + 1, buf)))
		return (-1);
	return (r);
}
