/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-qori <yel-qori@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 16:52:00 by rhafidi           #+#    #+#             */
/*   Updated: 2025/07/15 15:01:01 by yel-qori         ###   ########.fr       */
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

void	handle_pipe(t_pid *pid, t_fd *fd, char ***env, char ***exported,
		t_tree *root)
{
	int	pfd[2];

	pipein(pfd);
	pid->left_pid = fork();
	if (!pid->left_pid)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		close(pfd[0]);
		fd->out = pfd[1];
		execution(root->left, fd, env, exported);
		free_exit(pid, fd);
	}
	pid->right_pid = fork();
	if (!pid->right_pid)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		close(pfd[1]);
		fd->in = pfd[0];
		execution(root->right, fd, env, exported);
		free_exit(pid, fd);
	}
	close_wait(pfd, pid, fd);
}
