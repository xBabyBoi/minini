/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forker.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-qori <yel-qori@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 20:40:18 by yel-qori          #+#    #+#             */
/*   Updated: 2025/07/15 15:25:04 by yel-qori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_builtin_command(t_tree *root, t_fd *fd, char ***env,
		char ***exported)
{
	int	saved_stdin;
	int	saved_stdout;

	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	redirecting(fd->in, fd->out);
	if (fd->in == -1 || fd->out == -1)
	{
		exit_status = 1;
		dup2(saved_stdin, STDIN_FILENO);
		dup2(saved_stdout, STDOUT_FILENO);
		close(saved_stdin);
		close(saved_stdout);
		return ;
	}
	exit_status = handle_builtins(root, fd->in, fd->out, env, exported,
			exit_status);
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdin);
	close(saved_stdout);
}

static void	handle_child_process(t_tree *root, t_fd *fd, char **env)
{
	signal(SIGINT, child_sigint_handler);
	signal(SIGQUIT, SIG_DFL);
	redirecting(fd->in, fd->out);
	if (fd->in == -1 || fd->out == -1)
		exit(EXIT_FAILURE);
	execute_command(root, fd->in, fd->out, env);
}

static void	handle_parent_process(int pid)
{
	waitpid(pid, &exit_status, 0);
	if (WIFSIGNALED(exit_status))
	{
		if (WTERMSIG(exit_status) == SIGQUIT)
		{
			ft_putstr_fd("Quit (core dumped)\n", STDERR_FILENO);
			exit_status = 131;
		}
		else if (WTERMSIG(exit_status) == SIGINT)
		{
			ft_putstr_fd("\n", STDERR_FILENO);
			exit_status = 130;
		}
	}
	else if (WIFEXITED(exit_status))
		exit_status = WEXITSTATUS(exit_status);
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
}

static void	handle_external_command(t_tree *root, t_fd *fd, char ***env)
{
	int	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("fork failed :");
		exit(1);
	}
	if (!pid)
		handle_child_process(root, fd, *env);
	else
		handle_parent_process(pid);
}

void	forker(t_tree *root, t_fd *fd, char ***env, char ***exported)
{
	if (!root->command || !root->command[0])
	{
		handle_empty_command(fd);
		return ;
	}
	if (!is_builtin(root->command[0]))
	{
		handle_builtin_command(root, fd, env, exported);
		return ;
	}
	handle_external_command(root, fd, env);
}
