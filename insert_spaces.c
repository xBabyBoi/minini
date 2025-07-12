/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   insert_spaces.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-qori <yel-qori@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 14:42:40 by yel-qori          #+#    #+#             */
/*   Updated: 2025/07/12 15:23:17 by yel-qori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   insert_spaces.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-qori <yel-qori@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 14:42:40 by yel-qori          #+#    #+#             */
/*   Updated: 2025/07/12 14:04:16 by yel-qori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	calculate_new_len(char *input, int len)
{
	int		new_len;
	int		i;
	char	quote;

	new_len = len;
	i = 0;
	quote = 0;
	while (i < len)
	{
		update_quote_state(input[i], &quote);
		if (!quote && is_operator_char(input[i]))
		{
			if (i + 1 < len && input[i] == input[i + 1])
			{
				new_len += 2;
				i++;
			}
			else
			{
				new_len += 2;
			}
		}
		i++;
	}
	return (new_len);
}

int	process_character(t_parser *p)
{
	if (p->input[p->i] == '\'' || p->input[p->i] == '"')
	{
		update_quote_state(p->input[p->i], &p->quote);
		p->result[p->j++] = p->input[p->i];
		return (p->j);
	}
	if (!p->quote && is_operator_char(p->input[p->i]))
	{
		if (p->i + 1 < p->len && p->input[p->i] == p->input[p->i + 1])
		{
			p->j = handle_double_operator(p);
			p->i++;
			return (p->j);
		}
		else
		{
			return (handle_single_operator(p));
		}
	}
	else
	{
		p->result[p->j++] = p->input[p->i];
		return (p->j);
	}
}

void	init_parser(t_parser *p, char *input, char *result, int len)
{
	p->input = input;
	p->result = result;
	p->i = 0;
	p->j = 0;
	p->len = len;
	p->quote = 0;
}

void	process_input(t_parser *p)
{
	while (p->i < p->len)
	{
		p->j = process_character(p);
		p->i++;
	}
}

char	*add_delimiter_spaces(char *input)
{
	int			len;
	int			new_len;
	char		*result;
	t_parser	p;

	len = strlen(input);
	new_len = calculate_new_len(input, len);
	result = malloc(new_len + 1);
	if (!result)
		return (NULL);
	init_parser(&p, input, result, len);
	process_input(&p);
	result[p.j] = '\0';
	return (result);
}
