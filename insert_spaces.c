/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   insert_spaces.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-qori <yel-qori@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 14:42:40 by yel-qori          #+#    #+#             */
/*   Updated: 2025/07/15 15:22:59 by yel-qori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_operator(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

void	handle_quote_char(char c, char *quote)
{
	if (c == '\'' || c == '"')
	{
		if (!*quote)
			*quote = c;
		else if (*quote == c)
			*quote = 0;
	}
}

static void	process_length_for_operator(char *input, int *new_len, int *i,
		int len)
{
	if (*i + 1 < len && input[*i] == input[*i + 1])
	{
		*new_len += 2;
		(*i)++;
	}
	else
		*new_len += 2;
}

int	calculate_new_length(char *input, int len)
{
	int		new_len;
	int		i;
	char	quote;

	new_len = len;
	i = 0;
	quote = 0;
	while (i < len)
	{
		handle_quote_char(input[i], &quote);
		if (input[i] == '\'' || input[i] == '"')
		{
			i++;
			continue ;
		}
		if (!quote && is_operator_char(input[i]))
			process_length_for_operator(input, &new_len, &i, len);
		i++;
	}
	return (new_len);
}
