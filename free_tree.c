/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_tree.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-qori <yel-qori@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 15:20:39 by rhafidi           #+#    #+#             */
/*   Updated: 2025/07/12 15:45:03 by yel-qori         ###   ########.fr       */
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

void	free_segments(t_segment *seg)
{
	t_segment	*tmp;

	while (seg)
	{
		tmp = seg->next;
		free(seg->str);
		free(seg);
		seg = tmp;
	}
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
