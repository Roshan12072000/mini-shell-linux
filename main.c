/*builtin commands*/



#include "main.h"
char external[200][51];
char *builtins[] = {"echo", "printf", "read", "cd", "pwd", "pushd", "popd", "dirs", "let", "eval",
    "set", "unset", "export", "declare", "typeset", "readonly", "getopts", "source",
    "exit", "exec", "shopt", "caller", "true", "type", "hash", "bind", "help","fg","bg","jobs", NULL};
int main(){
    char prompt[100] = "minishell$";
    system("clear");
 extract_external_commands();
 scan_input(prompt);
return 0;
}
