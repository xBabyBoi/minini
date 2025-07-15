/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-qori <yel-qori@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 16:52:00 by rhafidi           #+#    #+#             */
/*   Updated: 2025/07/15 21:28:37 by yel-qori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_exit_status(void)
{
	if (WIFEXITED(exit_status))
		return (WEXITSTATUS(exit_status));
	else if (WIFSIGNALED(exit_status))
		return (128 + WTERMSIG(exit_status));
	return (EXIT_FAILURE);
}

void	redirecting(int in, int out)
{
	dup2(in, STDIN_FILENO);
	dup2(out, STDOUT_FILENO);
	if (in != STDIN_FILENO)
		close(in);
	if (out != STDOUT_FILENO)
		close(out);
}

void	pipein(int *pfd)
{
	if (pipe(pfd) == -1)
	{
		perror("pipe failed");
		exit(1);
	}
}

void	close_wait(int *pfd, t_pid *pid, t_fd *fd)
{
	close(pfd[0]);
	close(pfd[1]);
	waitpid(pid->left_pid, NULL, 0);
	waitpid(pid->right_pid, &exit_status, 0);
	exit_status = get_exit_status();
	free(pid);
}

void	handle_left_child(t_pipe_data *data, int *pfd, t_tree *root)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	close(pfd[0]);
	data->fd->out = pfd[1];
	execution(root->left, data->fd, data->env, data->exported);
	free_exit(data->pid, data->fd);
}
