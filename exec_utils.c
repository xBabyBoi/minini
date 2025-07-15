/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-qori <yel-qori@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 15:17:18 by rhafidi           #+#    #+#             */
/*   Updated: 2025/07/15 21:26:56 by yel-qori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

char	*get_path(char *cmd, char **env)
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

void	access_exec(char **argv, char **env)
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

int	find_valid_command(t_tree *root)
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
