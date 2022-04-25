/*
 * Syntelos SPAN
 * Copyright (C) 2022, John Pritchard, Syntelos
 *
 * This program is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * (LGPL and GPL) as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the LGPL
 * and GPL for more details.
 *
 * You should have received a copy of the LGPL and GPL along with this
 * program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <stdio.h>

#include "syntelos.h"
/*
 */
typedef enum span_class {
    span_class_unknown = 0,
    span_class_object  = '<',
    span_class_record  = '{',
    span_class_vector  = '[',
    span_class_group   = '('

} span_class;
/*
 */
typedef struct span {

    char *source;

    size_t length;

    span_class class;

    off_t begin, end;

} span;
/*
 * Re/initialize state of argument struct span.
 */
span* span_init(span*,off_t,char*,size_t);
/*
 * Destroy argument struct span.
 */
void span_destroy(span*);
/*
 * Return matching pair terminal.
 */
char span_target(char);
/*
 * Classify LHS character.
 */
span_class span_classification(char);
/*
 * Read source from offset into argument struct span.
 */
span* span_read(span*,off_t,char*,size_t);
/*
 * Dump debug representation of struct span to stdout.
 */
void span_print(FILE*,span*);

#define span_ascii(u8) (u8 & 0x7f)

#define span_is_open(ch) (('(' == ch)||('<' == ch)||('[' == ch)||('{' == ch))

#define span_is_close(ch) ((')' == ch)||('>' == ch)||(']' == ch)||('}' == ch))

#define span_class_is_valid(cl) (span_class_object == cl | span_class_record == cl | span_class_vector == cl | span_class_group == cl)

#define span_pointer(sp) (&(sp->source[sp->begin]))

#define span_length(sp) (sp->end-sp->begin)
