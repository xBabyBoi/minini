/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-qori <yel-qori@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 15:39:56 by rhafidi           #+#    #+#             */
/*   Updated: 2025/07/14 14:12:40 by yel-qori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_less_redirection(t_tree *root, int *in)
{
	if (*in != STDIN_FILENO)
		close(*in);
	*in = open(root->file_name, O_RDONLY);
	if (*in == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(root->file_name, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		exit_status = 1;
	}
}

static void	handle_greater_redirection(t_tree *root, int *out)
{
	if (*out != STDOUT_FILENO)
		close(*out);
	*out = open(root->file_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (*out == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(root->file_name, 2);
		ft_putstr_fd(": Permission denied\n", 2);
		exit_status = 1;
	}
}

void	less_and_greater(t_tree *root, int *in, int *out)
{
	if (root->type == LESS && root->file_name)
	{
		handle_less_redirection(root, in);
	}
	else if (root->type == GREATER && root->file_name)
	{
		handle_greater_redirection(root, out);
	}
}

static t_tree	*process_redirection_types(t_tree *root, int *in, int *out,
		char **env)
{
	t_tree	*cmd;

	cmd = handle_redirections(root->left, in, out, env);
	if (!cmd && exit_status == 1)
		return (NULL);
	if ((root->type == GREATER || root->type == LESS) && root->file_name)
	{
		less_and_greater(root, in, out);
		if (*in == -1 || *out == -1)
			return (NULL);
		return (cmd);
	}
	else if (root->type == APPEND && root->file_name)
	{
		append(root, in, out);
		if (*in == -1 || *out == -1)
			return (NULL);
		return (cmd);
	}
	else if (root->type == HEREDOC && root->file_name)
	{
		handle_heredoc_redirection(root, in, env);
	}
	return (cmd);
}

t_tree	*handle_redirections(t_tree *root, int *in, int *out, char **env)
{
	if (!root)
		return (NULL);
	if (root->type == COMMAND)
		return (root);
	if (root->type == LESS || root->type == GREATER || root->type == APPEND
		|| root->type == HEREDOC)
	{
		return (process_redirection_types(root, in, out, env));
	}
	return (handle_redirections(root->left, in, out, env));
}
