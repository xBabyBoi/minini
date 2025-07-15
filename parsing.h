/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-qori <yel-qori@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 13:24:33 by yel-qori          #+#    #+#             */
/*   Updated: 2025/07/14 20:47:18 by yel-qori         ###   ########.fr       */
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

typedef struct s_token_ctx {
	char	**tokens;
	char	**env;
	int		exit_status;
	int		token_count;
}	t_token_ctx;


int	check_syntax_errors(char **tokens);
int validate_redirections_syntax(char **tokens);
int is_complete_quoted_token(char *token);
char *remove_quotes_from_string(char *str);
int					is_operator_char(char c);
char *expand_string(char *str, char **env, int status, int heredoc);
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
char	**process_tokens(char *input, char **tokens, char **env,
		int exit_status);
char	**cleanup_tokens(char **tokens, int token_count);
int	skip_whitespace(char *input, int i);
int is_operator_char2(char c, char *quote);
char	**initial_tokenization_with_env(char *input, char **env,
		int exit_status);
int	handle_heredoc(char *delimiter, char **env);
void    append(t_tree *root, int *in, int *out);
t_tree	*handle_heredoc_redirection(t_tree *root, int *in, char **env);
void	strip_quotes_from_ast(t_tree *ast);
int	check_final_pipe(char **tokens, int token_count);
int	check_initial_pipe(char **tokens);
int	check_pipe_syntax(char **tokens, int i);
void	print_syntax_error(char *token);
int	check_redirection_syntax(char **tokens, int i);
int	check_tokens_loop(char **tokens);
void	free_cmd_args(char **cmd_args, int arg_count);
void	strip_quotes_from_ast(t_tree *ast);
void	process_child_nodes(t_tree *ast);
void	handle_heredoc_node(t_tree *ast);
void	handle_command_node(t_tree *ast);
void	process_command_array(char **command);
int	is_redirection(char *tokens);
t_tree	*create_pipe_node(void);
int	handle_redirection_skip(int i, int end, char **cmd_args, int arg_count);
t_tree	*handle_redirection_token(t_tree *root, char **tokens, int *i, int end);
int	is_complete_quoted_token(char *token);
int	find_token_end(char *input, int i, char *quote, int *quote_type);
char	*create_token(char *input, int start, int len, t_token_ctx *ctx);
int	process_one_token(char *input, int i, t_token_ctx *ctx);
char	**tokenize_loop(char *input, t_token_ctx *ctx);
char	**initial_tokenization_with_env(char *input, char **env, int exit_status);
char	**process_tokens(char *input, char **tokens, char **env, int exit_status);






#endif