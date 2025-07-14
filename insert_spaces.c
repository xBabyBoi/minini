/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   insert_spaces.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-qori <yel-qori@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 14:42:40 by yel-qori          #+#    #+#             */
/*   Updated: 2025/07/14 13:13:28 by yel-qori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_operator(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

static void	handle_quote_char(char c, char *quote)
{
	if (c == '\'' || c == '"')
	{
		if (!*quote)
			*quote = c;
		else if (*quote == c)
			*quote = 0;
	}
}

static void	process_length_for_operator(char *input, int *new_len, int *i, int len)
{
	if (*i + 1 < len && input[*i] == input[*i + 1])
	{
		*new_len += 2;
		(*i)++;
	}
	else
		*new_len += 2;
}

static int	calculate_new_length(char *input, int len)
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

static void	copy_double_operator(char *input, char *result, int *i, int *j, int len)
{
	if (*j > 0 && result[*j - 1] != ' ')
		result[(*j)++] = ' ';
	result[(*j)++] = input[*i];
	result[(*j)++] = input[*i + 1];
	if (*i + 2 < len && input[*i + 2] != ' ')
		result[(*j)++] = ' ';
	(*i)++;
}

static void	copy_single_operator(char *input, char *result, int *i, int *j, int len)
{
	if (*j > 0 && result[*j - 1] != ' ')
		result[(*j)++] = ' ';
	result[(*j)++] = input[*i];
	if (*i + 1 < len && input[*i + 1] != ' ')
		result[(*j)++] = ' ';
}

static void	process_operator(char *input, char *result, int *i, int *j, int len)
{
	if (*i + 1 < len && input[*i] == input[*i + 1])
		copy_double_operator(input, result, i, j, len);
	else
		copy_single_operator(input, result, i, j, len);
}

static void	fill_result_string(char *input, char *result, int len)
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
			process_operator(input, result, &i, &j, len);
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

