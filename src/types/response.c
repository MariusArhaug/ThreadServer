#include "response.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#define T_SIZE 1024
#define RFC2616 "%a, %d %b %Y %T GMT"
#define MAXREQ (4096*1024)

#define SET_STATUS(o, s)        \
  strcat(o, s->header->method); \
  strcat(o, " ");               \
  strcat(o, s->header->status); \
  strcat(o, "\n")

#define SET_DATE(o)                     \
  strcat(o, "Date: ");                  \
  char buf[T_SIZE];                     \
  time_t rawtime = time(NULL);          \
  struct tm *ptm = localtime(&rawtime); \
  strftime(buf, T_SIZE, RFC2616, ptm);  \
  strcat(output, buf);                  \
  strcat(output, "\n")

#define SET_LENGTH(o, s)                            \
  strcat(output, "Content-Length: ");               \
  char length[4];                                   \
  sprintf(length, "%d", s->header->content_length); \
  strcat(output, length);                           \
  strcat(output, "\n")

#define SET_NEWLINE(o)\
  strcat(o, "\n");

typedef struct {
  char  *method,    // "GET" or "POST"
        *uri,       // "/index.html" 
        *date,
        *content_type,
        *accept_range,
        *connection,
        *status,
        *prot;      // "HTTP/1.1"

  int content_length;
} response_h_t;

struct response_t {
  response_h_t* header;
  char* body;
} response;

static char* response_to_str(struct response_t* self);

response_t* response_init() {
  struct response_t* self = malloc(sizeof(struct response_t));
  memset(self, 0, sizeof(struct response_t));
  self->header = malloc(sizeof(response_h_t));
  memset(self->header, 0, sizeof(response_h_t));
  self->header->method = "HTTP/1.1";
  self->header->accept_range = "Accept-ranges: bytes";
  self->header->content_type = malloc(50*sizeof(char));
  strcpy(self->header->content_type, "Content-Type: "); 
  self->header->connection = "Connection: keep-alive";
  return self;
}

void set_request_header(struct response_t* self, char* method, char* uri) {
  self->header->method = method;
  self->header->uri = uri;
}

void set_response_status(struct response_t* self, char* status) {
  self->header->status = status;
}

void set_response_content(struct response_t* self, char* content) {
  strcat(self->header->content_type, content);
}

void set_body(struct response_t* self, char* body) {
  self->header->content_length = strlen(body);
  self->body = body;
}

void response_destroy(struct response_t* self) {
  free(self->header);
  free(self);
}

char* response_to_str(struct response_t* self) {
  char* output = malloc(MAXREQ*sizeof(char));
  memset(output, 0, MAXREQ*sizeof(char));

  SET_STATUS(output, self);
  SET_DATE(output);
  SET_LENGTH(output, self);

  strcat(output, ALLOW_ORIGIN);
  SET_NEWLINE(output);

  strcat(output, self->header->content_type);
  SET_NEWLINE(output);

  strcat(output, self->header->accept_range);
  SET_NEWLINE(output);

  strcat(output, self->header->connection);
  SET_NEWLINE(output);
  SET_NEWLINE(output);

  strcat(output, self->body);

  return output;

}

void send_response(int connfd, struct response_t* self) {
  char* resp_str = response_to_str(self);
  write(connfd, resp_str, strlen(resp_str));
  free(resp_str);
  response_destroy(self);
}