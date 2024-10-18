#define APP_VERSION "0.0.1"

#include <crow.h>
#include <string>

#include "utils/compare.hpp"
#include "utils/env.hpp"
#include "utils/logging.hpp"

namespace api {
    inline std::string get_all_problems() {
        return "Hello, World!";
    }

    inline crow::response submit_problem(const crow::request& req) {
        return {200, "Hello, World!"};
    }

    inline std::string get_result() {
        return "Hello, World!";
    }

    inline crow::App<> init() {
        using namespace crow;
        const auto logger = new SpdlogLogger(logging::create_logger("crow"));
        logger::setHandler(logger);
        App<> app;
        CROW_ROUTE(app, "/version").methods(HTTPMethod::GET)([]() { return APP_VERSION; });
        CROW_ROUTE(app, "/problems").methods(HTTPMethod::GET)(get_all_problems);
        CROW_ROUTE(app, "/submit").methods(HTTPMethod::POST)(submit_problem);
        CROW_ROUTE(app, "/result").methods(HTTPMethod::GET)(get_result);
        return app;
    }

}

int main() {
    logging::init();
    const int port = stoi(utils::get_env("PORT", "4000"));
    auto app = api::init();
    app.port(port).multithreaded().run();
}
