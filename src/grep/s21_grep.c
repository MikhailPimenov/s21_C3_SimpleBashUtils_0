
#include <regex.h>

#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define UNUSED_SHIT(fucking_unused_shit) do { (void)(fucking_unused_shit); } while (0)

// typedef intptr_t ssize_t;

// ssize_t getline(char **lineptr, size_t *n, FILE *stream) {
//     size_t pos;
//     int c;

//     if (lineptr == NULL || stream == NULL || n == NULL) {
//         errno = EINVAL;
//         return -1;
//     }

//     c = getc(stream);
//     if (c == EOF) {
//         return -1;
//     }

//     if (*lineptr == NULL) {
//         *lineptr = malloc(128);
//         if (*lineptr == NULL) {
//             return -1;
//         }
//         *n = 128;
//     }

//     pos = 0;
//     while(c != EOF) {
//         if (pos + 1 >= *n) {
//             size_t new_size = *n + (*n >> 2);
//             if (new_size < 128) {
//                 new_size = 128;
//             }
//             char *new_ptr = realloc(*lineptr, new_size);
//             if (new_ptr == NULL) {
//                 return -1;
//             }
//             *n = new_size;
//             *lineptr = new_ptr;
//         }

//         ((unsigned char *)(*lineptr))[pos ++] = c;
//         if (c == '\n') {
//             break;
//         }
//         c = getc(stream);
//     }

//     (*lineptr)[pos] = '\0';
//     return pos;
// }


enum Boolean {
    False,
    True
};

typedef struct Flags {
    char e;
    char i;
    char v;
    char c;
    char l;
    char n;
    char h;
    char s;
    char f;
    char o;

    char print_filename;
} Flags;


typedef struct FileStruct {
    char* data;
    int file_length;

    char** line;
    int line_count;
} FileStruct;

enum ArgumentTypes {
    ONLY_FLAG_T = 0,
    PATTERN_T = 1,
    FILENAME_T = 2,
    REGEX_FILENAME_T = 3
};

typedef struct Arguments {
    const char **word;
    int counter;

    int *type;
} Arguments;

void initialize_arguments_struct_allocate(Arguments* arguments_struct, int counter, const char** arguments) {
    arguments_struct->word = arguments;
    arguments_struct->counter = counter;
    arguments_struct->type = malloc(counter * sizeof(int));
    
    if (!arguments_struct->type)
        exit(-1);

    for (int index = 0; index < counter; ++index)
        arguments_struct->type[index] = ONLY_FLAG_T;
}

void free_arguments_struct(Arguments* arguments_struct) {
    if (arguments_struct->type) {
        free(arguments_struct->type);
        arguments_struct->type = NULL;
    }

    arguments_struct->word = NULL;
    arguments_struct->counter = 0;
}

// void print_file_struct(const FileStruct* file_struct) {
//     printf("begin:\n");

//     for (int line_index = 0; line_index < file_struct->line_count; ++line_index)
//         printf("%s\n", file_struct->line[line_index]);

//     printf("end:\n");
// }

void initialize_flags(Flags *flags) {
    flags->e = False;
    flags->i = False;
    flags->v = False;
    flags->c = False;
    flags->l = False;
    flags->n = False;
    flags->h = False;
    flags->s = False;
    flags->f = False;
    flags->o = False;

    flags->print_filename = False;
}

void print_line_number(int number) {
    printf("%d:", number);                                                                     
}

int are_equal(const char* string1, const char* string2, int length, int ignore_case) {
    for (int index = 0; index < length; ++index) {
        char symbol1 = string1[index];
        char symbol2 = string2[index];

        if (ignore_case) {
            if ('A' <= string1[index] && string1[index] <= 'Z')
                symbol1 += ('a' - 'A');
            if ('A' <= string2[index] && string2[index] <= 'Z')
                symbol2 += ('a' - 'A');
        }

        if (symbol1 != symbol2)
            return False;
    }
    return True;
}

typedef struct Line {
    const char* line;
    int length;
    int line_number;
    int match_counter;
    const char* pattern_word;
    const char* filename;
} Line;

void print_line(const Line* line, const Flags* flags) {
    
    if (flags->print_filename && !flags->h)
        printf("%s:", line->filename);

    if (flags->n)
        print_line_number(line->line_number);

    for(int index = 0; index < line->length; ++index)
        printf("%c", line->line[index]);

}

int get_string_length(const char* string) {
    if (!string)
        return 0;

    int length = 0;
    while (string[length])
        ++length;
    return length;
}

int get_line_length(const char* string) {
    if (!string)
        return 0;

    int length = 0;
    while (string[length] != '\n')
        ++length;
    ++length;
    return length;    
}

void print_for_o(const Line* line, int begin, int end, const Flags* flags, int* is_beginning_of_the_line) {
    if (*is_beginning_of_the_line) {

        if (flags->print_filename && !flags->h)
            printf("%s:", line->filename);

        if (flags->n)
            print_line_number(line->line_number);

        *is_beginning_of_the_line = False;
    }

    for (int index = begin; index < end; ++index)
        printf("%c", line->line[index]);

    printf("\n");
}

int is_line_suitable_and_print_o(Line* line, const Flags* flags, const Arguments* argument_struct, const FileStruct* all_regexes) {
    int is_suitable = False;

    int is_beginning_of_the_line = True;

    int offset = 0;

    for (int pattern_number = 0; pattern_number < argument_struct->counter; ++pattern_number) {
        
        if (argument_struct->type[pattern_number] == PATTERN_T) {

            const char* pattern = argument_struct->word[pattern_number];
            const int pattern_length = get_string_length(pattern);

            for (int index = offset; index < line->length; ++index) {

                if (index + pattern_length < line->length) {

                    if (are_equal(line->line + index, pattern, pattern_length, flags->i)) {

                        offset = index;

                        is_suitable = True;

                        if (!flags->o || flags->v || flags->c) {
                            break;
                        }

                        print_for_o(line, index, index + pattern_length, flags, &is_beginning_of_the_line);
                    }
                }  
            }
        }

        if (is_suitable && !flags->o) {
            break;
        }

    }

    UNUSED_SHIT(all_regexes);
    
    regex_t regex;
    

    for (int regex_number = 0; regex_number < all_regexes->line_count; ++regex_number) {

        const char* regex_word = all_regexes->line[regex_number];
        
        int compile_result = -1;

        if (flags->i)
            compile_result = regcomp(&regex, regex_word, REG_ICASE);
        else
            compile_result = regcomp(&regex, regex_word, 0);


        if (!compile_result) {
            
            regmatch_t match;
            int eflags = 0;
            
            while (0 == regexec(&regex, line->line + offset, 1, &match, eflags)){
                eflags = REG_NOTBOL;  //  not Beginning Of Line

                is_suitable = True;

                if (!flags->o || flags->v || flags->c) {
                        break;
                }
                
                const int begin = offset + match.rm_so;
                const int end = offset + match.rm_eo;
                print_for_o(line, begin, end, flags, &is_beginning_of_the_line);
                offset += end; // += end ???

                if (end == begin)
                    ++offset;
                
                if (offset > line->length)
                    break;
            }


        } else {
            fprintf(stderr, "Regex compilation fail\n");
        }
                
        if (is_suitable && !flags->o) {
            break;
        }

    }

    
    if (flags->v)
        is_suitable = !is_suitable;

    return is_suitable;
}

void initialize_line(Line* line) {
    line->line = NULL;
    line->length = 0;
    line->line_number = 0;
    line->match_counter = 0;
    line->pattern_word = NULL;
    line->filename = NULL;
}

void read_and_output_file_line_by_line(const char* filename, int is_last_file, const Flags* flags, const Arguments* arguments_struct, const FileStruct* all_regexes) {
    FILE* input_file = fopen(filename, "r");
    if (input_file == NULL) {
        if (!flags->s)  
            printf("Failed to open file %s\n", filename);
        return;
    }

    ssize_t line_actual_length = 0ul;
    size_t line_allocated_length = 0l;
    char *line_for_getline = NULL;

    int line_number = 1;                                                                                    //  first line has number '1'
    
    int is_file_suitable = False;
    int suitable_line_counter = 0;
    int is_previous_newline = False;
    while (True)  {                                                                                         //  getline allocates memory

        line_actual_length = getline(&line_for_getline, &line_allocated_length, input_file);



        Line line;
        initialize_line(&line);

        line.line = line_for_getline;
        line.length = line_actual_length;
        line.line_number = line_number;
        line.filename = filename;


        if (line_actual_length == EOF) {
            if (!is_previous_newline && !is_last_file && !flags->c && !flags->l) {
                printf("\n");
            }
            break;
        }

        if (is_line_suitable_and_print_o(&line, flags, arguments_struct, all_regexes)) {
            ++suitable_line_counter;

            if (line.length == 1 && line.line[0] == '\n') {
                is_previous_newline = True;
            } else {
                is_previous_newline = False;
            }

            if (flags->l) {
                is_file_suitable = True;
                break;
            }

            if ((!flags->o || flags->v) && !flags->c)
                print_line(&line, flags);
        }
        ++line_number;
    }

    free(line_for_getline);                                                                                             //  because getline allocates memory
    fclose(input_file);

    if (flags->c) {
        printf("%s:", filename);
        printf("%d\n", suitable_line_counter);
    }

    if (flags->l && is_file_suitable)
        printf("%s\n", filename);
}

void print_command_line_arguments(int counter, const char** arguments) {
    for (int index = 0; index < counter; ++index) {
        printf("%d - %s\n", index, arguments[index]);
    }
}


/* const char** arguments
 1               "-e",                  arguments[1]    arguments[1][1] = 'e'
 2               "include",             arguments[2]
 3               "-e",              
 4               "define",
 5               "${workspaceFolder}/src/grep/test2.txt",
 6               "-emain",              arguments[6]    arguments[6] + 1 + 1 = "main"
 7               "-ni",
 8               "-nf${workspaceFolder}/src/grep/regex_patterns1.txt",
 9              "${workspaceFolder}/src/grep/s21_grep.c",
 10              "-f",
 11              "${workspaceFolder}/src/grep/regex_patterns2.txt",
*/


int parse(int counter, const char** arguments, Flags* flags, Arguments* arguments_struct) {
    arguments_struct->counter = counter;

    int filename_counter = 0;
    for (int argument_index = 1; argument_index < counter; ++argument_index) {
    
        const char* argument = arguments[argument_index];
        const int argument_length = get_string_length(argument);

        if (argument[0] == '-') {
 
            for (int index = 1; index < argument_length; ++index) {
                
                if (argument[index] == 'e') {

                    flags->e = True;

                    if (index == argument_length - 1 ) {

                        if (argument_index < counter - 1) {

                            arguments_struct->type[argument_index + 1] = PATTERN_T;

                            ++argument_index;

                        } else {

                            fprintf(stderr, "grep: option requires an argument -- e");
                        
                        }
                    } else {

                        arguments_struct->word[argument_index] = arguments[argument_index] + index + 1;
                        arguments_struct->type[argument_index] = PATTERN_T;
                        
                    }
                    
                    break;

                } else if (argument[index] == 'i') {
                    flags->i = True;
                } else if (argument[index] == 'v') {
                    flags->v = True;
                } else if (argument[index] == 'c') {
                    flags->c = True;
                } else if (argument[index] == 'l') {
                    flags->l = True;
                } else if (argument[index] == 'n') {
                    flags->n = True;
                } else if (argument[index] == 'h') {
                    flags->h = True;
                } else if (argument[index] == 's') {
                    flags->s = True;
                } else if (argument[index] == 'f') {
                    
                    flags->f = True;

                    if (index == argument_length - 1 ) {

                        if (argument_index < counter - 1) {

                            arguments_struct->type[argument_index + 1] = REGEX_FILENAME_T;
                            
                            ++argument_index;

                        } else {

                            fprintf(stderr, "grep: option requires an argument -- f");

                        }

                    } else {

                        arguments_struct->word[argument_index] = arguments[argument_index] + index + 1;
                        arguments_struct->type[argument_index] = REGEX_FILENAME_T;
                        
                    }
                    
                    break;

                } else if (argument[index] == 'o') {

                    flags->o = True;
                
                } else {

                    fprintf(stderr, "invalid flag %c%c\n", '-', argument[index]);
                
                }
            }

        } else {

            arguments_struct->type[argument_index] = FILENAME_T;

            ++filename_counter;

        }

    }

    if (!flags->e && !flags->f) {
        for (int index = 0; index < arguments_struct->counter; ++index) {
            if (arguments_struct->type[index] == FILENAME_T) {
                arguments_struct->type[index] = PATTERN_T;
                break;
            }
        }
        --filename_counter;
    }

    if (filename_counter > 1)
        flags->print_filename = True;

    
    return filename_counter;
}

int get_file_character_count(const char* filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "No file %s\n", filename);
        return -1;
    }

    int counter = 1;

    while (fgetc(file) != EOF)
        ++counter;

    fclose(file);
    return counter;
}

int get_file_line_count(const char* filename) {
    FILE* file = fopen(filename, "r");

    if (!file) {
        fprintf(stderr, "No file %s\n", filename);
        return -1;
    }

    int counter = 1;
    while (True) {
        const int symbol = fgetc(file);

        if (symbol == '\n')
            ++counter;

        if (symbol == EOF) {
            break;
        }
    }

    fclose(file);
    return counter;
}

void initialize_file_struct(FileStruct* file_struct) {
    file_struct->data = NULL;
    file_struct->file_length = -1;

    file_struct->line = NULL;
    file_struct->line_count = -1;
}

void set_struct_from_file_allocate(const char* filename, FileStruct* file_struct) {
 
    int success = True;

    const int file_length = get_file_character_count(filename);
    const int line_count = get_file_line_count(filename);

    if (file_length == -1 || line_count == -1) {
        success = False;
    }

    if (success) {
        file_struct->data = malloc(file_length * sizeof(char));  //  character as int not char

        if (!file_struct->data)
            success = False;
        
        if (success) {
            file_struct->line = malloc(line_count * sizeof(char*));  

            if (!file_struct->line)
                success = False;
            
            if (success) {
                FILE* file = fopen(filename, "r");

                if (!file)
                    success = False;

                if (success) {

                    int symbol_index = 0;
                    int line_index = 0;

                    file_struct->line[0] = file_struct->data;
                    ++line_index;

                    int is_previous_newline = False;
                    while (True) {
                        const int symbol = fgetc(file);

                        if (symbol == EOF) {
                            file_struct->data[symbol_index] = '\0';
                            break;
                        }

                        file_struct->data[symbol_index] = (char)symbol;  // записываем по одному символу

                        if (is_previous_newline) {
                            file_struct->line[line_index] = file_struct->data + symbol_index;
                            ++line_index;
                        }
                        is_previous_newline = False;

                        if (symbol == '\n') {
                            is_previous_newline = True;
                            file_struct->data[symbol_index] = '\0';
                        }
                        
                        ++symbol_index;
                    }

                    file_struct->file_length = file_length;
                    file_struct->line_count = line_count;
                }

                if (file)
                    fclose(file);
            }
        }
    }
}

void free_file_struct(FileStruct* file_struct) {
    if (file_struct->data) {
        free(file_struct->data);
        file_struct->data = NULL;
    }
    
    if (file_struct->line) {
        free(file_struct->line);
        file_struct->line = NULL;
    }

    file_struct->file_length = -1;
    file_struct->line_count = -1;
}

void set_list_of_regexes_allocate(FileStruct* giant_file_struct, const Arguments* arguments_struct) {
    int total_line_count = 0;
    int total_character_count = 0;

    for (int index = 0; index < arguments_struct->counter; ++index) {
        
        if (arguments_struct->type[index] == REGEX_FILENAME_T) {
            
            const char* filename = arguments_struct->word[index];
            total_line_count += get_file_line_count(filename);
            total_character_count += get_file_character_count(filename);

        }

    }

    int success = True;

    giant_file_struct->data = malloc(total_character_count * sizeof(char));

    if (!giant_file_struct->data)
        success = False;

    if (success) {

        giant_file_struct->line = malloc(total_line_count * sizeof(char*));
        if (!giant_file_struct->line)
            success = False;
        
        giant_file_struct->line[0] = giant_file_struct->data;
        giant_file_struct->file_length = total_character_count;
        giant_file_struct->line_count = total_line_count;

        int line_index = 0;
        ++line_index;
        int character_shift = 0;

        if (success) {

            for (int filename_index = 0; filename_index < arguments_struct->counter; ++filename_index) {
                
                if (arguments_struct->type[filename_index] == REGEX_FILENAME_T) {

                    const char* filename = arguments_struct->word[filename_index];

                    FileStruct file_struct;
                    initialize_file_struct(&file_struct);
                    set_struct_from_file_allocate(filename, &file_struct);

                    for (int character_index = 0; character_index < file_struct.file_length; ++character_index) {
                        const char symbol = file_struct.data[character_index];

                        assert(character_index + character_shift < total_character_count);
                        giant_file_struct->data[character_index + character_shift] = symbol;
                
                        if (!symbol && line_index < total_line_count) {
                            assert(character_index + character_shift + 1 < total_character_count);
                            assert(line_index < total_line_count);
                            giant_file_struct->line[line_index] = giant_file_struct->data + character_index + character_shift + 1;
                            ++line_index;
                        }
                    }

                    character_shift += file_struct.file_length;
                    free_file_struct(&file_struct);
                }

            }
        }
    
    }

    if (!success)
        free_file_struct(giant_file_struct);
    
}

void print_arguments_struct(const Arguments* arguments_struct) {
    printf("Begin of parsed arguments:\n");
    for (int index = 1; index < arguments_struct->counter; ++index) {
        if (arguments_struct->type[index] == ONLY_FLAG_T)
            printf("%d - flag:\t\t%s\n", index, arguments_struct->word[index]);
        else if (arguments_struct->type[index] == PATTERN_T) 
            printf("%d - pattern:\t\t%s\n", index, arguments_struct->word[index]);
        else if (arguments_struct->type[index] == FILENAME_T)
            printf("%d - filename:\t\t%s\n", index, arguments_struct->word[index]);
        else if (arguments_struct->type[index] == REGEX_FILENAME_T)
            printf("%d - regex filename:\t%s\n", index, arguments_struct->word[index]);
    }
    printf("End of parsed arguments:\n");
}

int main(int counter, const char **arguments) {

#ifdef _DEBUG
    print_command_line_arguments(counter, arguments);
#endif

    Flags flags;
    initialize_flags(&flags);

    Arguments arguments_struct;
    initialize_arguments_struct_allocate(&arguments_struct, counter, arguments);

    const int number_of_files = parse(counter, arguments, &flags, &arguments_struct);

#ifdef _DEBUG
    print_arguments_struct(&arguments_struct);
#endif

    FileStruct all_regexes;
    initialize_file_struct(&all_regexes);

    set_list_of_regexes_allocate(&all_regexes, &arguments_struct);

#ifdef _DEBUG
    printf("\n\n\n\n\n");
#endif

    int file_number = 0;
    for (int argument_index = 1; argument_index < arguments_struct.counter; ++argument_index) {
        if (arguments_struct.type[argument_index] == FILENAME_T) {
#ifdef _DEBUG
            printf("%d) file to be opened: %s\n", argument_index, arguments_struct.word[argument_index]);
#endif

            ++file_number;
            const int is_last_file = (file_number == number_of_files);
            read_and_output_file_line_by_line(arguments_struct.word[argument_index], is_last_file, &flags, &arguments_struct, &all_regexes);
        }
    }

    free_file_struct(&all_regexes);
    free_arguments_struct(&arguments_struct);
    return 0;
}
