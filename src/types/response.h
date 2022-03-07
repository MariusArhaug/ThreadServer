#ifndef RESPONSE_H
#define RESPONSE_H

#define OK "200 OK"
#define NOT_FOUND "404 Not Found"
#define UNAUTHORIZED "401 Unauthorized"
#define INTERNAL_ERROR "500 Internal Server Error"
#define BAD_GATEAWAY "502 Bad Gateway";

typedef struct response_t response_t;

response_t* response_init();

void set_request_header(response_t* self, char* method, char* uri);

void set_response_header(struct response_t* self, char* status);

void set_body(struct response_t* self, char* body);

char* response_to_str(struct response_t* self);

void response_destroy(struct response_t* self);


#endif