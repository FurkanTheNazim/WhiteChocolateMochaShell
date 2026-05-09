/* ************************************************************************** */
/*                                                                            */
/*                                                          :::      :::::::: */
/*   signal.c                                             :+:      :+:    :+: */
/*                                                      +:+ +:+         +:+   */
/*   By: kedemiro <kedemiro@student.42istanbul.com.tr +#+  +:+       +#+      */
/*                                                  +#+#+#+#+#+   +#+         */
/*   Created: 2026/05/07 03:34:57 by kedemiro            #+#    #+#           */
/*   Updated: 2026/05/07 20:35:38 by kedemiro           ###   ########.fr     */
/*                                                                            */
/* ************************************************************************** */


#include "../../includes/WCMS.h"


volatile sig_atomic_t g_sig = 0;

void	signal_in_main(int sig)
{
	if (sig == SIGINT)
	{
		g_sig = SIGINT;
		// rl_replace_line("", 0);
		rl_redisplay();
		write(1, "^C\n", 3);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		g_sig = SIGINT;
	}
	else if (sig == SIGQUIT)
	{
		// rl_replace_line("", 0);
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

void	setup_signal(int mode)
{
	struct sigaction sa;

	ft_bzero(&sa, sizeof(sa));
	if (mode == 1)
		sa.sa_handler = signal_in_main;
	else
		sa.sa_handler = signal_in_wait;
	sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
}

void	term_pacifier(void)
{
	struct termios term;

	tcgetattr(STDIN_FILENO, &term);
	term.c_lflag &= ~ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
}