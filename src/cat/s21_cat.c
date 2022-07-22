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
    char n;
    char s;
    char t;
} Flags;

void initialize_flags(Flags *flags) {
    flags->b = False;
    flags->e = False;
    flags->n = False;
    flags->s = False;
    flags->t = False;
}

// void read_and_output_file_line_by_line(const char* filename) {
//     FILE* input_file = fopen(filename, "r");
//     if (input_file == NULL) {
//         exit(-1);
//     }

//     const size_t line_size = 300;
//     char* line = malloc(line_size);
    
//     if (!line)
//         exit(-1);

//     while (fgets(line, line_size, input_file) != NULL)  {
//         printf("%s", line);
//     }

//     free(line);             // dont forget to free heap memory
//     fclose(input_file);
// }


void read_and_output_file_line_by_line(const char* filename, const Flags* flags) {
    FILE* input_file = fopen(filename, "r");
    if (input_file == NULL) {
        exit(-1);
    }

    ssize_t line_actual_length = 0ul;
    size_t line_allocated_length = 0l;
    char *line = NULL;

    int line_number = 1;
    while (True)  {                                                                                         //  getline allocates memory
        line_actual_length = getline(&line, &line_allocated_length, input_file);
        if (flags->n) {
            printf("%6d  %s", line_number, line);  //  number width is 6
            ++line_number;
        } else {
            printf("%s", line);
        }

        if (line_actual_length == EOF) {
            break;
        }
    }

    free(line);                                                                                             //  because getline allocates memory
    fclose(input_file);

    UNUSED_SHIT(line_actual_length);
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
    while (string[length])
        ++length;
    return length;
}

void set_flags(int counter, const char** arguments, Flags* flags, int* flag_counter) {
    static const int short_flag_length = 2;

    for (int argument_index = 0; argument_index < counter; ++argument_index) {
    
        const int argument_length = get_string_length(arguments[argument_index]);
    
        if (argument_length == short_flag_length && are_equal("-b", arguments[argument_index], argument_length)) {
            flags->b = True;
            ++(*flag_counter);
        } else if (argument_length == short_flag_length && are_equal("-e", arguments[argument_index], argument_length)) {
            flags->e = True;
            ++(*flag_counter);
        } else if (argument_length == short_flag_length && are_equal("-n", arguments[argument_index], argument_length)) {
            flags->n = True;
            ++(*flag_counter);
        } else if (argument_length == short_flag_length && are_equal("-s", arguments[argument_index], argument_length)) {
            flags->s = True;
            ++(*flag_counter);
        } else if (argument_length == short_flag_length && are_equal("-t", arguments[argument_index], argument_length)) {   
            flags->t = True;
            ++(*flag_counter);
        }
    }
}
int main(int counter, const char **arguments) {
    // int flags = 0;

    // pick_flags(2, arguments, &flags);
    print_command_line_arguments(counter, arguments);

    Flags flags;
    initialize_flags(&flags);
    int flag_counter = 0;
    set_flags(counter - 1, arguments + 1, &flags, &flag_counter);

    read_and_output_file_line_by_line(arguments[1 + flag_counter], &flags);
    UNUSED_SHIT(counter);





    return -1;
} // last non-empty line