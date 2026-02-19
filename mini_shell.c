#include "main.h"
// void add_job(pid_t pid,char *cmd);
pid_t c1, p1, pid;
char *cmd, str[100], tcmd[20][39];
int i;
int status;

Job *job_list = NULL;

void add_job(pid_t pid, const char *cmd)
{
    Job *new_job = (Job *)malloc(sizeof(Job));
    new_job->pid = pid;
    strncpy(new_job->cmd, cmd, sizeof(new_job->cmd) - 1);
    new_job->next = NULL;

    if (job_list == NULL)
    {
        job_list = new_job;
    }
    else
    {
        Job *current = job_list;
        while (current->next != NULL)
        {
            current = current->next;
        }
        current->next = new_job;
    }
}

void handler(int num)
{
    if (num == SIGINT)
    {
        if (pid > 0)
        {
            kill(pid, SIGINT);
        }
        else
        {
            printf("\nminishell$ ");
            fflush(stdout);
        }
    }
    else if (num == SIGCHLD )
    {
        // it will return if no child has exited yet
        waitpid(-1, &status, WNOHANG);

    }
}
pid_t old = 0;
void sig_handler(int num)
{
    if (pid > 0)
    {
        if (old != pid)
        {
            add_job(pid, cmd);
            strcpy(tcmd[i], str);
            i++;
        }
        if (i > 0)
            printf("\n[%d]+ Stopped       %s\n", i, tcmd[i - 1]);
        old = pid;
    }
}

void remove_job(pid_t pid)
{
    i--;
    Job *current = job_list;
    Job *previous = NULL;

    while (current != NULL)
    {
        if (current->pid == pid)
        {
            if (previous == NULL)
            {
                job_list = current->next;
            }
            else
            {
                previous->next = current->next;
            }
            free(current);
            return;
        }
        previous = current;
        current = current->next;
    }
}

void fg()
{
    if (job_list == NULL)
    {
        printf("No jobs available\n");
        return;
    }
    Job *current = job_list;
    while (current->next != NULL)
    {
        current = current->next;
    }

    pid_t pid = current->pid;
    printf("%d %s\n", pid, tcmd[i - 1]);
    kill(pid, SIGCONT);

    waitpid(pid, NULL, WUNTRACED);
    if (!WIFSIGNALED(status))
    { // WIFEXITED(status) == 0
        remove_job(pid);
    }

    // remove_job(pid);
    // signal(SIGCHLD,handler);
}
void bg()
{
    if (job_list == NULL)
    {
        printf("No jobs available\n");
        return;
    }

    Job *current = job_list;
    while (current->next != NULL)
    {
        current = current->next;
    }

    pid_t pid = current->pid;

    kill(pid, SIGCONT);

   signal(SIGCHLD, handler);

  
        remove_job(pid);
  
}

void jobs()
{
    int k = 0;
    if (job_list == NULL)
    {
        printf("No jobs available\n");
        return;
    }
    Job *current = job_list;
    int count = 1;
    while (current != NULL)
    {
        printf("[%d] %d %s\n", count, current->pid,tcmd[k]);
        current = current->next;
        if(k < i)
        k++;
        count++;
    }
}

void extract_external_commands()
{
    int fd = open("command.txt", O_RDONLY);
    if (fd == -1)
    {
        perror("ERROR:");
        return;
    }
    char ch;
    int i = 0, j = 0, flag = 0;
    while (read(fd, &ch, 1) > 0)
    {
        if (i >= 200 || j >= 50)
        { // Prevent overflow
            printf("Size exceed i = %d j = %d\n", i, j);
            break;
        }
        if (ch == '\n')
        {
            external[i][--j] = '\0';

            i++;
            j = 0;
        }
        else
        {
            external[i][j] = ch;
            j++;
        }
    }
}

void scan_input(char *prompt)
{

    char *ch, x = 0;

    signal(SIGINT, handler);
    signal(SIGTSTP, sig_handler);
    while (1)
    {

        printf("%s ", prompt);
        fflush(stdout);
        if (x)
        {
            memset(str, '\0', sizeof(str));
            getchar();
        }
        scanf("%[^\n]", str);
        fflush(stdin);

        if (strncmp(str, "PS1=abc", 4) == 0 && strstr(str, "PS1= ") == NULL)
        {
            strcpy(prompt, str + 4);
        }
        else if (strstr(str, "PS1") != NULL || strstr(str, "PS1= ") != NULL)
        {
            printf("PS1: command not found\n");
        }
        else
        {
            cmd = get_command(str);
            int ret = check_command_type(cmd);
            if (ret == EXTERNAL)
            {

                pid = fork();
                if (pid == 0)
                {
                    signal(SIGINT, SIG_DFL);
                    signal(SIGTSTP, SIG_DFL);
                    execute_external_commands(str);
                }
                else
                {
                    // add_job(pid, cmd);
                    waitpid(pid, &status, WUNTRACED);
                    p1 = getpid();
                }
            }
            else if (ret == BUILTIN)
            {
                execute_internal_commands(str);
            }
            else if (ret == NO_COMMAND)
                printf("Command '%s' not found\n", cmd);
        }
        x++;
    }
}

char *get_command(char *input_string)
{
    int len;
    char *ch = strchr(input_string, ' ');
    if (ch)
        len = ch - input_string;
    else
        len = strlen(input_string);
    static char ptr[30];
    strcpy(ptr, input_string);
    ptr[len] = '\0';
    // printf("this cmd-> %s\n",ptr);
    return ptr;
}

int check_command_type(char *command)
{
    int i = 0;
    while (builtins[i] != NULL)
    {
        if (strcmp(builtins[i], command) == 0)
        {
            return BUILTIN;
        }
        i++;
    }

    for (i = 0; i < 152; i++)
    {
        if (strcmp(external[i], command) == 0)
        {
            return EXTERNAL;
        }
    }
    if (*command != '\0')
        return NO_COMMAND;
}

void execute_external_commands(char *str)
{
    char *vector[30];
    int i = 0, j = 0, k = 0, n_pip = 0, arr[20], l = 1;
    char buffer[20];
    arr[0] = 0;

    while (str[j] != '\0')
    {
        if (str[j] == '|')
        {
            n_pip++;
            vector[i] = NULL;

            i++;
            arr[l++] = i;

            j += 2;
        }
        else if (str[j] == '\'')
            ;
        else if (str[j] != ' ')
        {

            buffer[k++] = str[j];
        }
        else
        {
            buffer[k] = '\0';
            vector[i] = (char *)malloc(sizeof(char) * (strlen(buffer)) + 1);
            strcpy(vector[i], buffer);

            i++;
            k = 0;
        }
        j++;
    }

    if (k != 0)
    {
        buffer[k] = '\0';
        vector[i] = (char *)malloc(sizeof(char) * (strlen(buffer)) + 1);
        strcpy(vector[i], buffer);
        i++;
    }
    vector[i] = NULL;
    // for(int k = 0; k <= i; k++)
    // {
    //     printf(" %s\n",vector[k]);
    // }
    // for(int k = 0; k <= n_pip; k++)
    // {
    //     printf(" %d\n",arr[k]);
    // }
    // return;
    if (n_pip > 0)
    {
        int fd[2], in_fd = 0;
        pid_t pid;

        for (int i = 0; i <= n_pip; i++)
        {
            if (i != n_pip)
                pipe(fd);

            pid = fork();

            if (pid == 0)
            {

                if (i != n_pip)
                {
                    close(fd[0]);
                    dup2(fd[1], 1);
                }

                execvp(vector[arr[i]], vector + arr[i]);
                exit(0);
            }
            else
            {
                if (i != n_pip)
                {
                    close(fd[1]);
                    dup2(fd[0], 0);
                    close(fd[0]);
                }
            }
        }
    }
    else
    {
        execvp(vector[0], vector);
        printf("\n");
    }
    waitpid(pid, NULL, WUNTRACED);

    exit(0);
}

void execute_internal_commands(char *input_string)
{
    if (strcmp(input_string, "echo $") == 0)
    {
        if (WIFEXITED(status))
        {
            printf("child exit status = %d\n", WEXITSTATUS(status));
        }
    }
    else if (strcmp(input_string, "echo $$") == 0)
    {
        printf("The mini-shell process id is %d\n", p1);
    }
    else if (strcmp(input_string, "echo $SHELL") == 0)
    {
        char *shell = getenv("SHELL");
        printf("%s\n", shell);
    }
    else if (strcmp(input_string, "exit") == 0)
        exit(0);
    else if (strcmp(input_string, "pwd") == 0)
    {
        char buf[80];
        getcwd(buf, 80);
        printf("%s\n", buf);
    }
    else if (strncmp(input_string, "cd ", 3) == 0)
    {
        char buf[80];
        chdir(input_string + 3);
    }
    else if (strcmp(input_string, "fg") == 0)
        fg();
    else if (strcmp(input_string, "bg") == 0)
        bg();
    else if (strcmp(input_string, "jobs") == 0)
        jobs();
}

// void handler(int num){
//     signal(SIGINT,SIG_DFL);
// }

// int main(){
//     signal(SIGINT,handler);
//   //  printf("hello\n");
//     while(1);
// }