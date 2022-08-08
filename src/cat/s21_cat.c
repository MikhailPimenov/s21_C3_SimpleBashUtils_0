#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "../common/my_getline.h"

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

int is_tab(char symbol) {
    return symbol == '\t';
}

int is_end_of_line(char symbol) {
    return symbol == '\n';
}

void print_line_number(int number) {
    printf("%6d\t", number);                    //  width for line number is 6
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

// int get_line_length(const char* line) {
//     int length = 0;
//     while (line && line[length] && line[length] != '\n')
//         ++length;
//     ++length;

//     return length;
// }

void read_and_output_file_line_by_line(const char* filename, const Flags* flags) {
    FILE* input_file = fopen(filename, "r");
    if (input_file == NULL) {
        fprintf(stderr, "cat: %s: No such file or directory\n", filename);
        return;
    }

    char *line = NULL;


    int line_number = 1;                 //  first line has number '1'
    int is_line_empty = 0;
    while (True) {
        ssize_t line_actual_length = 0ul;
        size_t line_allocated_length = 0l;
        line_actual_length = my_getline_allocate(&line, &line_allocated_length, input_file);

        if (line_actual_length == EOF) {
            break;
        }

        print_line(&line_number, line, line_actual_length, flags, &is_line_empty);
    }

    free(line);                         //  because getline allocates memory
    fclose(input_file);
}

int are_equal(const char* string1, const char* string2, int length) {
    for (int index = 0; index < length; ++index)
        if (string1[index] != string2[index])
            return False;
    return True;
}

// void print_command_line_arguments(int counter, const char** arguments) {
//     for (int index = 0; index < counter; ++index) {
//         printf("%d - %s\n", index, arguments[index]);
//     }
// }

int get_string_length(const char* string) {
    int length = 0;
    while (string && string[length])
        ++length;
    return length;
}

int set_flags(int counter, const char** arguments, Flags* flags, int* flag_counter) {
    static const int short_flag_length = 2;

    int result = True;
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
        } else if (argument_length >= 1 &&
                   arguments[argument_index][0] == '-') {
            fprintf(stderr, "cat: illegal option -- %s\n", arguments[argument_index] + 1);
            result = False;
        }
    }

    return result;
}

int main(int counter, const char **arguments) {
    //  print_command_line_arguments(counter, arguments);

    Flags flags;
    initialize_flags(&flags);
    int flag_counter = 0;
    const int status = set_flags(counter, arguments, &flags, &flag_counter);

    if (status) {
        for (int file_index = flag_counter + 1; file_index < counter; ++file_index) {
            read_and_output_file_line_by_line(arguments[file_index], &flags);
        }
    }

    return 0;
}
