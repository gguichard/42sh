/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   apply_escape.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwerner <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/18 08:58:37 by fwerner           #+#    #+#             */
/*   Updated: 2019/01/27 17:51:33 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef APPLY_ESCAPE_H
# define APPLY_ESCAPE_H

/*
** Alloue et retourne une nouvelle chaine de caractere avec les caracteres
** echappes remplaces par les caracteres adequats. Retourne NULL en
** cas d'erreur.
*/
char	*apply_escape(const char *str);

/*
** Alloue et retourne une nouvelle chaine de caractere avec les newlines
** echappes (non presents dans des quotes) supprimes. Retourne NULL en cas
** d'erreur.
*/
char	*apply_only_newline_escape(const char *str);

#endif
