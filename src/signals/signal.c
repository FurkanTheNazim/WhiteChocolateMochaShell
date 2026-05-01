#define _XOPEN_SOURCE 700

#include "../../includes/WCMS.h"

volatile sig_atomic_t g_sig = 0;


// void loop()
// {
// 	while (1)
// 	{
// 		printf("loop\n");
// 		sleep(1); 
// 	}
// }

void	signal_in_main(int sig)
{
	if (sig == SIGINT)
	{
		g_sig = SIGINT;
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

// void	signal_in_child(int sig)
// {
// 	if (sig == SIGINT || sig == SIGQUIT)
// 		write(1, "\n", 1);
// }

void	signal_in_wait(int sig)
{
		if (sig == SIGINT || sig == SIGQUIT)
			write(1, "\n", 1);
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

// int	main()
// {
// 	char	*input;
// 	pid_t pid;
// 	struct sigaction sa;
// 	while (1)
// 	{
// 		setup_signal(1);
// 		signal(SIGQUIT, SIG_IGN);
// 		if (g_sig != 0)
// 			g_sig = 0;
// 		input = readline("input>");
// 		if (!input)
// 			exit(0);
// 		setup_signal(2);
// 		pid = fork();
// 		if (pid == 0)
// 		{
// 			signal(SIGQUIT, SIG_DFL);
// 			signal(SIGINT, SIG_DFL);
// 			loop();
// 		}
// 		else
// 		{
// 			wait(&pid);
// 			printf("proces öldü\n");
// 		}

// 	}
// }