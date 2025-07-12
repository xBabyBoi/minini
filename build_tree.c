/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_tree.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-qori <yel-qori@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 14:50:58 by yel-qori          #+#    #+#             */
/*   Updated: 2025/07/12 15:30:12 by yel-qori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_tree	*create_command(void)
{
	t_tree	*node;

	node = malloc(sizeof(t_tree));
	if (!node)
		return (NULL);
	node->command = NULL;
	node->file_name = NULL;
	node->left = NULL;
	node->right = NULL;
	node->type = 0;
	return (node);
}

t_tree	*create_command_node(char **cmd_args)
{
	t_tree	*cmd_node;

	cmd_node = create_command();
	if (!cmd_node)
		return (NULL);
	cmd_node->type = COMMAND;
	cmd_node->command = cmd_args;
	return (cmd_node);
}

int	initialize_end_index(char **tokens, int end)
{
	if (end == -1)
	{
		end = 0;
		while (tokens[end])
			end++;
		end--;
	}
	return (end);
}

t_tree	*create_pipe_node(void)
{
	t_tree	*cmd;

	cmd = create_command();
	if (!cmd)
		return (NULL);
	cmd->type = PIPE;
	return (cmd);
}

char	**handle_error_cleanup(char **cmd_args, int arg_count)
{
	while (arg_count > 0)
		free(cmd_args[--arg_count]);
	free(cmd_args);
	return (NULL);
}
