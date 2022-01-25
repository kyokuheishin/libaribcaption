/*
 * Copyright (C) 2022 magicxqq <xqq@xqq.im>. All rights reserved.
 *
 * This file is part of libaribcaption.
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef ARIBCAPTION_CONTEXT_H
#define ARIBCAPTION_CONTEXT_H

#include "aribcc_export.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum aribcc_loglevel_t {
    ARIBCC_LOGLEVEL_ERROR = 0,
    ARIBCC_LOGLEVEL_WARNING = 1,
    ARIBCC_LOGLEVEL_VERBOSE = 2
} aribcc_loglevel_t;

typedef void(*aribcc_logcat_callback_t)(aribcc_loglevel_t, const char*, void*);

typedef struct aribcc_context_t aribcc_context_t;


ARIBCC_API aribcc_context_t* aribcc_context_alloc(void);

ARIBCC_API void aribcc_context_free(aribcc_context_t* context);

ARIBCC_API void aribcc_context_set_logcat_callback(aribcc_context_t* context,
                                                   aribcc_logcat_callback_t callback,
                                                   void* userdata);


#ifdef __cplusplus
}  // extern "C"
#endif

#endif  // ARIBCAPTION_CONTEXT_H
