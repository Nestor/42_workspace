/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmarchau <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/02/16 11:55:17 by vmarchau          #+#    #+#             */
/*   Updated: 2016/02/20 13:39:04 by vmarchau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int			get_type(char *path)
{
	struct stat		*stat;
	int				ret;

	if ((stat = malloc(sizeof(struct stat))) == NULL)
		return -1;
	if (lstat(path, stat) == -1)
		ret = 0;
	else if (S_ISDIR(stat->st_mode))
		ret = 2;
	else if (S_ISLNK(stat->st_mode))
		ret = 3;
	else if (S_ISREG(stat->st_mode))
		ret = 1;
	else
		ret = 0;
	free(stat);
	return (ret);
}

char		*check_with_env(t_global *gbl, char *name)
{
	int		i;
	char	**tab;
	t_env	*entry;
	char	*ret;

	if ((entry = find_entry(gbl, "PATH")) == NULL)
		return (NULL);
	tab = ft_strsplit(entry->value, ':');
	i = 0;
	while (tab[i])
	{
		if (get_type((ret = strjoins(tab[i], "/", name))) == 1)
			return (ret);
		else
			free(ret);
		i++;
	}
	return (NULL);
}

void		execute(t_global *gbl, char *path, int size, char **args)
{
	(void)size;
	if (fork() == 0)
	{
		execve(path, args, env_to_tab(gbl->env, gbl->env_size));
		exit(0);
	}
	wait(NULL);
}

int			execute_cmd(t_global *gbl, int size, char **args)
{
	int		ret;
	char	*path;

	ret = get_type(args[0]);
	//if (ret == 2)
		// builtin_cd
	if (ret == 1)
		execute(gbl, args[0], size, args);
	else if (ret == 0 && (path = check_with_env(gbl, *args)) != NULL)
		execute(gbl, path, size, args);
	else
		ft_putendl("command not found");
	return (0);	
}
