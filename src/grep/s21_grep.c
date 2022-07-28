#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define UNUSED_SHIT(fucking_unused_shit) do { (void)(fucking_unused_shit); } while (0)

enum Boolean {
    False,
    True
};

typedef struct Patterns {
    const char** words;                                                                                //  a.k.a. command line arguments
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

    char print_filename;
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

    flags->print_filename = False;
}

int is_tab(char symbol) {
    return symbol == '\t';
}

int is_end_of_file(char symbol) {
    return symbol == '\n';
}

void print_line_number(int number) {
    printf("%d:", number);                                                                                //  width for line number is 6
}

// int are_equal(const char* string1, const char* string2, int length) {
//     for (int index = 0; index < length; ++index)
//         if (string1[index] != string2[index])
//             return False;
//     return True;
// }

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

int is_found(const char* substring, int sublength, const char* string, int length, const Flags* flags) {
    for (int index = 0; index < length - sublength; ++index)
        if (are_equal(substring, string + index, sublength, flags->i))
            return True;
    return False;
}

void print_line(int line_number, const char* line, int length, const Flags* flags, int* is_empty, const char* filename) {
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
    
    if (flags->print_filename && !flags->h)
        printf("%s:", filename);

    if (flags->n)
        print_line_number(line_number);

    // if (flags->o)
    //     printf("%s", line);    
    
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

    UNUSED_SHIT(flags);
    UNUSED_SHIT(is_empty);
    UNUSED_SHIT(line_number);
}

int get_string_length(const char* string) {
    int length = 0;
    while (string[length])
        ++length;
    return length;
}

int is_line_suitable(const char* line, int line_length, const Flags* flags, const Patterns* patterns) {
    int is_suitable = False;
        
    for (int pattern_number = 0; pattern_number < patterns->counter; ++pattern_number) {
        const char* word = patterns->words[patterns->indices[pattern_number]];
        const int word_length = get_string_length(word);

        if (is_found(word, word_length, line, line_length, flags)) {
            is_suitable = True;
            break;
        }
    }


    UNUSED_SHIT(flags);

    if (flags->v)
        is_suitable = !is_suitable;

    return is_suitable;
}

void read_and_output_file_line_by_line(const char* filename, const Flags* flags, const Patterns* patterns) {
    FILE* input_file = fopen(filename, "r");
    if (input_file == NULL) {
        if (!flags->s)  
            printf("Failed to open file %s\n", filename);
        return;
    }

    // ssize_t line_actual_length = 0ul;
    int line_actual_length = 0ul;
    // size_t line_allocated_length = 0l;
    char *line = NULL;
    static const int max_line_length = 500;
    // char *line = malloc(max_line_length * sizeof(char));                                                    //  can be replaced with static array

    int line_number = 1;                                                                                    //  first line has number '1'
    int is_line_empty = False;
    int is_file_suitable = False;
    while (True)  {                                                                                         //  getline allocates memory
        // line_actual_length = getline(&line, &line_allocated_length, input_file);
        const char* result = fgets(line, max_line_length, input_file);
  
        // if (line_actual_length == EOF) {
        if (!result) {
            break;
        }
        // line_actual_length = get_line_length(line);
        if (is_line_suitable(line, line_actual_length, flags, patterns)) {
            
            if (flags->l) {
                is_file_suitable = True;
                break;
            }

            print_line(line_number, line, line_actual_length, flags, &is_line_empty, filename);
        }
        ++line_number;
    }

    free(line);                                                                                             //  because getline allocates memory
    fclose(input_file);

    if (flags->l && is_file_suitable)
        printf("%s\n", filename);

    UNUSED_SHIT(line_allocated_length);
    UNUSED_SHIT(line_number);
    UNUSED_SHIT(flags);
    UNUSED_SHIT(is_line_empty);
    UNUSED_SHIT(patterns);
}



void print_command_line_arguments(int counter, const char** arguments) {
    for (int index = 0; index < counter; ++index) {
        printf("%d - %s\n", index, arguments[index]);
    }
}



void set_flags(int counter, const char** arguments, Flags* flags, int* words_counter, Patterns* patterns) {
    static const int short_flag_length = 2;

    int pattern_index = 0;
    for (int argument_index = 0; argument_index < counter; ++argument_index) {
    
        const int argument_length = get_string_length(arguments[argument_index]);
    
        if (argument_length == short_flag_length && 
            are_equal("-e", arguments[argument_index], argument_length, False)) {

            flags->e = True;
            (*words_counter) += 2;

            printf("pattern #%d: %s\n", argument_index + 1, arguments[argument_index + 1]);
            patterns->indices[pattern_index] = argument_index + 1;  //  because pattern goes right after -e, so its index is +1
            printf("pattern #%d: %s\n", patterns->indices[pattern_index], patterns->words[patterns->indices[pattern_index] + 1]);
            ++pattern_index;
            ++(patterns->counter);


        } else if (argument_length == short_flag_length && 
                   are_equal("-i", arguments[argument_index], argument_length, False)) {

            flags->i = True;
            ++(*words_counter);

        } else if (argument_length == short_flag_length && 
                   are_equal("-v", arguments[argument_index], argument_length, False)) {

            flags->v = True;
            ++(*words_counter);
        
        } else if (argument_length == short_flag_length && 
                   are_equal("-c", arguments[argument_index], argument_length, False)) {

            flags->c = True;
            ++(*words_counter);
        
        } else if (argument_length == short_flag_length && 
                   are_equal("-l", arguments[argument_index], argument_length, False)) {

            flags->l = True;
            ++(*words_counter);
        
        } else if (argument_length == short_flag_length && 
                   are_equal("-n", arguments[argument_index], argument_length, False)) {

            flags->n = True;
            ++(*words_counter);
        
        } else if (argument_length == short_flag_length && 
                   are_equal("-h", arguments[argument_index], argument_length, False)) {

            flags->h = True;
            ++(*words_counter);
        
        } else if (argument_length == short_flag_length && 
                   are_equal("-s", arguments[argument_index], argument_length, False)) {

            flags->s = True;
            ++(*words_counter);
        
        } else if (argument_length == short_flag_length && 
                   are_equal("-f", arguments[argument_index], argument_length, False)) {

            flags->f = True;
            ++(*words_counter);

            assert(0 && "NOT IMPLEMENTED FILENAME STORAGE FOR REGEX!");
            //  TODO: store somewhere filename for regex!!!
        }
    }
}

void initialize_patterns(Patterns* patterns) {
    patterns->words = NULL;
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
    initialize_patterns(&patterns);
    patterns.indices = malloc(counter * sizeof(int));
    patterns.words = arguments;


    int flag_counter = 0;
    set_flags(counter, arguments, &flags, &flag_counter, &patterns);


    if (counter - (flag_counter + 1) > 1)  //  файлов больше одного
        flags.print_filename = True;


    // TODO: STRUCT TO STORE INDICES FOR FILENAMES


    for (int file_index = flag_counter + 1; file_index < counter; ++file_index) {
        printf("filename #%d: %s\n", file_index, arguments[file_index]);
        read_and_output_file_line_by_line(arguments[file_index], &flags, &patterns);
    }
    UNUSED_SHIT(counter);




    free(patterns.indices);
    return -1;
} // last non-empty line