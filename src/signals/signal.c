/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahmmous <mahmmous@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/07 03:34:57 by kedemiro          #+#    #+#             */
/*   Updated: 2026/06/03 18:43:10 by mahmmous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/WCMS.h"

volatile sig_atomic_t	g_sig = 0;

void	signal_in_main(int sig)
{
	if (sig == SIGINT)
	{
		write(1, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
		g_sig = SIGINT;
	}
	else if (sig == SIGQUIT)
	{
		rl_redisplay();
		g_sig = SIGQUIT;
	}
}

void	signal_in_wait(int sig)
{
	if (sig == SIGINT)
	{
		write(1, "^C\n", 3);
		g_sig = SIGINT;
	}
	else if (sig == SIGQUIT)
	{
		write(2, "^\\Quit (core dumped)\n\n", 21);
		g_sig = SIGQUIT;
	}
}

static void	signal_in_heredoc(int sig)
{
	if (sig == SIGINT)
	{
		rl_redisplay();
		write(1, "^C\n", 4);
		g_sig = SIGINT;
	}
}

void	setup_signal(int mode)
{
	struct sigaction	sa;

	ft_bzero(&sa, sizeof(sa));
	if (mode == 1)
		sa.sa_handler = signal_in_main;
	else if (mode == 2)
		sa.sa_handler = signal_in_heredoc;
	else
		sa.sa_handler = signal_in_wait;
	sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
}

void	term_pacifier(void)
{
	struct termios	term;

	tcgetattr(STDIN_FILENO, &term);
	term.c_lflag &= ~ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
}
