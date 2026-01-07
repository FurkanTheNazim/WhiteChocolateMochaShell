/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kedemiro <kedemiro@student.42istanbul.com. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 11:37:43 by kedemiro          #+#    #+#             */
/*   Updated: 2025/07/21 11:38:16 by kedemiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 24
# endif

# include <stdlib.h>
# include <unistd.h>

char	*ft_range(char *storage, int start, int end, int flag);
char	*str_join(char *storage, char *buffer);
void	*c_alloc(size_t nmemb, size_t size);
char	*get_next_line(int fd);
char	*str_dup(char *str);
int		line_len(char *str);

#endif