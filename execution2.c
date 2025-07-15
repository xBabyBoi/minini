/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-qori <yel-qori@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 21:23:53 by yel-qori          #+#    #+#             */
/*   Updated: 2025/07/15 21:24:47 by yel-qori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_right_child(t_pipe_data *data, int *pfd, t_tree *root)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	close(pfd[1]);
	data->fd->in = pfd[0];
	execution(root->right, data->fd, data->env, data->exported);
	free_exit(data->pid, data->fd);
}

void	handle_pipe(t_pipe_data *data, t_tree *root)
{
	int	pfd[2];

	pipein(pfd);
	data->pid->left_pid = fork();
	if (!data->pid->left_pid)
		handle_left_child(data, pfd, root);
	data->pid->right_pid = fork();
	if (!data->pid->right_pid)
		handle_right_child(data, pfd, root);
	close_wait(pfd, data->pid, data->fd);
}
