#ifndef SH_SIGNALS_H
# define SH_SIGNALS_H

void	setup_signal(int mode);
void	signal_in_wait(int sig);
void	signal_in_main(int sig);
void	term_pacifier(void);
void setup_signal_heredoc(void);	

#endif