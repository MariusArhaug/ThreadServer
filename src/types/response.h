#ifndef RESPONSE_H
#define RESPONSE_H

#define OK_C 200
#define BAD_REQUEST_C 400
#define UNAUTHORIZED_C 401
#define FORBIDDEN_C 403
#define NOT_FOUND_C 404
#define METHOD_NOT_ALLOWED_C 405
#define INTERNAL_ERROR_C 500
#define BAD_GATEAWAY_C 502

#define OK_S "200 OK"
#define BAD_REQUEST_S "400 Bad Request"
#define FORBIDDEN_S "403 Forbidden "
#define NOT_FOUND_S "404 Not Found"
#define UNAUTHORIZED_S "401 Unauthorized"
#define METHOD_NOT_ALLOWED_S "405 Method Not Allowed"
#define INTERNAL_ERROR_S "500 Internal Server Error"
#define BAD_GATEAWAY_S "502 Bad Gateway";


#define JSON_CONTENT "application/json"
#define HTML_CONTENT "text/html"
#define ALLOW_ORIGIN "Access-Control-Allow-Origin: *"


typedef struct response_t response_t;

/**
 * @brief allocate memory for response struct
 * 
 * @return response_t* pointer to respone struct 
 */
response_t* response_init();

/**
 * @brief Set the request header object
 * 
 * @param self resp to set header to
 * @param method what method to be used
 * @param uri path in header
 */
void set_request_header(response_t* self, char* method, char* uri);

/**
 * @brief Set the response status object
 * 
 * @param self resp 
 * @param status status type defined in response.h
 */
void set_response_status(struct response_t* self, char* status);

/**
 * @brief Set the response content object
 * 
 * @param self resp
 * @param content content type of resp, application/json or text/html, etc..
 */
void set_response_content(struct response_t* self, char* content);

/**
 * @brief Set the body object
 * 
 * @param self 
 * @param body 
 */
void set_body(struct response_t* self, char* body);

/**
 * @brief destroy respone struct
 * 
 * @param self 
 */
void response_destroy(struct response_t* self);

/**
 * @brief send response struct to receiver
 * 
 * @param self resp to send
 * @param connfd file descriptor of reciever
 */
void send_response(struct response_t* self, int connfd);

#endif