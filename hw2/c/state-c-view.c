#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

static void state_path(char *path, size_t pathsz) {
  const char *ip = getenv("REMOTE_ADDR");
  if (!ip || !*ip) ip = "unknown";
  char safe[128];
  size_t j = 0;
  for (size_t i = 0; ip[i] && j + 1 < sizeof(safe); i++) {
    char c = ip[i];
    safe[j++] = (isalnum((unsigned char)c) || c == '.' || c == '_' || c == '-') ? c : '_';
  }
  safe[j] = '\0';
  snprintf(path, pathsz, "/tmp/c_state_%s.txt", safe);
}

static void html_escape_print(const char *s) {
  for (; *s; s++) {
    switch (*s) {
      case '&': fputs("&amp;", stdout); break;
      case '<': fputs("&lt;", stdout); break;
      case '>': fputs("&gt;", stdout); break;
      case '"': fputs("&quot;", stdout); break;
      default: fputc(*s, stdout); break;
    }
  }
}

static void strip_newline(char *s) {
  size_t n = strlen(s);
  while (n && (s[n-1] == '\n' || s[n-1] == '\r')) {
    s[n-1] = '\0';
    n--;
  }
}

int main(void) {
  char path[256];
  state_path(path, sizeof(path));

  char name[512] = "(none)";
  char favorite[512] = "(none)";
  char saved_at[512] = "(none)";

  FILE *fp = fopen(path, "r");
  if (fp) {
    char line[1024];
    while (fgets(line, sizeof(line), fp)) {
      strip_newline(line);
      if (strncmp(line, "name=", 5) == 0) {
        strncpy(name, line + 5, sizeof(name)-1);
        name[sizeof(name)-1] = '\0';
      } else if (strncmp(line, "favorite=", 9) == 0) {
        strncpy(favorite, line + 9, sizeof(favorite)-1);
        favorite[sizeof(favorite)-1] = '\0';
      } else if (strncmp(line, "saved_at=", 9) == 0) {
        // Convert epoch to readable time if possible
        long t = atol(line + 9);
        if (t > 0) {
          time_t tt = (time_t)t;
          struct tm *tm = localtime(&tt);
          if (tm) {
            strftime(saved_at, sizeof(saved_at), "%a %b %d %H:%M:%S %Y", tm);
          } else {
            strncpy(saved_at, line + 9, sizeof(saved_at)-1);
          }
        } else {
          strncpy(saved_at, "(none)", sizeof(saved_at)-1);
        }
      }
    }
    fclose(fp);
  }

  printf("Cache-Control: no-cache\r\n");
  printf("Content-Type: text/html\r\n\r\n");

  printf("<!DOCTYPE html><html><head><meta charset=\"utf-8\">");
  printf("<title>State Demo (C) - View</title></head><body>");
  printf("<h1>State Demo (C) - View</h1><hr/>");

  printf("<p><b>Name:</b> ");
  html_escape_print(name);
  printf("</p>");

  printf("<p><b>Favorite:</b> ");
  html_escape_print(favorite);
  printf("</p>");

  printf("<p><b>Saved At:</b> ");
  html_escape_print(saved_at);
  printf("</p>");

  printf("<form method=\"POST\" action=\"/hw2/c/state-c-clear.cgi\">");
  printf("<button type=\"submit\">Clear State</button>");
  printf("</form>");

  printf("<p><a href=\"/hw2/c/state-c-set.cgi\">Back to Set State</a></p>");
  printf("</body></html>");

  return 0;
}
