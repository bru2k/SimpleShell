//
// Created by Peter on 29/01/2021.
//
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "prompt.h"
#include <ctype.h>


int SIZE = 512;
char DELIMITERS[] = " \t|><&;";
char PROMPT[] = "==>";

char* new_input() {
    // allocates memory to input
    return malloc(sizeof(char)*SIZE);
}

void warn_user(char* warning){
    // print an Error with warning as a message
    printf("ERROR! - %s\n", warning);
}

void print_prompt() {
    printf("%s", PROMPT);
}

char* get_input(char* input){
    // stores typed commands from stdin to input
    return fgets(input, SIZE, stdin);
}

int ctrl_d_typed() {
    //  returns 1 if ctrl+d was pressed
    return feof(stdin) != 0;

}

int input_is_empty(char* input) {
    // checks for empty input
    return strlen(input)-1 == 0;
}

int input_too_large(char* input) {
    // if the string has not exceed the maximum length (SIZE-2 characters to
    // accomodate for \0 and \n) the last character should be the \n
    //
    // returns 1 if input is tool large (more than size-2) and 0 otherwise

    return input[strlen(input)-1] != '\n';
}

void clear_stdin() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}


void remove_trailing_new_line(char* string) {
    // removes last newline character in passed string
//    string[strlen(string)-1] = '\0';
    char *new = strchr( string, '\n' );
    if (new)
        *new = 0;
}

void remove_leading_whitespace(char** input) {
    // here so that string such as "  " can be typed without segfault
    while (**input == ' ')
        (*input)++;
}

int check_for_exit(char* tokens) {
    // returns 1 if exit was "requested"
   return (strcmp(&tokens[0], "exit") == 0 ||
           strcmp(&tokens[0], "EXIT") == 0);
}

void process_tokens(char* tokens) {
    // prints tokens for now

    print_tokens(tokens);
}

void print_tokens(char* tokens){
    // prints tokens on terminal
    while (tokens) {
        printf("%s\n", tokens);
        tokens = strtok(0, DELIMITERS);
    }
}

char* get_tokens(char* input) {
    // returns the tokens of the string
    remove_trailing_new_line(input);
    return strtok(input, DELIMITERS);
}

int fork_process(char* input){
    printf("This works");
    pid_t pid;
    pid = fork();

    if(pid < 0){
        printf(stderr, "Fork Failed");
        return 1;
    }
    else if (pid == 0){
        execlp();
    }
    else{
        wait(NULL);
        printf("Child Complete");
    }
    return 0;
}


