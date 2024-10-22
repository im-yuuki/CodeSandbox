#ifndef SUBMIT_HPP
#define SUBMIT_HPP
#include <crow/http_request.h>
#include <crow/http_response.h>

namespace api {

	inline crow::response submit(const crow::request& req) {
		return {200, "Hello, World!"};
	}

}

#endif

