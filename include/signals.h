#ifndef SIGNALS_H
# define SIGNALS_H

extern volatile sig_atomic_t	g_signal_status;

void	handle_sigint(int sig);

#endif
