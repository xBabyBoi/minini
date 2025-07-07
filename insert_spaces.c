/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   insert_spaces.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-qori <yel-qori@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 14:42:40 by yel-qori          #+#    #+#             */
/*   Updated: 2025/07/07 16:36:33 by yel-qori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_operator(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

char *add_delimiter_spaces(char *input)
{
    int i = 0;
    int len = strlen(input);
    char *result = malloc(len * 3 + 1); // Extra space for potential spaces
    int j = 0;
    char quote = 0;
    
    if (!result)
        return NULL;
    
    while (input[i])
    {
        // Track quote state - if we're inside quotes, don't add spaces
        if ((input[i] == '\'' || input[i] == '"') && !quote)
            quote = input[i];
        else if (input[i] == quote)
            quote = 0;
        
        // Only add spaces around operators if NOT inside quotes
        if (!quote && is_operator_char(input[i]))
        {
            // Add space before operator if needed
            if (j > 0 && result[j-1] != ' ')
                result[j++] = ' ';
            
            // Add the operator
            result[j++] = input[i];
            
            // Handle double operators (>> or <<)
            if (input[i+1] == input[i])
            {
                result[j++] = input[++i];
            }
            
            // Add space after operator if needed
            if (input[i+1] && input[i+1] != ' ')
                result[j++] = ' ';
        }
        else
        {
            // Just copy the character as-is
            result[j++] = input[i];
        }
        i++;
    }
    result[j] = '\0';
    return result;
}