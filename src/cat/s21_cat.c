#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define UNUSED_SHIT(fucking_unused_shit) do { (void)(fucking_unused_shit); } while (0)

enum Boolean {
    False,
    True
};

typedef struct Flags {
    char b;
    char e;
    char E;
    char n;
    char s;
    char t;
    char T;
    char v;
} Flags;

void initialize_flags(Flags *flags) {
    flags->b = False;
    flags->e = False;
    flags->E = False;
    flags->n = False;
    flags->s = False;
    flags->t = False;
    flags->T = False;
    flags->v = False;
}


#include <errno.h>
#include <stdint.h>

// if typedef doesn't exist (msvc, blah)
typedef intptr_t ssize_t;

ssize_t my_getline_allocate(char **line, size_t *allocated_size, FILE *stream) {
    size_t pos;
    int c;

    if (line == NULL || stream == NULL || allocated_size == NULL) {
        errno = EINVAL;
        return -1;
    }

    c = getc(stream);
    if (c == EOF) {
        return -1;
    }

    if (*line == NULL) {
        *line = malloc(128);
        if (*line == NULL) {
            return -1;
        }
        *allocated_size = 128;
    }

    pos = 0;
    while(c != EOF) {
        if (pos + 1 >= *allocated_size) {
            size_t new_size = *allocated_size + (*allocated_size >> 2);
            if (new_size < 128) {
                new_size = 128;
            }
            char *new_ptr = realloc(*line, new_size);
            if (new_ptr == NULL) {
                return -1;
            }
            *allocated_size = new_size;
            *line = new_ptr;
        }

        ((unsigned char *)(*line))[pos ++] = c;
        if (c == '\n') {
            break;
        }
        c = getc(stream);
    }

    (*line)[pos] = '\0';
    return pos;
}


int is_tab(char symbol) {
    return symbol == '\t';
}

int is_end_of_line(char symbol) {
    return symbol == '\n';
}

void print_line_number(int number) {
    printf("%6d\t", number);                                                                                //  width for line number is 6
}

void print_with_unprintable(char symbol) {
    if (0 <= symbol && symbol <= 31 && !is_tab(symbol) && !is_end_of_line(symbol)) {
    
        symbol += 64;
        printf("^%c", symbol);
    
    } else if (symbol == 127) {
    
        symbol = '?';
        printf("^%c", symbol);
    
    } else {
    
        printf("%c", symbol);
    
    }
}

void print_line(int *line_number, const char* line, int length, const Flags* flags, int* is_empty) {
    if (flags->s && *is_empty && length <= 1)
        return;

    if (length <= 1)
        *is_empty = True;
    else 
        *is_empty = False;

    if (flags->b) {
        if (length > 1) {

            print_line_number(*line_number);
            ++(*line_number);

        }
    } else if (flags->n) {

        print_line_number(*line_number);
        ++(*line_number);

    }

    for (int index = 0; index < length; ++index) {

        if (flags->t && is_tab(line[index])) {
        
                printf("%s", "^I");

        } else if (flags->T && is_tab(line[index])) {

            printf("%s", "^I");

        } else if (flags->e && is_end_of_line(line[index])) {

            printf("%c", '$');
            printf("%c", '\n');

        } else if (flags->E && is_end_of_line(line[index])) {

            printf("%c", '$'); 
            printf("%c", '\n');

        } else if (flags->v || flags->t || flags->e) {

            print_with_unprintable(line[index]);

        } else {

            printf("%c", line[index]);

        }
    }
}

int get_line_length(const char* line) {    
    int length = 0;
    while (line && line[length] && line[length] != '\n')
        ++length;
    ++length;

    return length;
}

void read_and_output_file_line_by_line(const char* filename, const Flags* flags) {
    FILE* input_file = fopen(filename, "r");
    if (input_file == NULL) {
        fprintf(stderr, "cat: %s: No such file or directory\n", filename);
        return;
    }

    ssize_t line_actual_length = 0ul;
    size_t line_allocated_length = 0l;
    char *line = NULL;


    int line_number = 1;                                                                                    //  first line has number '1'
    int is_line_empty = 0;
    while (True)  {                                                                                         
        line_actual_length = my_getline_allocate(&line, &line_allocated_length, input_file);                         
  
        if (line_actual_length == EOF) {
            break;
        }

        print_line(&line_number, line, line_actual_length, flags, &is_line_empty);
    }

    free(line);                                                                                             //  because getline allocates memory
    fclose(input_file);
}

int are_equal(const char* string1, const char* string2, int length) {
    for (int index = 0; index < length; ++index)
        if (string1[index] != string2[index])
            return False;
    return True;
}

int is_found(const char* substring, int sublength, const char* string, int length) {
    for (int index = 0; index < length - sublength; ++index)
        if (are_equal(substring, string + index, length))
            return True;
    return False;
}


void print_command_line_arguments(int counter, const char** arguments) {
    for (int index = 0; index < counter; ++index) {
        printf("%d - %s\n", index, arguments[index]);
    }
}

int get_string_length(const char* string) {
    int length = 0;
    while (string && string[length])
        ++length;
    return length;
}

void set_flags(int counter, const char** arguments, Flags* flags, int* flag_counter) {
    static const int short_flag_length = 2;

    for (int argument_index = 0; argument_index < counter; ++argument_index) {
    
        const int argument_length = get_string_length(arguments[argument_index]);
    
        if ((argument_length == short_flag_length && 
             are_equal("-b", arguments[argument_index], argument_length)) || 
            (argument_length == get_string_length("--number-nonblank") && 
             are_equal("--number-nonblank", arguments[argument_index], argument_length))) {

            flags->b = True;
            ++(*flag_counter);

        } else if (argument_length == short_flag_length && 
                   are_equal("-e", arguments[argument_index], argument_length)) {

            flags->e = True;
            ++(*flag_counter);

        } else if ((argument_length == short_flag_length && 
                    are_equal("-n", arguments[argument_index], argument_length)) || 
                   (argument_length == get_string_length("--number") && 
                    are_equal("--number", arguments[argument_index], argument_length))) {

            flags->n = True;
            ++(*flag_counter);

        } else if ((argument_length == short_flag_length && 
                    are_equal("-s", arguments[argument_index], argument_length)) || 
                   (argument_length == get_string_length("--squeeze-blank") && 
                    are_equal("--squeeze-blank", arguments[argument_index], argument_length))) {

            flags->s = True;
            ++(*flag_counter);

        } else if (argument_length == short_flag_length && 
                   are_equal("-t", arguments[argument_index], argument_length)) {

            flags->t = True;
            ++(*flag_counter);

        } else if (argument_length == short_flag_length && 
                   are_equal("-E", arguments[argument_index], argument_length)) {

            flags->E = True;
            ++(*flag_counter);

        } else if (argument_length == short_flag_length && 
                   are_equal("-T", arguments[argument_index], argument_length)) {

            flags->T = True;
            ++(*flag_counter);

        } else if (argument_length == short_flag_length && 
                   are_equal("-v", arguments[argument_index], argument_length)) {

            flags->v = True;
            ++(*flag_counter);

        }
    }
}

int main(int counter, const char **arguments) {
    // print_command_line_arguments(counter, arguments);

    Flags flags;
    initialize_flags(&flags);
    int flag_counter = 0;
    set_flags(counter, arguments, &flags, &flag_counter);

    for (int file_index = flag_counter + 1; file_index < counter; ++file_index) {
        read_and_output_file_line_by_line(arguments[file_index], &flags);
    }

    return 0;
} // last non-empty line
