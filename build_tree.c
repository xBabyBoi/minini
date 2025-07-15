/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_tree.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-qori <yel-qori@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 14:50:58 by yel-qori          #+#    #+#             */
/*   Updated: 2025/07/15 15:01:14 by yel-qori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_tree *create_command()
{
	t_tree *node;
	
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

t_tree	*create_pipe_node(void)
{
	t_tree	*cmd;

	cmd = create_command();
	if (!cmd)
		return (NULL);
	cmd->type = PIPE;
	return (cmd);
}

int    initialize(t_tree *root, t_fd *fd, char ***env, char ***exported)
{
    execution(root, fd, env, exported);
    return (exit_status);
}
