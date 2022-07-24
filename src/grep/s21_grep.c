#include <assert.h
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define UNUSED_SHIT(fucking_unused_shit) do { (void)(fucking_unused_shit); } while (0)

enum Boolean {
    False,
    True
};

typedef struct Patterns {
    int *indices;
    int counter;
} Patterns;

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
} Flags;

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
}

int is_tab(char symbol) {
    return symbol == '\t';
}
int is_end_of_file(char symbol) {
    return symbol == '\n';
}

void print_line_number(int number) {
    printf("%6d  ", number);                                                                                //  width for line number is 6
}
void print_line(int *line_number, const char* line, int length, const Flags* flags, int* is_empty) {
    // if (flags->s && *is_empty && length <= 1)
    //     return;

    // if (length <= 1)
    //     *is_empty = True;
    // else 
    //     *is_empty = False;

    // if (flags->b) {
    //     if (length > 1) {
    //         print_line_number(*line_number);
    //         ++(*line_number);
    //     }
    // } else if (flags->n) {
    //     print_line_number(*line_number);
    //     ++(*line_number);
    // }
    
    for (int index = 0; index < length; ++index) {
        // if (flags->t && is_tab(line[index])) {
        //     printf("%s", "^I");
        // } else if (flags->e && is_end_of_file(line[index])) {
        //     printf("%c", '$');
        //     printf("%c", '\n');
        // } else {
            printf("%c", line[index]);
        // }
    }
}
void read_and_output_file_line_by_line(const char* filename, const Flags* flags, const Patterns* patterns) {
    FILE* input_file = fopen(filename, "r");
    if (input_file == NULL) {
        printf("Failed to open file %s\n", filename);
        return;
    }

    // ssize_t line_actual_length = 0ul;
    int line_actual_length = 0ul;
    size_t line_allocated_length = 0l;
    // char *line = NULL;
    static const int max_line_length = 500;
    char *line = malloc(max_line_length * sizeof(char));                                                    //  can be replaced with static array

    int line_number = 1;                                                                                    //  first line has number '1'
    int is_line_empty = 0;
    while (True)  {                                                                                         //  getline allocates memory
        // line_actual_length = getline(&line, &line_allocated_length, input_file);
        const char* result = fgets(line, max_line_length, input_file);
  
        // if (line_actual_length == EOF) {
        if (!result) {
            break;
        }
        line_actual_length = get_line_length(line);
        print_line(&line_number, line, line_actual_length, flags, &is_line_empty);
    }

    free(line);                                                                                             //  because getline allocates memory
    fclose(input_file);

    UNUSED_SHIT(line_allocated_length);
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

void set_flags(int counter, const char** arguments, Flags* flags, int* flag_counter, Patterns* patterns) {
    static const int short_flag_length = 2;

    int pattern_index = 0;
    for (int argument_index = 0; argument_index < counter; ++argument_index) {
    
        const int argument_length = get_string_length(arguments[argument_index]);
    
        if (argument_length == short_flag_length && 
            are_equal("-e", arguments[argument_index], argument_length)) {

            flags->e = True;
            ++(*flag_counter);

            patterns->indices[pattern_index] = argument_index + 1;  //  because pattern goes right after -e, so its index is +1
            ++pattern_index;
            ++(patterns->counter);

        } else if (argument_length == short_flag_length && 
                   are_equal("-i", arguments[argument_index], argument_length)) {

            flags->i = True;
            ++(*flag_counter);

        } else if (argument_length == short_flag_length && 
                   are_equal("-v", arguments[argument_index], argument_length)) {

            flags->v = True;
            ++(*flag_counter);
        
        } else if (argument_length == short_flag_length && 
                   are_equal("-c", arguments[argument_index], argument_length)) {

            flags->c = True;
            ++(*flag_counter);
        
        } else if (argument_length == short_flag_length && 
                   are_equal("-l", arguments[argument_index], argument_length)) {

            flags->l = True;
            ++(*flag_counter);
        
        } else if (argument_length == short_flag_length && 
                   are_equal("-n", arguments[argument_index], argument_length)) {

            flags->n = True;
            ++(*flag_counter);
        
        } else if (argument_length == short_flag_length && 
                   are_equal("-h", arguments[argument_index], argument_length)) {

            flags->h = True;
            ++(*flag_counter);
        
        } else if (argument_length == short_flag_length && 
                   are_equal("-s", arguments[argument_index], argument_length)) {

            flags->s = True;
            ++(*flag_counter);
        
        } else if (argument_length == short_flag_length && 
                   are_equal("-f", arguments[argument_index], argument_length)) {

            flags->f = True;
            ++(*flag_counter);

            assert(0 && "NOT IMPLEMENTED FILENAME STORAGE FOR REGEX!");
            //  TODO: store somewhere filename for regex!!!
        }
    }
}
void initialize_patterns(Patterns* patterns) {
    patterns->indices = NULL;
    patterns->counter = 0;
}
int main(int counter, const char **arguments) {
    // int flags = 0;

    // pick_flags(2, arguments, &flags);
    print_command_line_arguments(counter, arguments);

    Flags flags;
    initialize_flags(&flags);

    Patterns patterns;
    initialize_patternts(&patterns);
    patterns.indices = malloc(counter * sizeof(int));


    int flag_counter = 0;
    set_flags(counter - 1, arguments + 1, &flags, &flag_counter, &patterns);

    for (int file_index = flag_counter + 1; file_index < counter; ++file_index) {
        read_and_output_file_line_by_line(arguments[file_index], &flags, &patterns);
    }
    UNUSED_SHIT(counter);




    free(patterns.indices);
    return -1;
} // last non-empty line