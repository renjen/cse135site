#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static void json_escape_print(const char *s) {
    // Minimal JSON string escaper (handles quotes, backslashes, and newlines)
    for (const unsigned char *p = (const unsigned char *)s; *p; p++) {
        if (*p == '\"')      fputs("\\\"", stdout);
        else if (*p == '\\') fputs("\\\\", stdout);
        else if (*p == '\n') fputs("\\n", stdout);
        else if (*p == '\r') fputs("\\r", stdout);
        else if (*p == '\t') fputs("\\t", stdout);
        else putchar(*p);
    }
}

int main(void) {
    // CGI headers
    printf("Cache-Control: no-cache\r\n");
    printf("Content-Type: application/json\r\n\r\n");

    // Time string
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    char tbuf[128];
    if (tm_info) {
        strftime(tbuf, sizeof(tbuf), "%a %b %d %H:%M:%S %Y", tm_info);
    } else {
        snprintf(tbuf, sizeof(tbuf), "unknown");
    }

    // Client IP (CGI env var)
    const char *ip = getenv("REMOTE_ADDR");
    if (!ip) ip = "unknown";

    // Output JSON
    printf("{\n");
    printf("  \"message\": \"Hello from C\",\n");
    printf("  \"language\": \"C\",\n");

    printf("  \"generated_at\": \"");
    json_escape_print(tbuf);
    printf("\",\n");

    printf("  \"ip\": \"");
    json_escape_print(ip);
    printf("\"\n");

    printf("}\n");

    return 0;
}
