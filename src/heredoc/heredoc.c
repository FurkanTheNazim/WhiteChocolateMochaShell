/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahmmous <mahmmous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/12 16:35:12 by mahmmous          #+#    #+#             */
/*   Updated: 2026/03/12 17:19:04 by mahmmous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WCMS.h"

int read_heredoc(t_sh *sh, t_redir *redir)
{
    int pipe_fd[2];
    char *line;
    
    if (pipe(pipe_fd) < 0) // Pipe'ı oluştur
        return (-1);
    while (1)
    {
        line = readline("> "); // kullanıcıdan satır al
        if (!line)
            break ;
        if (ft_strncmp(line, redir->file, ft_strlen(redir->file) + 1) == 0)
        {
            free(line);
            break ;
        }
        write(pipe_fd[1], line, ft_strlen(line));
        write(pipe_fd[1], "\n", 1);
        free(line);
    }
    close(pipe_fd[1]);
    redir->heredoc_fd = pipe_fd[0];
    return (0);
}