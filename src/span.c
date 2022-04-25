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

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "span.h"

span *span_init(span* state, off_t offset, char *source, size_t length){

    if (null != source && 0 != length){

        if (null == state){

            state = calloc(1,sizeof(span));
        }
        else {

            memset(state,0,sizeof(span));
        }

        if (null != state){

            state->source = source;
            state->length = length;
            state->begin = offset;
            state->end = offset;
        }
    }
    return state;
}
void span_destroy(span* state){

    if (null != state){

        memset(state,0,sizeof(span));
        free(state);
    }
}
char span_target(char lhs){
    switch(lhs){
    case '<':
        return '>';
    case '[':
        return ']';
    case '{':
        return '}';
    case '(':
        return ')';
    default:
        return 0;
    }
}
span_class span_classification(char lhs){
    switch(lhs){
    case '<':
        return span_class_object;
    case '[':
        return span_class_vector;
    case '{':
        return span_class_record;
    case '(':
        return span_class_group;
    default:
        return span_class_unknown;
    }
}
bool_t span_read_close(span *scope, off_t offset, char *source, size_t length){
    if (null != scope && null != source && 0 != length){

        char lhs = span_ascii(source[scope->begin]);
        char rhs = span_target(lhs);

        if (0 != rhs){
            off_t nesting = 0;
            off_t ofs;

            for (ofs = (offset+1); ofs < length; ofs++){

                char co = span_ascii(source[ofs]);

                if (rhs == co){

                    if (0 == nesting){

                        scope->class = span_classification(lhs);

                        scope->end = (ofs+1);

                        return true;
                    }
                    else {

                        nesting -= 1;
                    }
                }
                else if (lhs == co){

                    nesting += 1;
                }
            }
        }
    }
    return false;
}
span* span_read(span* state, off_t offset, char *source, size_t length){
    if (null != source && 0 != length){

        bool_t new = (null == state);

        state = span_init(state,offset,source,length);
        if (null != state){

            for (; offset < length; offset++){

                char co = span_ascii(source[offset]);
                if (span_is_open(co)){

                    state->end = offset;
                    state->begin = offset;

                    if (span_read_close(state,offset,source,length)){
                        return state;
                    }
                }
            }

            if (new){

                free(state);
            }
        }
    }
    return null;
}
void span_print(FILE* out, span *sp){
    if (null != sp && span_class_is_valid(sp->class)){
        char *cp;
        char *content;
        size_t cz;

        cp = span_pointer(sp);
        cz = span_length(sp);
        content = strndup(cp,cz);

        switch(sp->class){
        case span_class_object:
            fprintf(out,"span_class_object\t%s\n",content);
            break;
        case span_class_record:
            fprintf(out,"span_class_record\t%s\n",content);
            break;
        case span_class_vector:
            fprintf(out,"span_class_vector\t%s\n",content);
            break;
        case span_class_group:
            fprintf(out,"span_class_group\t%s\n",content);
            break;
        default:
            break;
        }

        free(content);
    }
}
