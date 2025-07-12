/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-qori <yel-qori@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 13:00:43 by yel-qori          #+#    #+#             */
/*   Updated: 2025/07/12 15:45:49 by yel-qori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_operator_char(char c)
{
	return (c == '<' || c == '>' || c == '|');
}

size_t	calculate_segment_length(t_segment *seg, char **env, int exit_status)
{
	char	*expanded;
	size_t	len;

	if (seg->quote == '\'')
	{
		if (is_operator_char(seg->str[0]) && strlen(seg->str) == 1)
			len = strlen(seg->str) + 2;
		else
			len = strlen(seg->str);
	}
	else
	{
		expanded = expand_string(seg->str, env, exit_status);
		if (seg->quote == '"' && is_operator_char(seg->str[0])
			&& strlen(seg->str) == 1)
			len = strlen(expanded) + 2;
		else
			len = strlen(expanded);
		if (expanded != seg->str)
			free(expanded);
	}
	return (len);
}

size_t	calculate_total_length(t_segment *head, char **env, int exit_status)
{
	size_t		total;
	t_segment	*seg;

	total = 0;
	seg = head;
	while (seg)
	{
		total += calculate_segment_length(seg, env, exit_status);
		seg = seg->next;
	}
	return (total);
}

void	append_quoted_segment(char *result, t_segment *seg)
{
	if (is_operator_char(seg->str[0]) && strlen(seg->str) == 1)
	{
		strcat(result, "'");
		strcat(result, seg->str);
		strcat(result, "'");
	}
	else
		strcat(result, seg->str);
}
