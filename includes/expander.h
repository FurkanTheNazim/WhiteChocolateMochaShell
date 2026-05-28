#ifndef EXPANDER_H
# define EXPANDER_H

typedef struct s_sh t_sh;

typedef enum e_seg_type
{
	SEG_NO_QUOTE,
	SEG_SINGLE_QUOTE,
	SEG_DOUBLE_QUOTE
}t_seg_type;

typedef struct s_segment
{
	char		*value;
	char		*expanded;
	int			fail;
	t_seg_type	type;
}t_segment;

int			count_dollar_sign(char *str);
void		split_by_dollar(t_sh *sh, char **tmp, char *str, int n);
int			joined_strlen(char **tmp, int n);
char		*create_expanded(t_sh *sh, char **tmp);
char		*word_splitter(t_sh *sh, char *str);

char		*get_env_value(t_sh *sh, char *str);
char		*resolve_var_name(t_sh *sh, char *str, int *i);
int			expand_dollar_segment(t_sh *sh, char **tmp, int n);
char		*expand_str(t_sh *sh, t_segment seg);

t_segment	parse_single_quote(t_sh *sh, char *str, int *i);
t_segment	parse_double_quote(t_sh *sh, char *str, int *i);
t_segment	parse_unquote(t_sh *sh, char *str, int *i);

t_segment	parse_next_segment(t_sh *sh, char *str, int *i, int *flag);
void		init_expander_data(t_sh *sh, t_segment *seg, char **new_value);
char		*expand_token(t_sh *sh, char *value, int *flag);
int			expand_token_list(t_sh *sh);

#endif