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
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#include "span.h"

int main_usage(int argc, char **argv){
    char *n = argv[0];
    printf("Synopsis\n\n\t%s -i <file>\n\n",n);
    return 1;
}

typedef struct main_file {

    char *content;

    size_t length;

} main_file;

main_file* main_read(const char *reference){
    struct stat sfi;
    if (0 == stat(reference,&sfi)){
        size_t length = sfi.st_size;
        if (0 != length){
            char *content = calloc(1,(length+1));
            if (null != content){
                int fd = open(reference,O_RDONLY);
                if (-1 < fd){
                    char* cp = content;
                    size_t rm = length;
                    ssize_t rd;
                    while (0 < rm){

                        if (0 < (rd = read(fd,cp,rm))){

                            cp += rd;
                            rm -= rd;
                        }
                        else {
                            break;
                        }
                    }
                    close(fd);

                    main_file *m = calloc(1,sizeof(main_file));
                    if (null != m){

                        m->length = length;
                        m->content = content;

                        return m;
                    }
                    else {
                        free(content);
                    }
                }
                else {
                    free(content);
                }
            }
        }
    }
    return null;
}
int main_input(int argc, char **argv){
    main_file *file = main_read(argv[2]);
    if (null != file){
        char *source = file->content;
        size_t length = file->length;
        {
            free(file);
            file = null;
        }
        off_t offset;
        span *sp = null, *ck;

        for (offset = 0; offset < length; ){

            ck = span_read(sp,offset,source,length);
            if (null == sp){

                if (null != ck){
                    sp = ck;

                    span_print(stdout,sp);

                    offset = sp->end;
                }
                else {

                    return 1;
                }
            }
            else if (null == ck){

                break;
            }
            else {
                span_print(stdout,sp);

                offset = sp->end;
            }
        }
        return 0;
    }
    else {
        return 1;
    }
}
int main(int argc, char **argv){

    if (3 == argc && (0 == strcmp("-i",argv[1]))){

        return main_input(argc,argv);
    }
    else {
        return main_usage(argc,argv);
    }
}
