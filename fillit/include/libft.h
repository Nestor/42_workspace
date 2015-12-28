/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmarchau <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/11/23 13:35:21 by vmarchau          #+#    #+#             */
/*   Updated: 2015/12/02 13:16:37 by vmarchau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H
# include <string.h>
# include <stdlib.h>
# include <unistd.h>

typedef	struct	s_list
{
	void			*content;
	size_t			content_size;
	struct s_list	*next;
}				t_list;

int				ft_toupper(int c);
int				ft_tolower(int c);
int				ft_isalpha(int c);
int				ft_isdigit(int c);
int				ft_isalnum(int c);
int				ft_isascii(int c);
int				ft_isprint(int c);
int				ft_atoi(const char *str);

char			*ft_strstr(char *str1, char *str2);
char			*ft_strnstr(const char *str1, const char *str2, size_t nb);
size_t			ft_strlen(const char *str);
int				ft_strcmp(const char *str1, const char *str2);
int				ft_strncmp(const char *str1, const char *str2, size_t nb);
char			*ft_strrchr(char *str, int c);
char			*ft_strchr(char *str, int c);
char			*ft_strcat(const char *dest, const char *src);
char			*ft_strncat(char *dest, const char *src, size_t size);
size_t			ft_strlcat(char *dest, const char *src, size_t size);
char			*ft_strcpy(char *dest, const char *src);
char			*ft_strncpy(char *dest, const char *src, size_t size);
char			*ft_strdup(char *str);
int				ft_strfind(char *str1, char *str2);

int				ft_memcmp(const void *s1, const void *s2, size_t size);
void			*ft_memchr(const void *s, int c, size_t size);
void			*ft_memmove(void *dest, const void *src, size_t size);
void			*ft_memccpy(void *dest, const void *src, int c, size_t size);
void			*ft_memcpy(void *dest, const void *src, size_t size);
void			*ft_memset(void *s, int c, size_t size);
void			ft_bzero(void *s, size_t size);

void			*ft_memalloc(size_t size);
void			ft_memdel(void **ap);
char			*ft_strnew(size_t size);
void			ft_strdel(char **as);
void			ft_strclr(char *s);
void			ft_striter(char *s, void (*f)(char *));
void			ft_striteri(char *s, void (*f)(unsigned int, char *));
char			*ft_strmap(char const *s, char (*f)(char));
char			*ft_strmapi(char const *s, char (*f)(unsigned int, char));
int				ft_strequ(char const *s1, char const *s2);
int				ft_strnequ(char const *s1, char const *s2, size_t n);
char			*ft_strsub(char const *s1, unsigned int start, size_t len);
char			*ft_strjoin(char const *s1, char const *s2);
char			*ft_strtrim(char const *s);
char			**ft_strsplit(char const *s, char c);
char			*ft_itoa(int n);
void			ft_putchar(char c);
void			ft_putstr(char const *s);
void			ft_putendl(char const *s);
void			ft_putnbr(int n);
void			ft_putchar_fd(char c, int fd);
void			ft_putstr_fd(char const *s, int fd);
void			ft_putendl_fd(char const *s, int fd);
void			ft_putnbr_fd(int n, int fd);

int				ft_power(int nb, int pow);
size_t			ft_count_char(char *str, char c);

t_list			*ft_lstnew(void const *content, size_t content_size);
void			ft_lstdelone(t_list **alst, void (*del)(void *, size_t));
void			ft_lstdel(t_list **alst, void (*del)(void *, size_t));
void			ft_lstadd(t_list **alstm, t_list *new);
void			ft_lstiter(t_list *lst, void (*f)(t_list *elem));
t_list			*ft_lstmap(t_list *lst, t_list *(*f)(t_list *elem));

#endif
