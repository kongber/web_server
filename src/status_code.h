// 
// Create by kong on 2024/11/6
//
#ifndef WEB_SERVER_STATUS_CODE_H
#define WEB_SERVER_STATUS_CODE_H

#include <map>
#include "../http_client.h"

namespace WebServer {

enum class ErrorCode {
    ok = 0,
    unknown = 1,
    indata_error = 2,
};

void error_if(bool condition, const char *error_msg) {
    if (condition) {
        perror(error_msg);
        exit(1);
    }
}

inline int error_code(ErrorCode error_code_enum) noexcept {
    return static_cast<int>(error_code_enum);
}

enum class StatusCode {
    unknown = 0,
    success_ok = 200,
    redirection_multiple_choices = 300,
    client_error_bad_request = 400,
    server_error_internal_server_error = 500,
};

inline const std::map<StatusCode, std::string> &status_code_strings() {
    static const std::map<StatusCode, std::string> status_code_strings = {
        {StatusCode::unknown, ""},
        {StatusCode::success_ok, "200 OK"},
        {StatusCode::redirection_multiple_choices, "300 Multiple choices"},
        {StatusCode::client_error_bad_request, "400 Bad request"},
        {StatusCode::server_error_internal_server_error, "500 Internal Server Error"}
    };
    return status_code_strings;
}

}



#endif //WEB_SERVER_STATUS_CODE_H
