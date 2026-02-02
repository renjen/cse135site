#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_IN   8192
#define MAX_VAL  512

static void url_decode(char *dst, const char *src, size_t dstsz) {
  size_t di = 0;
  for (size_t si = 0; src[si] && di + 1 < dstsz; si++) {
    if (src[si] == '+') {
      dst[di++] = ' ';
    } else if (src[si] == '%' && isxdigit((unsigned char)src[si+1]) && isxdigit((unsigned char)src[si+2])) {
      char hex[3] = { src[si+1], src[si+2], 0 };
      dst[di++] = (char) strtol(hex, NULL, 16);
      si += 2;
    } else {
      dst[di++] = src[si];
    }
  }
  dst[di] = '\0';
}

static void get_kv(const char *body, const char *key, char *out, size_t outsz) {
  out[0] = '\0';
  size_t klen = strlen(key);

  const char *p = body;
  while (p && *p) {
    const char *amp = strchr(p, '&');
    size_t len = amp ? (size_t)(amp - p) : strlen(p);

    const char *eq = memchr(p, '=', len);
    if (eq) {
      size_t nlen = (size_t)(eq - p);
      if (nlen == klen && strncmp(p, key, klen) == 0) {
        char enc[MAX_VAL] = {0};
        size_t vlen = len - nlen - 1;
        if (vlen >= sizeof(enc)) vlen = sizeof(enc) - 1;
        memcpy(enc, eq + 1, vlen);
        enc[vlen] = '\0';
        url_decode(out, enc, outsz);
        return;
      }
    }

    p = amp ? amp + 1 : NULL;
  }
}

static void state_path(char *path, size_t pathsz) {
  const char *ip = getenv("REMOTE_ADDR");
  if (!ip || !*ip) ip = "unknown";
  // Replace unsafe chars just in case
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

int main(void) {
  const char *method = getenv("REQUEST_METHOD");
  if (!method) method = "GET";

  char name[MAX_VAL] = "";
  char favorite[MAX_VAL] = "";

  int saved = 0;

  if (strcmp(method, "POST") == 0) {
    const char *cl = getenv("CONTENT_LENGTH");
    int n = cl ? atoi(cl) : 0;
    if (n < 0) n = 0;
    if (n > MAX_IN) n = MAX_IN;

    char body[MAX_IN + 1];
    size_t got = fread(body, 1, (size_t)n, stdin);
    body[got] = '\0';

    get_kv(body, "name", name, sizeof(name));
    get_kv(body, "favorite", favorite, sizeof(favorite));

    // Save to file (server-side state)
    char path[256];
    state_path(path, sizeof(path));
    FILE *fp = fopen(path, "w");
    if (fp) {
      time_t now = time(NULL);
      fprintf(fp, "name=%s\n", name);
      fprintf(fp, "favorite=%s\n", favorite);
      fprintf(fp, "saved_at=%ld\n", (long)now);
      fclose(fp);
      saved = 1;
    }
  }

  printf("Cache-Control: no-cache\r\n");
  printf("Content-Type: text/html\r\n\r\n");

  printf("<!DOCTYPE html><html><head><meta charset=\"utf-8\">");
  printf("<title>State Demo (C) - Set</title></head><body>");
  printf("<h1>State Demo (C) - Set</h1><hr/>");
  printf("<p>Enter values to save (server-side state stored in a file on the server).</p>");

  if (saved) {
    printf("<p><b>Saved!</b> ");
    printf("Go to <a href=\"/hw2/c/state-c-view.cgi\">View State</a>.</p>");
  }

  printf("<form method=\"POST\" action=\"/hw2/c/state-c-set.cgi\">");
  printf("<p><label>Name: <input name=\"name\" value=\"");
  html_escape_print(name);
  printf("\"></label></p>");

  printf("<p><label>Favorite: <input name=\"favorite\" value=\"");
  html_escape_print(favorite);
  printf("\"></label></p>");

  printf("<p><button type=\"submit\">Save State</button></p>");
  printf("</form>");

  printf("<p><a href=\"/hw2/c/state-c-view.cgi\">Go to View State</a></p>");
  printf("</body></html>");

  return 0;
}
