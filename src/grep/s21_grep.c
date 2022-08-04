#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

#define UNUSED_SHIT(fucking_unused_shit) do { (void)(fucking_unused_shit); } while (0)

regex_t regex;  //  TODO: initialize it
int reti = 0;
char msgbuf[100] = {'\0'};

enum Boolean {
    False,
    True
};

typedef struct RegexesFilenames {
    const char** words;
    int* indices;
    int counter;

    // const char** regexes;
} RegexesFilenames;

typedef struct Patterns {
    const char** words;                                                                                //  a.k.a. command line arguments
    int *indices;
    int counter;
} Patterns;

typedef struct Filenames {
    const char** words;
    int* indices;
    int counter;
} Filenames;

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

void print_line_number(int number) {
    printf("%d:", number);                                                                                //  width for line number is 6
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

void print_for_o(const Line* line, const Flags* flags, const char* matched_pattern, int* is_beginning_of_the_line) {
    if (*is_beginning_of_the_line) {

        if (flags->print_filename && !flags->h)
            printf("%s:", line->filename);

        if (flags->n)
            print_line_number(line->line_number);

        *is_beginning_of_the_line = False;
    }

    printf("%s\n", matched_pattern);
}


enum TypeToSeek {
    NO_TYPE_T = -1,
    PATTERN_T = 0,
    REGEX_T = 1
};

typedef struct RegexLine {
    char* word;
    int length;
    int type;
} RegexLine;

typedef struct RegexList {
    RegexLine* line;    
    int length;
} RegexList;

void initialize_regex_line(RegexLine* regex_line) {
    regex_line->word = NULL;
    regex_line->length = -1;
    regex_line->type = NO_TYPE_T;
}

void initialize_regex_list(RegexList* regex_list) {
    regex_list->line = NULL;
    regex_list->length = -1;
}

int is_line_suitable2(Line* line, const Flags* flags, const Patterns* patterns) {
    int is_suitable = False;

    int is_beginning_of_the_line = True;

    int index_from_previous_pattern = 0;

    for (int pattern_number = 0; pattern_number < patterns->counter; ++pattern_number) {

        const char* pattern = patterns->words[patterns->indices[pattern_number]];
        const int pattern_length = get_string_length(pattern);

        for (int index = index_from_previous_pattern; index < line->length; ++index) {

            if (index + pattern_length < line->length) {

                if (are_equal(line->line + index, pattern, pattern_length, flags->i)) {

                    index_from_previous_pattern = index;

                    is_suitable = True;

                    if (!flags->o || flags->v || flags->c) {
                        break;
                    }

                    print_for_o(line, flags, pattern, &is_beginning_of_the_line);
                }
            }  
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


void read_and_output_file_line_by_line(const char* filename, const Flags* flags, const Patterns* patterns) {
    FILE* input_file = fopen(filename, "r");
    if (input_file == NULL) {
        if (!flags->s)  
            printf("Failed to open file %s\n", filename);
        return;
    }

    ssize_t line_actual_length = 0ul;
    // int line_actual_length = 0;
    size_t line_allocated_length = 0l;
    char *line_for_getline = NULL;
    // static const int max_line_length = 500;
    // char *line = malloc(max_line_length * sizeof(char));                                                    //  can be replaced with static array

    int line_number = 1;                                                                                    //  first line has number '1'
    
    
        
    int is_file_suitable = False;
    int suitable_line_counter = 0;
    while (True)  {                                                                                         //  getline allocates memory

        line_actual_length = getline(&line_for_getline, &line_allocated_length, input_file);
        
        // printf("read_file_and_output...: %s\n", line);
        // const char* result = fgets(line, max_line_length, input_file);
  
        if (line_actual_length == EOF) {
        // if (!result) {
            break;
        }
        // line_actual_length = get_line_length(line);

        Line line;
        initialize_line(&line);

        line.line = line_for_getline;
        line.length = line_actual_length;
        line.line_number = line_number;
        line.filename = filename;

        // if (is_line_suitable(line_for_getline, line_actual_length, flags, patterns, filename)) {
        if (is_line_suitable2(&line, flags, patterns)) {
            ++suitable_line_counter;
            if (flags->l) {
                is_file_suitable = True;
                break;
            }

            if ((!flags->o || flags->v) && !flags->c)
                // print_line(line_number, line, line_actual_length, flags, filename);
                print_line(&line, flags);
            // print_line();
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



void parse(int counter, const char** arguments, Flags* flags, Patterns* patterns, Filenames* filenames, RegexesFilenames* regexes) {
    static const int short_flag_length = 2;

    int pattern_index = 0;
    int filename_index = 0;
    int regex_filename_index = 0;
    for (int argument_index = 1; argument_index < counter; ++argument_index) {
    
        const int argument_length = get_string_length(arguments[argument_index]);
    
        if (argument_length == short_flag_length && 
            are_equal("-e", arguments[argument_index], argument_length, False)) {

            flags->e = True;

            printf("pattern #%d: %s\n", argument_index + 1, arguments[argument_index + 1]);
            patterns->indices[pattern_index] = argument_index + 1;  //  because pattern goes right after -e, so its index is +1
            printf("pattern #%d: %s\n", patterns->indices[pattern_index], patterns->words[patterns->indices[pattern_index]]);
            ++pattern_index;
            ++(patterns->counter);

            ++argument_index;

        } else if (argument_length == short_flag_length && 
                   are_equal("-i", arguments[argument_index], argument_length, False)) {

            flags->i = True;

        } else if (argument_length == short_flag_length && 
                   are_equal("-v", arguments[argument_index], argument_length, False)) {

            flags->v = True;
        
        } else if (argument_length == short_flag_length && 
                   are_equal("-c", arguments[argument_index], argument_length, False)) {

            flags->c = True;
        
        } else if (argument_length == short_flag_length && 
                   are_equal("-l", arguments[argument_index], argument_length, False)) {

            flags->l = True;
        
        } else if (argument_length == short_flag_length && 
                   are_equal("-n", arguments[argument_index], argument_length, False)) {

            flags->n = True;
        
        } else if (argument_length == short_flag_length && 
                   are_equal("-h", arguments[argument_index], argument_length, False)) {

            flags->h = True;
        
        } else if (argument_length == short_flag_length && 
                   are_equal("-s", arguments[argument_index], argument_length, False)) {

            flags->s = True;
        
        } else if (argument_length == short_flag_length && 
                   are_equal("-f", arguments[argument_index], argument_length, False)) {

            flags->f = True;

            printf("regex file #%d: %s\n", argument_index + 1, arguments[argument_index + 1]);
            regexes->indices[regex_filename_index] = argument_index + 1;  //  because regex filename goes right after -f, so its index is +1
            printf("regex file #%d: %s\n", regexes->indices[regex_filename_index], regexes->words[regexes->indices[regex_filename_index]]);
            ++regex_filename_index;
            ++(regexes->counter);

            ++argument_index;

            //  assert(0 && "NOT IMPLEMENTED REGEX!");
            //  TODO: store somewhere filename for regex!!!


        } else if (argument_length == short_flag_length && 
                   are_equal("-o", arguments[argument_index], argument_length, False)) {

            flags->o = True;
        
        } else {
            printf("filename #%d: %s\n", argument_index, arguments[argument_index]);
            filenames->indices[filename_index] = argument_index;
            printf("filename #%d: %s\n", filenames->indices[filename_index], filenames->words[filenames->indices[filename_index]]);
            ++filename_index;
            ++(filenames->counter);
        }
    }

    if (filenames->counter > 1)
        flags->print_filename = True;

    UNUSED_SHIT(regexes);
}

void initialize_patterns(Patterns* patterns) {
    patterns->words = NULL;
    patterns->indices = NULL;
    patterns->counter = 0;
}

void initialize_filenames(Filenames* filenames) {
    filenames->words = NULL;
    filenames->indices = NULL;
    filenames->counter = 0;
}

void initialize_regexes(RegexesFilenames* regexes) {
    regexes->words = NULL;
    regexes->indices = NULL;
    regexes->counter = 0;

    // regexes->regexes = NULL;
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


typedef struct File {
    char* data;
    int file_length;

    char** line;
    int line_count;
} File;

void initialize_file_struct(File* file_struct) {
    file_struct->data = NULL;
    file_struct->file_length = -1;

    file_struct->line = NULL;
    file_struct->line_count = -1;
}

void set_struct_from_file_allocate(const char* filename, File* file_struct) {
 
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

void free_file_struct(File* file_struct) {
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

void print_file_struct(const File* file_struct) {
    printf("begin:\n");

    for (int line_index = 0; line_index < file_struct->line_count; ++line_index)
        printf("%s\n", file_struct->line[line_index]);

    printf("end:\n");
}


void set_list_of_regexes_allocate(File* giant_file_struct, const RegexesFilenames* regexes_filenames) {
    int total_line_count = 0;
    int total_character_count = 0;
    for (int filename_index = 0; filename_index < regexes_filenames->counter; ++filename_index) {
        const char* filename = regexes_filenames->words[regexes_filenames->indices[filename_index]];

        total_line_count += get_file_line_count(filename);
        total_character_count += get_file_character_count(filename);
    }

    int success = True;

    giant_file_struct->data = malloc(total_character_count * sizeof(char));

    if (!giant_file_struct->data)
        success = False;

    if (success) {

        giant_file_struct->line = malloc(total_line_count * sizeof(char));
        if (!giant_file_struct->line)
            success = False;
        
        giant_file_struct->line[0] = giant_file_struct->data;
        giant_file_struct->file_length = total_character_count;
        giant_file_struct->line_count = total_line_count;

        int line_index = 0;
        ++line_index;
        int character_shift = 0;



        if (success) {

            for (int filename_index = 0; filename_index < regexes_filenames->counter; ++filename_index) {
                
                const char* filename = regexes_filenames->words[regexes_filenames->indices[filename_index]];

                File file_struct;
                initialize_file_struct(&file_struct);
                set_struct_from_file_allocate(filename, &file_struct);
                // print_file_struct(&file_struct);


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

    if (!success)
        free_file_struct(giant_file_struct);
    
}

int main(int counter, const char **arguments) {
    // File file_struct;
    // initialize_file_struct(&file_struct);

    // set_struct_from_file_allocate("s21_grep.c", &file_struct);
    // // //set_struct_from_file_allocate("regex_patterns2.txt", &file_struct);
    // print_file_struct(&file_struct);
    // free_file_struct(&file_struct);
    // return -14/88;


    // const int length = get_file_character_count("regex_patterns.txt");
    // const int line_count = get_file_line_count("regex_patterns.txt");
    // printf("length = %d\nlines = %d\n", length, line_count);

    // print_command_line_arguments(counter, arguments);

    // int number_in_list = 0;

    Flags flags;
    initialize_flags(&flags);

    Patterns patterns;
    initialize_patterns(&patterns);
    patterns.indices = malloc(counter * sizeof(int));
    if (!patterns.indices) {
        printf("Failed to allocate memory for pattern indices!\n");
        return -1;
    }
    patterns.words = arguments;

    Filenames filenames;
    initialize_filenames(&filenames);
    filenames.indices = malloc(counter * sizeof(int));
    if (!filenames.indices) {
        printf("Failed to allocate memory for filename indices!\n");
        free(patterns.indices);
        return -1;
    }
    filenames.words = arguments;


    RegexesFilenames regexes_filenames;
    initialize_regexes(&regexes_filenames);
    regexes_filenames.indices = malloc(counter * sizeof(int));
    if (!regexes_filenames.indices) {
        printf("Failed to allocate memory for regex filename indices!\n");
        free(patterns.indices);
        free(filenames.indices);
        return -1;
    }
    regexes_filenames.words = arguments;

    parse(counter, arguments, &flags, &patterns, &filenames, &regexes_filenames);

    // TODO: STRUCT TO STORE INDICES FOR FILENAMES

    printf("\n\n\n\n\n");

    File all_regexes;
    initialize_file_struct(&all_regexes);

    set_list_of_regexes_allocate(&all_regexes, &regexes_filenames);
    // print_file_struct(&all_regexes);

    for (int filename_index = 0; filename_index < filenames.counter; ++filename_index) {
        printf("%d) file to be opened: %s\n", filename_index, arguments[filenames.indices[filename_index]]);
        read_and_output_file_line_by_line(arguments[filenames.indices[filename_index]], &flags, &patterns);
    }


    free(regexes_filenames.indices);
    free(patterns.indices);
    free(filenames.indices);
    return 0;
} // last non-empty line