/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_tree.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-qori <yel-qori@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 15:20:39 by rhafidi           #+#    #+#             */
/*   Updated: 2025/07/14 14:23:39 by yel-qori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_tree(t_tree **root)
{
	if (!*root)
		return ;
	free_tree(&(*root)->left);
	free_tree(&(*root)->right);
	free((*root)->command);
	free((*root));
}

void	free_token_array(char **tokens)
{
	int	i;

	i = 0;
	if (!tokens)
		return ;
	while (tokens[i])
	{
		free(tokens[i]);
		i++;
	}
	free(tokens);
}

char	**cleanup_tokens(char **tokens, int token_count)
{
	int	j;

	j = 0;
	while (j < token_count)
	{
		free(tokens[j]);
		j++;
	}
	free(tokens);
	return (NULL);
}

void	free_cmd_args(char **cmd_args, int arg_count)
{
	while (arg_count > 0)
		free(cmd_args[--arg_count]);
	free(cmd_args);
}
