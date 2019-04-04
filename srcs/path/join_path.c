/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join_path.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/02 16:47:53 by gguichar          #+#    #+#             */
/*   Updated: 2019/04/02 17:06:03 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"
#include "check_path.h"

char	*join_path(const char *left_path, const char *right_path)
{
	char	*tab[3];

	if (left_path == NULL || left_path[0] == '\0')
		left_path = "./";
	tab[0] = (char *)left_path;
	tab[1] = (right_path == NULL ? "" : (char *)right_path);
	tab[2] = NULL;
	return (ft_join(tab, left_path[ft_strlen(left_path) - 1] == '/'
				? "" : "/"));
}
