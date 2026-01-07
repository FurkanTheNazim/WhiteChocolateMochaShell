/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kedemiro <kedemiro@student.42istanbul.com. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 11:35:41 by kedemiro          #+#    #+#             */
/*   Updated: 2025/07/21 21:04:32 by kedemiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static int	find_newline(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (-1);
	while (str[i])
	{
		if (str[i] == '\n')
			return (i);
		i++;
	}
	return (-1);
}

static char	*read_file(int fd, char *storage)
{
	char	*buffer;
	ssize_t	count;

	count = 1;
	buffer = c_alloc(BUFFER_SIZE + 1, sizeof(char));
	if (!buffer)
		return (NULL);
	if (!storage)
		storage = str_dup("");
	while (find_newline(storage) < 0 && count > 0)
	{
		count = read(fd, buffer, BUFFER_SIZE);
		if (count == -1)
		{
			free (buffer);
			free (storage);
			return (NULL);
		}
		buffer[count] = '\0';
		storage = str_join(storage, buffer);
	}
	free(buffer);
	return (storage);
}

char	*get_next_line(int fd)
{
	static char	*storage;
	char		*line;
	int			index;

	if (BUFFER_SIZE <= 0 || fd < 0)
		return (NULL);
	storage = read_file(fd, storage);
	if (!storage)
		return (NULL);
	index = find_newline(storage);
	if (index == -1)
	{
		line = storage;
		storage = NULL;
		if (*line == '\0')
			return (free(line), NULL);
		return (line);
	}
	else
	{
		line = ft_range(storage, 0, index, 0);
		storage = ft_range(storage, index + 1, line_len(storage), 1);
		return (line);
	}
}
