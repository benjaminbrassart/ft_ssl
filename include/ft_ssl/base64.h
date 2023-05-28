/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   base64.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbrassar <bbrassar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/28 03:41:13 by bbrassar          #+#    #+#             */
/*   Updated: 2023/05/28 04:14:56 by bbrassar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BASE64_H
# define BASE64_H

# include "ft_ssl/cpp.h"
# include <stddef.h>

CPP_BEGIN

typedef struct base64_context
{
    char buffer[64];
    char* output;
    size_t capacity;
    size_t size;
} Base64Context;

void base64_init(Base64Context* context);

int base64_update(Base64Context* context, void const* data, size_t len);

int base64_finalize(Base64Context* context);

CPP_END

#endif // BASE64_H
