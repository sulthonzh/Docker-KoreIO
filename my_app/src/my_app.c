#include <kore/kore.h>
#include <kore/http.h>
#include <stdio.h>
#include <string.h>

#define LOG(...) kore_log(LOG_NOTICE, __VA_ARGS__) ; fflush(stdout); fflush(stderr);

char *str_replace(char *orig, char *rep, char *with) {
    char *result; // the return string
    char *ins;    // the next insert point
    char *tmp;    // varies
    int len_rep;  // length of rep (the string to remove)
    int len_with; // length of with (the string to replace rep with)
    int len_front; // distance between rep and end of last rep
    int count;    // number of replacements

    // sanity checks and initialization
    if (!orig || !rep)
        return NULL;
    len_rep = strlen(rep);
    if (len_rep == 0)
        return NULL; // empty rep causes infinite loop during count
    if (!with)
        with = "";
    len_with = strlen(with);

    // count the number of replacements needed
    ins = orig;
    for ( (count = 0) ; (tmp = (strstr(ins, rep))); ++count) {
        ins = tmp + len_rep;
    }

    tmp = result = malloc(strlen(orig) + (len_with - len_rep) * count + 1);

    if (!result)
        return NULL;

    // first time through the loop, all the variable are set correctly
    // from here on,
    //    tmp points to the end of the result string
    //    ins points to the next occurrence of rep in orig
    //    orig points to the remainder of orig after "end of rep"
    while (count--) {
        ins = strstr(orig, rep);
        len_front = ins - orig;
        tmp = strncpy(tmp, orig, len_front) + len_front;
        tmp = strcpy(tmp, with) + len_with;
        orig += len_front + len_rep; // move to next "end of rep"
    }
    strcpy(tmp, orig);
    return result;
}

char * read_entire_file(char *file)
{
    FILE *f = fopen(file, "rb");
    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);  
    char *string = malloc(fsize + 1);
    fread(string, fsize, 1, f);
    fclose(f);

    string[fsize] = 0;
    return string;
}

int page(struct http_request *req)
{    
    LOG("HTTP REQUEST");
    LOG("method: %d", req->method);
    LOG("host: %s", req->host);
    LOG("path: %s", req->path);
    LOG("agent: %s", req->agent);
    LOG("query: %s", req->query_string);

    char *html = read_entire_file("docs/index.html");
    html = str_replace(html, "{{ url }}", req->path);
    if (req->method == 1)
        html = str_replace(html, "Hello from kore.io", "Posted");

    http_response(req, 200, html, strlen(html));

    return (KORE_RESULT_OK);
}

