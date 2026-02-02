#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

static int hexval(char c) {
    if ('0' <= c && c <= '9') return c - '0';
    if ('a' <= c && c <= 'f') return 10 + (c - 'a');
    if ('A' <= c && c <= 'F') return 10 + (c - 'A');
    return -1;
}

// URL decode: + -> space, %XX -> byte
static void urldecode(char *dst, const char *src, size_t dstsz) {
    size_t di = 0;
    for (size_t si = 0; src[si] && di + 1 < dstsz; si++) {
        if (src[si] == '+') {
            dst[di++] = ' ';
        } else if (src[si] == '%' && src[si+1] && src[si+2]) {
            int hi = hexval(src[si+1]);
            int lo = hexval(src[si+2]);
            if (hi >= 0 && lo >= 0) {
                dst[di++] = (char)((hi << 4) | lo);
                si += 2;
            } else {
                dst[di++] = src[si];
            }
        } else {
            dst[di++] = src[si];
        }
    }
    dst[di] = '\0';
}

static void print_kv_li(const char *k, const char *v_raw) {
    // decode value for display
    char v[4096];
    urldecode(v, v_raw, sizeof(v));
    printf("<li>%s = %s</li>\n", k, v);
}

// Very small helper: pull "key":"value" from JSON (string values only).
// This is intentionally simple for HW purposes.
static int json_get_string(const char *json, const char *key, char *out, size_t outsz) {
    // look for "key"
    char pattern[256];
    snprintf(pattern, sizeof(pattern), "\"%s\"", key);
    const char *p = strstr(json, pattern);
    if (!p) return 0;

    // find the colon
    p = strchr(p, ':');
    if (!p) return 0;
    p++;

    // skip spaces
    while (*p && isspace((unsigned char)*p)) p++;

    // must start with quote
    if (*p != '"') return 0;
    p++;

    // copy until next unescaped quote (we'll keep it simple and stop at ")
    size_t i = 0;
    while (*p && *p != '"' && i + 1 < outsz) {
        // minimal escape handling for \" and \\ (optional)
        if (*p == '\\' && p[1] && i + 1 < outsz) {
            p++;
            out[i++] = *p++;
        } else {
            out[i++] = *p++;
        }
    }
    out[i] = '\0';
    return 1;
}

int main(void) {
    const char *method = getenv("REQUEST_METHOD");
    if (!method) method = "UNKNOWN";

    const char *content_type = getenv("CONTENT_TYPE");
    if (!content_type) content_type = "";

    const char *qs = getenv("QUERY_STRING");
    if (!qs) qs = "";

    const char *ip = getenv("REMOTE_ADDR");
    if (!ip) ip = "unknown";

    const char *ua = getenv("HTTP_USER_AGENT");
    if (!ua) ua = "unknown";

    // Read body if needed
    char *body = NULL;
    long body_len = 0;
    const char *clen = getenv("CONTENT_LENGTH");
    if (clen) body_len = strtol(clen, NULL, 10);

    if ((strcmp(method, "POST") == 0 || strcmp(method, "PUT") == 0 || strcmp(method, "DELETE") == 0) && body_len > 0) {
        body = (char*)malloc((size_t)body_len + 1);
        if (body) {
            size_t n = fread(body, 1, (size_t)body_len, stdin);
            body[n] = '\0';
        }
    }

    // Timestamp
    time_t t = time(NULL);
    char timestr[128];
    strftime(timestr, sizeof(timestr), "%a %b %d %H:%M:%S %Y", localtime(&t));

    // Headers + HTML
    printf("Cache-Control: no-cache\r\n");
    printf("Content-Type: text/html\r\n\r\n");

    printf("<!DOCTYPE html><html><head><title>Echo C</title></head><body>\n");
    printf("<h1>Echo (C)</h1><hr/>\n");
    printf("<p><b>Method:</b> %s</p>\n", method);
    printf("<p><b>Content-Type:</b> %s</p>\n", content_type);
    printf("<p><b>Generated at:</b> %s</p>\n", timestr);
    printf("<p><b>Your IP:</b> %s</p>\n", ip);
    printf("<p><b>User Agent:</b> %s</p>\n", ua);

    printf("<h2>Parameters</h2>\n<ul>\n");

    // Decide where params come from:
    // - GET: query string
    // - POST/PUT/DELETE:
    //    - JSON: parse name/favorite if present
    //    - urlencoded: parse key=value&...
    const char *data = NULL;

    if (strcmp(method, "GET") == 0) {
        data = qs;
    } else {
        data = body ? body : "";
    }

    if (strstr(content_type, "application/json") != NULL) {
        char name[1024] = {0};
        char favorite[1024] = {0};

        int found_any = 0;
        if (json_get_string(data, "name", name, sizeof(name))) {
            printf("<li>name = %s</li>\n", name);
            found_any = 1;
        }
        if (json_get_string(data, "favorite", favorite, sizeof(favorite))) {
            printf("<li>favorite = %s</li>\n", favorite);
            found_any = 1;
        }

        // also accept "class" if your form uses it
        char klass[1024] = {0};
        if (json_get_string(data, "class", klass, sizeof(klass))) {
            printf("<li>class = %s</li>\n", klass);
            found_any = 1;
        }

        if (!found_any) {
            printf("<li>(no JSON parameters found)</li>\n");
        }
    } else {
        // parse x-www-form-urlencoded or query string: k=v&k2=v2
        // We'll split on '&' and '='
        char *tmp = strdup(data ? data : "");
        if (tmp && tmp[0] != '\0') {
            char *saveptr = NULL;
            for (char *pair = strtok_r(tmp, "&", &saveptr); pair; pair = strtok_r(NULL, "&", &saveptr)) {
                char *eq = strchr(pair, '=');
                if (eq) {
                    *eq = '\0';
                    const char *k = pair;
                    const char *v = eq + 1;
                    if (k && *k) print_kv_li(k, v);
                } else {
                    // key without '='
                    if (pair && *pair) printf("<li>%s</li>\n", pair);
                }
            }
        } else {
            printf("<li>(no parameters)</li>\n");
        }
        if (tmp) free(tmp);
    }

    printf("</ul>\n</body></html>\n");

    if (body) free(body);
    return 0;
}
