// Group 26 Members:
// Magdalena Veleva
// Frazer Sandison
// Rachael Espie
// Aleksandra Kliaugaite
// Bruno Romanski
// Piotr Cichon

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "prompt.h"

int main() {

    // initialise state variables, get current PATH
    // save path and set cwd to users home directory
    char *env = getenv("PATH");
    chdir(getenv("HOME"));
    History history;
    history.counter = 0;
    history.is_full = -1;
    char *aliases[ALIAS_MAX][2] = {0};

    //load history and aliases
    load_history(&history);
    load_aliases(aliases);

    while (1) {
        char *input = malloc(sizeof(char) * INPUT_SIZE);
        memset(input, 0, sizeof(char) * INPUT_SIZE);
        // empty stdin, clear input and print the prompt
        print_prompt();
        fflush(stdout);
        read(STDIN_FILENO, input, INPUT_SIZE);

        // sanitize the input - remove leading whitespace, trailing new line
        // and check for correct size of input
        char *sanitized_input = sanitize_input(input);
        if (strcmp(sanitized_input, "") == 0) {
            continue;
        }
        // check for exit
        if (strncmp(sanitized_input, "exit", 4) == 0) {
            break;
        }

        char *hist_subs_input = substitute_from_history(history, sanitized_input);
        if (hist_subs_input == NULL) {
            continue;
        }

        int belongs_to_history = 1;
        int counter = 0;
        char *alias_sub_input = substitute_from_aliases(aliases, hist_subs_input);
        // history substitution took place
        if (strcmp(sanitized_input, "history") == 0 || strncmp(sanitized_input, "!", 1) == 0 ||
            strcmp(alias_sub_input, "history") == 0 || strncmp(alias_sub_input, "!", 1) == 0) {
            belongs_to_history = 0;
        }
        while (strcmp(hist_subs_input, alias_sub_input) != 0 && counter <= 3) {
            hist_subs_input = substitute_from_history(history, alias_sub_input);
            if (hist_subs_input == NULL) {
                break;
            }
            alias_sub_input = substitute_from_aliases(aliases, hist_subs_input);
            counter++;
            if (strcmp(alias_sub_input, "history") == 0 || strncmp(alias_sub_input, "!", 1) == 0) {
                belongs_to_history = 0;
            }
        }
        if (hist_subs_input == NULL) {
            continue;
        }

        if (belongs_to_history == 1) {
            store_in_history(&history, sanitized_input);
        }
        // extract tokens from the input to be passed to exec
        char **tokens = tokenize_input(alias_sub_input);
        if (tokens == NULL) {
            continue;
        }

        // execute the command or fork and execute the program
        int command_index;
        if ((command_index = is_command(tokens[0])) != -1) {
            exec_command(command_index, tokens, history, aliases);
        } else {
            fork_process(tokens);
        }

        memset(tokens, 0, sizeof(char *) * TOKENS_SIZE);
    }

    save_history(history);
    save_aliases(aliases);
    // restore path
    setenv("PATH", env, 1);
    printf("%s\n", getenv("PATH"));
    return 0;
}
