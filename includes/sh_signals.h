/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sh_signals.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahmmous <mahmmous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/01 00:00:00 by mahmmous          #+#    #+#             */
/*   Updated: 2026/06/01 00:00:00 by mahmmous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef SH_SIGNALS_H
# define SH_SIGNALS_H

typedef struct s_sh		t_sh;

void	setup_signal(int mode);
void	signal_in_wait(int sig);
void	signal_in_main(int sig);
void	term_pacifier(void);
// void	setup_signal_heredoc(void);

#endif