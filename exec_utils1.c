/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-qori <yel-qori@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 21:25:28 by yel-qori          #+#    #+#             */
/*   Updated: 2025/07/15 21:26:00 by yel-qori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	print_error_message(char *command, char *error_msg)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(command, STDERR_FILENO);
	ft_putstr_fd(error_msg, STDERR_FILENO);
}

static char	*handle_absolute_path(char *command)
{
	if (access(command, F_OK) == -1)
	{
		print_error_message(command, ": No such file or directory\n");
		exit(127);
	}
	if (access(command, X_OK) == -1)
	{
		print_error_message(command, ": Permission denied\n");
		exit(126);
	}
	return (command);
}

static char	*get_command_path(char *command, char **env)
{
	char	*path;

	if (ft_strchr(command, '/') || command[0] == '.')
		return (handle_absolute_path(command));
	path = get_path(command, env);
	if (!path)
	{
		print_error_message(command, ": command not found\n");
		exit(127);
	}
	return (path);
}

static void	execute_with_error_handling(char *path, char **args, char **env,
		char *command)
{
	if (execve(path, args, env) == -1)
	{
		if (errno == EACCES)
		{
			print_error_message(command, ": Permission denied\n");
			exit(126);
		}
		if (errno == ENOENT)
			exit(127);
		exit(EXIT_FAILURE);
	}
}

void	execute_command(t_tree *root, int in, int out, char **env)
{
	char	*path;
	int		i;

	(void)in;
	(void)out;
	i = find_valid_command(root);
	path = get_command_path(root->command[i], env);
	execute_with_error_handling(path, &root->command[i], env, root->command[i]);
}
