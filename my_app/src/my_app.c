#include <kore/kore.h>
#include <kore/http.h>
#include <stdio.h>
#include <string.h>

#define LOG(...) kore_log(LOG_NOTICE, __VA_ARGS__) ; fflush(stdout); fflush(stderr);

char * read_entire_file(char *file)
{
    FILE *f = fopen(file, "rb");
    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);  //same as rewind(f);

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
    http_response(req, 200, html, strlen(html));
    free(html);    
    html = 0;
    return (KORE_RESULT_OK);
}

