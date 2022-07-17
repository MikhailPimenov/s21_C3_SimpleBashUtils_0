#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define UNUSED_SHIT(fucking_unused_shit) do { (void)(fucking_unused_shit); } while (0)

enum Boolean {
    false,
    true
};

enum Flags {
    b_flag = (1 << 0),
    e_flag = (1 << 1),
    n_flag = (1 << 2),
    s_flag = (1 << 3),
    t_flag = (1 << 4),
} Flags; 


// int get_number_of_flags(int first_flag_index, const char** arguments) {
//     //int number_of_flags = 0;
//     //
//     //while (true) {
//     //    
//     //}
//     return -1;
// }

// void pick_flags(int first_flag_index, const char **arguments, int* flags) {
//     const int number_of_flags = get_number_of_flags(first_flag_index, arguments);
//     for (int index = first_flag_index; number_of_flags; ++index) {
//         ;
//     }
//     *flags = 0;
// }

void read_and_output_file_line_by_line(const char* filename) {
    FILE* input_file = fopen(filename, "r");
    if (input_file == NULL) {
        exit(-1);
    }

    const size_t line_size = 300;
    char* line = malloc(line_size);
    
    if (!line)
        exit(-1);

    while (fgets(line, line_size, input_file) != NULL)  {
        printf("%s", line);
    }

    free(line);             // dont forget to free heap memory
    fclose(input_file);
}

void print_command_line_arguments(int counter, const char** arguments) {
    for (int index = 0; index < counter; ++index) {
        printf("%d - %s\n", index, arguments[index]);
    }
}
int main(int counter, const char **arguments) {
    // int flags = 0;

    // pick_flags(2, arguments, &flags);
    print_command_line_arguments(counter, arguments);

    read_and_output_file_line_by_line(arguments[1]);
    UNUSED_SHIT(counter);

    
    return -1;
}