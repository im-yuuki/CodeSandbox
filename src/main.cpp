#define APP_VERSION "1.1-beta"
#define LOGO R"(
__  __            __
\ \/ /_  ____  __/ /__(_)
 \  / / / / / / / //_/ /
 / / /_/ / /_/ / ,< / /
/_/\__,_/\__,_/_/|_/_/
      / ___/____  / __/ /__      ______ _________
      \__ \/ __ \/ /_/ __/ | /| / / __ `/ ___/ _ \
     ___/ / /_/ / __/ /_ | |/ |/ / /_/ / /  /  __/
    /____/\____/_/  \__/ |__/|__/\__/_/_/   \___/

)"

#include <crow.h>
#include <string>

#include "api/info.hpp"
#include "api/submit.hpp"
#include "data/problems.hpp"
#include "data/storage.hpp"
#include "utils/env.hpp"
#include "utils/logging.hpp"

namespace api {

    inline crow::App<> init() {
        using namespace crow;
        logger::setHandler(new SpdlogLogger(logging::create_logger("crow")));
        SimpleApp app;
        app.loglevel(LogLevel::Warning);
        CROW_ROUTE(app, "/version").methods(HTTPMethod::GET)([]() { return APP_VERSION; });
        CROW_ROUTE(app, "/modules").methods(HTTPMethod::GET)(get_all_modules);
        CROW_ROUTE(app, "/problems").methods(HTTPMethod::GET)(get_all_problems);
        CROW_ROUTE(app, "/submit").methods(HTTPMethod::POST)(submit);
        return app;
    }

}

int main() {
    std::cout << LOGO;
    utils::load_env();
    logging::init();
    modules::init();
    data::scan_problems();
    const int port = stoi(utils::get_env("PORT", "4000"));
    logging::info(std::string("Starting sandbox node version: ") + APP_VERSION + " at http://0.0.0.0:" + std::to_string(port));
    api::init().port(port).multithreaded().run();
    return 0;
}
