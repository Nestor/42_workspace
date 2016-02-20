/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_parse.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmarchau <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/02/15 12:24:32 by vmarchau          #+#    #+#             */
/*   Updated: 2016/02/20 15:23:40 by vmarchau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**env_to_tab(t_env	*env, size_t size)
{
	char	**tab;
	size_t	i;
	t_env	*tmp;

	if ((tab = malloc(sizeof(char*) * (size + 1))) == NULL)
		return (NULL);
	i = 0;
	tmp = env;
	while (i < size)
	{
		tab[i] = assmbl_env(tmp->key, tmp->value);
		ft_putendl(tab[i]);
		i++;
		tmp = tmp->next;
	}
	tab[i] = NULL;
	return (0);
}

t_env		*tab_to_env(char **tab, size_t size)
{
	t_env	*head;
	size_t	i;
	
	i = 0;
	head = NULL;
	while (i < size)
	{
		if (!head)
			head = deassmbl_env(tab[i]);
		else
			add_env_entry(head, deassmbl_env(tab[i]));
		i++;
	}
	return (head);
}

void		update_tabenv(t_global *gbl)
{
	char	**tab;
	int		i;

	tab = gbl->tabenv;
	gbl->tabenv = env_to_tab(gbl->env, gbl->env_size);
	i = 0;
	while (tab[i])
	{
		ft_putnbr(i);
		free(tab[i]);
		tab[i] = NULL;
		i++;
	}
	free(tab);
}
