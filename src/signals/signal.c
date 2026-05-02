
#include "../../includes/WCMS.h"


volatile sig_atomic_t g_sig = 0;

void	signal_in_main(int sig)
{
	if (sig == SIGINT)
	{
		g_sig = SIGINT;
		rl_replace_line("", 0);
		rl_redisplay();
		write(1, "^C\n", 3);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		g_sig = SIGINT;
	}
	else if (sig == SIGQUIT)
	{
		rl_replace_line("", 0);
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
		write(1, "^\\Quit (core dumped)\n", 20);
		write(1, "\n", 1);
		g_sig = SIGQUIT;
	}
}

void	setup_signal(t_sh *sh, int mode)
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
	if (g_sig == SIGINT)
	{
		sh->exit_status = 128 + SIGINT;
		g_sig = 0;
	}
	else if (g_sig == SIGQUIT)
	{
		sh->exit_status = 128 + SIGQUIT;
		g_sig = 0;
	}
}

void	term_pacifier(void)
{
	struct termios term;

	tcgetattr(STDIN_FILENO, &term);
	term.c_lflag &= ~ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
}