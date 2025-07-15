/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-qori <yel-qori@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 15:17:18 by rhafidi           #+#    #+#             */
/*   Updated: 2025/07/15 14:54:55 by yel-qori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    free_array(char **str)
{
    int i;

    i = 0;
    while (str[i])
    {
        free(str[i]);
        str[i] = NULL;
        i++;
    }
    free(str);
    str = NULL;
}
char	*get_env_path(char **env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], "PATH=", 5) == 0)
			return (&env[i][5]);
		i++;
	}
	return (NULL);
}

char    *get_path(char *cmd, char **env)
{
	char	*path;
	char	**dir;
	char	*cmd_path;
	char	*tmp_cmd;
	int		i;

	i = 0;
	path = get_env_path(env);
	if (!path)
		return (NULL);
	dir = ft_split(path, ':');
	tmp_cmd = ft_strjoin("/", cmd);
	while (dir[i])
	{
		cmd_path = ft_strjoin(dir[i], tmp_cmd);
		if (!access(cmd_path, F_OK | X_OK))
		{
			free_array(dir);
			return (cmd_path);
		}
		free(cmd_path);
		i++;
	}
	(free(tmp_cmd), free_array(dir));
	return (NULL);
}

void    access_exec(char **argv, char **env)
{
    if (access(argv[0], F_OK | X_OK) == -1)
    {
        perror("access failed : ");
        free_array(argv);
        exit(1);
    }    
    if (execve(argv[0], argv, env) == -1)
    {
        perror("exec failed :");
        free_array(argv);
        exit(1);
    }
}

static int	find_valid_command(t_tree *root)
{
	int	i;

	i = 0;
	while (root->command[i][0] == '\0')
	{
		i++;
		if (!root->command[i])
			exit(EXIT_SUCCESS);
	}
	return (i);
}

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

static void	execute_with_error_handling(char *path, char **args, char **env, char *command)
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
