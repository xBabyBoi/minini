/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_helper.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-qori <yel-qori@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 14:55:11 by yel-qori          #+#    #+#             */
/*   Updated: 2025/07/13 15:04:51 by yel-qori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_operator_char(char c)
{
	return (c == '<' || c == '>' || c == '|');
}

int	is_operator_char2(char c, char *quote)
{
	if (*quote)
		return (0);
	return (c == '<' || c == '>' || c == '|');
}

int	skip_whitespace(char *input, int i)
{
	while (input[i] && input[i] == ' ')
		i++;
	return (i);
}

int	is_quoted_token(char *token, char *quote)
{
	if (!token || !quote)
		return (0);
	if (token[0] == '\'' || token[0] == '"')
	{
		*quote = token[0];
		return (1);
	}
	return (0);
}

// char	**initial_tokenization(char *input)
// {
// 	(void)input;
// 	return (NULL);
// }