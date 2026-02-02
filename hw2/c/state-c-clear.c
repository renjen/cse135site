#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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

int main(void) {
  char path[256];
  state_path(path, sizeof(path));

  int ok = (remove(path) == 0);

  printf("Cache-Control: no-cache\r\n");
  printf("Content-Type: text/html\r\n\r\n");

  printf("<!DOCTYPE html><html><head><meta charset=\"utf-8\">");
  printf("<title>State Demo (C) - Clear</title></head><body>");
  printf("<h1>State Demo (C) - Clear</h1><hr/>");

  if (ok) {
    printf("<p><b>State cleared.</b></p>");
  } else {
    printf("<p><b>No state to clear</b> (or already cleared).</p>");
  }

  printf("<p><a href=\"/hw2/c/state-c-set.cgi\">Back to Set State</a></p>");
  printf("<p><a href=\"/hw2/c/state-c-view.cgi\">Go to View State</a></p>");
  printf("</body></html>");

  return 0;
}
