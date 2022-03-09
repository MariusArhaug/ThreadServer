#ifndef RESPONSE_H
#define RESPONSE_H

#define OK_C 200
#define NOT_FOUND_C 404
#define UNAUTHORIZED_C 401
#define INTERNAL_ERROR_C 500
#define BAD_GATEAWAY_C 502

#define OK_S "200 OK"
#define NOT_FOUND_S "404 Not Found"
#define UNAUTHORIZED_S "401 Unauthorized"
#define INTERNAL_ERROR_S "500 Internal Server Error"
#define BAD_GATEAWAY_S "502 Bad Gateway";

typedef struct response_t response_t;

response_t* response_init();

void set_request_header(response_t* self, char* method, char* uri);

void set_response_header(struct response_t* self, char* status);

void set_body(struct response_t* self, char* body);

char* response_to_str(struct response_t* self);

void response_destroy(struct response_t* self);


#endif