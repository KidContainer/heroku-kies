#include <cstdlib>
#include <cinatra.hpp>
#include <spdlog/spdlog.h>
#include "handler/handler.hpp"
#include "handler/page.hpp"
#include "handler/table_manage.hpp"

#include "utils/file.hpp"
#include "db/db_operate.hpp"

#include "db/t_user_info.hpp"

using namespace cinatra;

void bind_page(http_server &server)
{
    server.set_http_handler<GET>("/", handler::home_page);              //The home page
}

void bind_api(http_server &server)
{
    server.set_http_handler<POST>("/api/upload_file", handler::upload_file); //Update file handler
    server.set_http_handler<GET>("/api/db/t_user_info_op", handler::t_user_info_op);

}

int main()
{
    const char *port = std::getenv("PORT");
    if (port == nullptr)
    {
        port = "8080";
    }

    SPDLOG_INFO("PORT={}", port);
    std::size_t max_thread_num = std::thread::hardware_concurrency();
    http_server server(max_thread_num);
    server.listen("0.0.0.0", port);
    server.set_not_found_handler(handler::not_found_page);

    bind_page(server);
    bind_api(server);

    db::database_init();


    server.run();    

    return 0;
}