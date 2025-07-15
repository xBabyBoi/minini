/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-qori <yel-qori@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 14:58:59 by yel-qori          #+#    #+#             */
/*   Updated: 2025/07/15 21:20:51 by yel-qori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_pid	*init_pid_struct(void)
{
	t_pid	*pid;

	pid = malloc(sizeof(t_pid));
	if (!pid)
		return (NULL);
	return (pid);
}

static void	handle_redirection_execution(t_tree *root, t_fd *fd, char ***env,
		char ***exported)
{
	t_tree	*cmd;
	t_pid	*pid;

	pid = init_pid_struct();
	if (!pid)
		return ;
	cmd = handle_redirections(root, &fd->in, &fd->out, env[0]);
	if (exit_status != 0)
	{
		cleanup_fds_and_pid(fd, pid);
		return ;
	}
	if (cmd)
		forker(cmd, fd, env, exported);
	else
		forker(NULL, fd, env, exported);
	cleanup_fds_and_pid(fd, pid);
}

static void	handle_command_execution(t_tree *root, t_fd *fd, char ***env,
		char ***exported)
{
	t_pid	*pid;

	pid = init_pid_struct();
	if (!pid)
		return ;
	forker(root, fd, env, exported);
	free(pid);
}

static void	handle_pipe_execution(t_tree *root, t_fd *fd, char ***env,
		char ***exported)
{
	t_pid		*pid;
	t_pipe_data	data;

	pid = init_pid_struct();
	if (!pid)
		return ;
	data.pid = pid;
	data.fd = fd;
	data.env = env;
	data.exported = exported;
	handle_pipe(&data, root);
}

void	execution(t_tree *root, t_fd *fd, char ***env, char ***exported)
{
	if (!root)
		return ;
	if (root->type == APPEND || root->type == GREATER || root->type == LESS
		|| root->type == HEREDOC)
		handle_redirection_execution(root, fd, env, exported);
	else if (root->type == COMMAND)
		handle_command_execution(root, fd, env, exported);
	else if (root->type == PIPE)
		handle_pipe_execution(root, fd, env, exported);
}
