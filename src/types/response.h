#ifndef RESPONSE_H
#define RESPONSE_H

#define OK_C 200
#define BAD_REQUEST_C 400
#define UNAUTHORIZED_C 401
#define NOT_FOUND_C 404
#define METHOD_NOT_ALLOWED_C 405
#define INTERNAL_ERROR_C 500
#define BAD_GATEAWAY_C 502

#define OK_S "200 OK"
#define BAD_REQUEST_S "400 Bad Request"
#define NOT_FOUND_S "404 Not Found"
#define UNAUTHORIZED_S "401 Unauthorized"
#define METHOD_NOT_ALLOWED_S "405 Method Not Allowed"
#define INTERNAL_ERROR_S "500 Internal Server Error"
#define BAD_GATEAWAY_S "502 Bad Gateway";


#define JSON_CONTENT "application/json"
#define HTML_CONTENT "text/html"
#define ALLOW_ORIGIN "Access-Control-Allow-Origin: *"


typedef struct response_t response_t;

response_t* response_init();

void set_request_header(response_t* self, char* method, char* uri);

void set_response_status(struct response_t* self, char* status);
void set_response_content(struct response_t* self, char* content);
void set_body(struct response_t* self, char* body);
void response_destroy(struct response_t* self);
void send_response(int connfd, struct response_t* self);

#endif