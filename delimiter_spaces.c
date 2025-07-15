/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   delimiter_spaces.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-qori <yel-qori@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 15:17:11 by yel-qori          #+#    #+#             */
/*   Updated: 2025/07/15 21:30:44 by yel-qori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	copy_double_operator(char *input, char *result, int *i, int *j)
{
	if (*j > 0 && result[*j - 1] != ' ')
		result[(*j)++] = ' ';
	result[(*j)++] = input[*i];
	result[(*j)++] = input[*i + 1];
	result[(*j)++] = ' ';
	(*i)++;
}

void	copy_single_operator(char *input, char *result, int *i, int *j)
{
	if (*j > 0 && result[*j - 1] != ' ')
		result[(*j)++] = ' ';
	result[(*j)++] = input[*i];
	result[(*j)++] = ' ';
}

void	process_operator(char *input, char *result, int *i, int *j)
{
	if (*i + 1 < strlen(input) && input[*i] == input[*i + 1])
		copy_double_operator(input, result, i, j);
	else
		copy_single_operator(input, result, i, j);
}

void	fill_result_string(char *input, char *result, int len)
{
	int		i;
	int		j;
	char	quote;

	i = 0;
	j = 0;
	quote = 0;
	while (i < len)
	{
		handle_quote_char(input[i], &quote);
		if (input[i] == '\'' || input[i] == '"')
		{
			result[j++] = input[i];
			i++;
			continue ;
		}
		if (!quote && is_operator_char(input[i]))
			process_operator(input, result, &i, &j);
		else
			result[j++] = input[i];
		i++;
	}
	result[j] = '\0';
}

char	*add_delimiter_spaces(char *input)
{
	int		len;
	int		new_len;
	char	*result;

	len = strlen(input);
	new_len = calculate_new_length(input, len);
	result = malloc(new_len + 1);
	if (!result)
		return (NULL);
	fill_result_string(input, result, len);
	return (result);
}
