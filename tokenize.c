/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-qori <yel-qori@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 15:05:52 by yel-qori          #+#    #+#             */
/*   Updated: 2025/07/14 15:05:54 by yel-qori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	strip_quotes_from_tokens(char **tokens, int skip_heredoc_delimiter)
{
	int		i;
	char	*new_token;

	if (!tokens)
		return ;
	i = 0;
	while (tokens[i])
	{
		if (skip_heredoc_delimiter && i == 1)
		{
			i++;
			continue ;
		}
		new_token = remove_quotes_from_string(tokens[i]);
		if (new_token)
		{
			free(tokens[i]);
			tokens[i] = new_token;
		}
		i++;
	}
}

char	*merge_tokens(char **tokens, int start, int end)
{
	char	*merged;
	char	*tmp;
	int		i;

	merged = ft_strdup("");
	i = start;
	while (i <= end)
	{
		tmp = ft_strjoin(merged, tokens[i]);
		free(merged);
		merged = tmp;
		if (i != end)
		{
			tmp = ft_strjoin(merged, " ");
			free(merged);
			merged = tmp;
		}
		i++;
	}
	return (merged);
}
