/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yel-qori <yel-qori@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 13:24:33 by yel-qori          #+#    #+#             */
/*   Updated: 2025/07/12 15:57:43 by yel-qori         ###   ########.fr       */
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

typedef struct s_parser
{
	char	*input;
	char	*result;
	int		i;
	int		j;
	int		len;
	char	quote;
}	t_parser;

typedef struct s_token_data
{
	char	**tokens;
	int		*t;
	char	**env;
	int		exit_status;
}	t_token_data;

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
t_segment	*create_segment(const char *input, size_t start, size_t len, char quote);
t_segment	*handle_quoted_segment(const char *input, size_t *i);
t_segment	*handle_unquoted_segment(const char *input, size_t *i);
char	*merge_and_expand_segments(t_segment *head, char **env, int exit_status);
t_segment	*build_segment_list(const char *input, size_t *i, size_t len);
t_segment	*get_next_segment(const char *input, size_t *i);
char	*prepare_spaced_input(char *input);
char	*merge_tokens(char **tokens, int start, int end);
char	**tokenize_input(char *input, char **env, int exit_status);
char	**initial_tokenization_with_env(char *input, char **env, int exit_status);
char	**tokenize_input(char *input, char **env, int exit_status);
char	*merge_tokens(char **tokens, int start, int end);
char	*prepare_spaced_input(char *input);
t_segment	*get_next_segment(const char *input, size_t *i);
int	check_redirection_token(char **tokens, int i);
int	invalid_redirections(char **tokens);
int	handle_missing_redirection(char **tokens, int i);
int	handle_long_redirection(char *token);
int	validate_tokens(char **tokens);
int	validate_input(char *input);
void	process_segment_list(t_segment *head, t_token_data *data);
void	skip_spaces(const char *input, size_t *i, size_t len);
void	append_segment_to_result(char *result, t_segment *seg, char **env, int exit_status);
void	append_unquoted_segment(char *result, t_segment *seg, char **env, int exit_status);
void	append_double_quoted_segment(char *result, t_segment *seg, char **env, int exit_status);
void	append_quoted_segment(char *result, t_segment *seg);
size_t	calculate_total_length(t_segment *head, char **env, int exit_status);
void	free_segments(t_segment *seg);
void	free_token_array(char **tokens);
int	is_operator_char(char c);
void	update_quote_state(char c, char *quote);
int	calculate_new_len(char *input, int len);
int	handle_double_operator(t_parser *p);
int	handle_single_operator(t_parser *p);
int	process_character(t_parser *p);
char	*add_delimiter_spaces(char *input);
void	init_parser(t_parser *p, char *input, char *result, int len);
void	process_input(t_parser *p);
t_tree	*handle_redirection(t_tree *root, char **tokens, int i, int end);
char	**handle_error_cleanup(char **cmd_args, int arg_count);
char	**process_arguments(char **cmd_args, char **tokens, int start, int end);
char	**collect_command_arguments(char **tokens, int start, int end);
t_tree	*parse_token_subset(char **tokens, int start, int end);
t_tree	*parse_tokens(char **tokens);
t_tree	*create_pipe_node(void);
size_t	calculate_segment_length(t_segment *seg, char **env, int exit_status);
void	set_env_data(t_token_data *data, char **env, int exit_status);
void	init_tokenization_data(t_token_data *data, char **tokens, int *t);
void	process_tokenization_loop(char *input, size_t len, t_token_data *data);

#endif