/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmarchau <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/02/15 15:06:11 by vmarchau          #+#    #+#             */
/*   Updated: 2016/02/16 12:34:56 by vmarchau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		array_size(void **array)
{
	int	i;
	
	i = 0;
	while (array[i])
		i++;
	return (i);
}

char	*strjoins(char *first, char*second, char* third)
{
	char *ret;
	char *del;

	ret = ft_strjoin(first, second);
	del = ret;
	ret = ft_strjoin(ret, third);
	free(del);
	return (ret);
}
