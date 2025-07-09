/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-qori <yel-qori@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 13:24:33 by yel-qori          #+#    #+#             */
/*   Updated: 2025/07/09 15:33:04 by yel-qori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include <readline/history.h>
# include <readline/readline.h>
# include <stddef.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>

extern int exit_status;

typedef struct s_token_type
{
	char			**tokens;
	char			quote;
	char			*merged;
	int				start;
	int				end;
	int				len;
	int				full_len;
	int				end_len;
}					t_token_type;

// Helper struct for segment
typedef struct s_segment {
    char *str;
    char quote; // 0=unquoted, '\''=single, '"'=double
    int was_quoted; // New: tracks if this was originally quoted
    struct s_segment *next;
} t_segment;


typedef enum type
{
	COMMAND, // cmd
	LESS,    // <
	GREATER, // >
	APPEND,  // >>
	PIPE,    // |
	HEREDOC,  // <<
}					type;

typedef struct s_tree
{
	struct s_tree	*left;
	struct s_tree	*right;
	char			*file_name;
	char			**command;
	type			type;
}					t_tree;



int check_standalone_operators(char **tokens);
int					is_operator_char(char c);
char *expand_string(char *str, char **env, int status);
char				**tokenize_input(char *input, char **env, int status);
char				*merge_tokens(char **tokens, int start, int end);
t_tree				*create_command(void);
void				print_tree(t_tree *root);
int					is_redirections(char *tokens);
t_tree				*create_redirections(char *operator, char * file_name);
t_tree				*parse_tokens(char **tokens);
void				ft_print_tree(t_tree *root);
int					initialize_end_index(char **tokens, int end);
char				**collect_command_arguments(char **tokens, int start,
						int end);
t_tree				*create_command_node(char **cmd_args);
t_tree				*process_redirections(t_tree *cmd_node, char **tokens,
						int start, int end);
char				**process_quotes(char **tokens);
void				free_token_array(char **tokens);
char				**create_new_token_array(char **tokens, char *merged_token,
						int start, int end);
int					calculate_merged_length(char **tokens, int start, int end);
int					find_quote_end(char **tokens, int start, char quote);
int					is_quoted_token(char *token, char *quote);
char				**initial_tokenization(char *input);
char				*add_delimiter_spaces(char *input);
int					check_valid_quotes(char *input);
int 				invalid_redirections(char **tokens);
int					check_redirection(char *token);
int 				is_pipe(char *token);
int					invalid_pipe(char **tokens);
int					ft_strcmp(char *s1, char *s2);
int					invalid_parenthese(char *input);
int					special_characters(char *input);

#endif