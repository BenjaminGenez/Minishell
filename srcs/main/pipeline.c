#include "minishell.h"

void	setup_pipe_child(int prev_fd, int pipe_fd[2], t_token *current)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (prev_fd != -1)
	{
		dup2(prev_fd, STDIN_FILENO);
		close(prev_fd);
	}
	if (current->next != NULL)
	{
		dup2(pipe_fd[1], STDOUT_FILENO);
		close(pipe_fd[0]);
		close(pipe_fd[1]);
	}
}

static void	update_pipe_fds(int *prev_fd, int *pipe_fd, t_token **cmd_start,
		t_token *current)
{
	if (*prev_fd != -1)
		close(*prev_fd);
	if (current->next != NULL)
	{
		close(pipe_fd[1]);
		*prev_fd = pipe_fd[0];
		*cmd_start = current->next;
	}
}

pid_t	process_pipe_segment(t_mini *shell, t_token **cmd_start,
		t_token *current, int *prev_fd)
{
	int		pipe_fd[2];
	pid_t	pid;

	pid = -1;
	if (current->next != NULL && pipe(pipe_fd) == -1)
	{
		perror("minishell: pipe");
		return (-1);
	}
	pid = fork();
	if (pid == 0)
	{
		setup_pipe_child(*prev_fd, pipe_fd, current);
		exec_cmd(shell, *cmd_start);
		exit(shell->ret);
	}
	update_pipe_fds(prev_fd, pipe_fd, cmd_start, current);
	return (pid);
}

static void	wait_all_children(int prev_fd)
{
	if (prev_fd != -1)
		close(prev_fd);
	while (wait(NULL) > 0)
		;
	g_sig.pid = 0;
}

static pid_t	process_pipeline_loop(t_mini *shell, int *prev_fd)
{
	t_token	*cmd_start;
	t_token	*current;
	pid_t	last_pid;

	last_pid = -1;
	cmd_start = shell->start;
	current = shell->start;
	while (current)
	{
		if (current->type == PIPE || current->next == NULL)
		{
			last_pid = process_pipe_segment(shell, &cmd_start, current,
					prev_fd);
			if (last_pid == -1)
				return (-1);
			if (current->next == NULL)
				g_sig.pid = last_pid;
		}
		current = current->next;
	}
	return (last_pid);
}

void	exec_pipeline(t_mini *shell)
{
	int		prev_fd;
	pid_t	last_pid;

	prev_fd = -1;
	last_pid = process_pipeline_loop(shell, &prev_fd);
	if (last_pid == -1)
		return ;
	wait_all_children(prev_fd);
}
