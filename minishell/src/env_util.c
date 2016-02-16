/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmarchau <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/02/15 13:05:57 by vmarchau          #+#    #+#             */
/*   Updated: 2016/02/16 12:28:54 by vmarchau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*assmbl_env(char *key, char *value)
{
	char	*ret;
	char	*del;

	ret = ft_strjoin(key, "=");
	del = ret;
	ret = ft_strjoin(ret, value);
	free(ret);
}

t_env	*deassmbl_env(char *entry)
{
	t_env	*env;
	char	**tab;

	tab = ft_strsplit(entry, "=");
	env = new_entry(tab[0], tab[1]);
	return (env);
}

t_env	*find_entry(t_global *gbl, char *name)
{
	t_env *tmp;

	tmp = gbl->env;
	while(tmp->next)
	{
		if (ft_strcmp(name, tmp->key) == 0)
			return (tmp);
		tmp = tmp->next;
	}
	return (NULL);
}
