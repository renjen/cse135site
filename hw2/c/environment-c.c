#include <stdio.h>

extern char **environ;

int main(void) {
    // CGI headers
    printf("Cache-Control: no-cache\r\n");
    printf("Content-Type: text/plain\r\n\r\n");

    // Print all environment variables
    for (char **env = environ; *env != NULL; env++) {
        printf("%s\n", *env);
    }

    return 0;
}

