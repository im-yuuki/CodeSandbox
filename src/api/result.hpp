#ifndef RESULT_HPP
#define RESULT_HPP

#include <crow/http_response.h>
#include <crow/http_request.h>

namespace api {

	inline crow::response result(const crow::request& req) {
		return {200, "Hello, World!"};
	}

}

#endif
