/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-qori <yel-qori@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 15:57:00 by yel-qori          #+#    #+#             */
/*   Updated: 2025/07/12 15:57:03 by yel-qori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_segment	*create_segment(const char *input, size_t start, size_t len,
		char quote)
{
	t_segment	*seg;

	seg = malloc(sizeof(t_segment));
	if (!seg)
		return (NULL);
	seg->str = malloc(len + 1);
	if (!seg->str)
	{
		free(seg);
		return (NULL);
	}
	if (len)
		memcpy(seg->str, input + start, len);
	seg->str[len] = 0;
	seg->quote = quote;
	seg->next = NULL;
	return (seg);
}

t_segment	*handle_quoted_segment(const char *input, size_t *i)
{
	size_t	start;
	size_t	len;
	char	quote;

	quote = input[(*i)++];
	start = *i;
	while (input[*i])
	{
		if (input[*i] == quote)
			break ;
		(*i)++;
	}
	len = *i - start;
	if (input[*i] == quote)
		(*i)++;
	return (create_segment(input, start, len, quote));
}

t_segment	*handle_unquoted_segment(const char *input, size_t *i)
{
	size_t	start;
	size_t	len;

	start = *i;
	while (input[*i] && input[*i] != ' ' && input[*i] != '\''
		&& input[*i] != '"')
		(*i)++;
	len = *i - start;
	if (len == 0)
		return (NULL);
	return (create_segment(input, start, len, 0));
}

char	*merge_and_expand_segments(t_segment *head, char **env, int exit_status)
{
	size_t		total;
	char		*result;
	t_segment	*seg;

	total = calculate_total_length(head, env, exit_status);
	result = malloc(total + 1);
	if (!result)
		return (NULL);
	result[0] = '\0';
	seg = head;
	while (seg)
	{
		append_segment_to_result(result, seg, env, exit_status);
		seg = seg->next;
	}
	return (result);
}

t_segment	*build_segment_list(const char *input, size_t *i, size_t len)
{
	t_segment	*head;
	t_segment	*tail;
	t_segment	*seg;

	head = NULL;
	tail = NULL;
	while (*i < len && input[*i] != ' ')
	{
		seg = get_next_segment(input, i);
		if (!seg)
			break ;
		if (!head)
			head = seg;
		else
			tail->next = seg;
		tail = seg;
	}
	return (head);
}
