/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cpp.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbrassar <bbrassar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 16:07:56 by bbrassar          #+#    #+#             */
/*   Updated: 2023/05/23 16:39:49 by bbrassar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file cpp.h
 */

#ifndef CPP_H
# define CPP_H

# ifdef __cplusplus

#  define CPP_BEGIN extern "C" {
#  define CPP_END }

# else

#  define CPP_BEGIN
#  define CPP_END

# endif

#endif // CPP_H
