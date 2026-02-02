#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    time_t now = time(NULL);
    char *ip = getenv("REMOTE_ADDR");

    printf("Cache-Control: no-cache\n");
    printf("Content-Type: text/html\n\n");

    printf("<!DOCTYPE html>");
    printf("<html><head><title>Hello C</title></head><body>");
    printf("<h1>Hello from C</h1><hr/>");
    printf("<p>This page was generated using the C programming language.</p>");
    printf("<p>Generated at: %s</p>", ctime(&now));
    printf("<p>Your IP Address: %s</p>", ip ? ip : "Unknown");
    printf("</body></html>");

    return 0;
}
