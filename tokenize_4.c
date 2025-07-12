/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_4.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-qori <yel-qori@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 13:06:29 by yel-qori          #+#    #+#             */
/*   Updated: 2025/07/12 15:42:57 by yel-qori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_redirection_token(char **tokens, int i)
{
	if (ft_strlen(tokens[i]) > 2)
		return (handle_long_redirection(tokens[i]));
	if (!handle_missing_redirection(tokens, i))
		return (0);
	return (1);
}

int	invalid_redirections(char **tokens)
{
	int	i;

	if (!tokens)
		return (0);
	i = 0;
	while (tokens[i])
	{
		if ((tokens[i][0] == '>' || tokens[i][0] == '<')
			&& (ft_strlen(tokens[i]) <= 2) && (tokens[i][1] == '\0'
				|| tokens[i][1] == tokens[i][0]))
		{
			if (!check_redirection_token(tokens, i))
				return (0);
		}
		i++;
	}
	return (1);
}

char	**initial_tokenization(char *input)
{
	(void)input;
	return (NULL);
}
