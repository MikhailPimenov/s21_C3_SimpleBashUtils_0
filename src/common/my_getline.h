#ifndef SRC_COMMON_MY_GETLINE_H_
#define SRC_COMMON_MY_GETLINE_H_

#include <stdio.h>
#include <stdint.h>

//  if typedef doesn't exist (msvc, blah)
typedef intptr_t ssize_t;

ssize_t my_getline_allocate(char **line, size_t *allocated_size, FILE *stream);


#endif  //  SRC_COMMON_MY_GETLINE_H_
