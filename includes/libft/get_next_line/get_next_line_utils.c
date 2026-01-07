/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kedemiro <kedemiro@student.42istanbul.com. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 11:35:44 by kedemiro          #+#    #+#             */
/*   Updated: 2025/07/21 11:37:30 by kedemiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

int	line_len(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	while (str[i])
		i++;
	return (i);
}

void	*c_alloc(size_t nmemb, size_t size)
{
	void			*tmp;
	size_t			i;

	i = 0;
	tmp = malloc(nmemb * size);
	if (!tmp)
		return (NULL);
	while (i < nmemb * size)
	{
		((unsigned char *)tmp)[i] = 0;
		i++;
	}
	return (tmp);
}

char	*ft_range(char *storage, int start, int end, int flag)
{
	char	*range;
	int		i;

	i = 0;
	range = c_alloc(end - start + 2, sizeof(char));
	if (!range)
		return (NULL);
	while (start <= end)
	{
		range[i] = storage[start];
		start++;
		i++;
	}
	range[i] = '\0';
	if (flag)
		free(storage);
	return (range);
}

char	*str_dup(char *str)
{
	char	*dup;
	int		size;
	int		i;

	i = 0;
	if (!str)
		size = 0;
	else
		size = line_len(str);
	dup = c_alloc(size + 1, sizeof(char));
	if (!dup)
		return (NULL);
	while (str[i])
	{
		dup[i] = str[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}

char	*str_join(char *storage, char *buffer)
{
	char	*join;
	int		i;
	int		j;

	i = 0;
	j = 0;
	if (!storage || !buffer)
		return (str_dup(""));
	join = c_alloc(line_len(storage) + line_len(buffer) + 1, sizeof(char));
	if (!join)
		return (NULL);
	while (storage[i])
	{
		join[i] = storage[i];
		i++;
	}
	while (buffer[j])
	{
		join[i + j] = buffer[j];
		j++;
	}
	join[i + j] = '\0';
	free(storage);
	return (join);
}
