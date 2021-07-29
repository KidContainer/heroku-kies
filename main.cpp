#include <cstdlib>
#include <cinatra.hpp>
#include <spdlog/spdlog.h>
#include "handler/handler.hpp"
#include "utils/file.hpp"
using namespace cinatra;


void bind_http(http_server& server){
    server.set_http_handler<GET,POST>("/",handler::home_page); //The home page
    server.set_http_handler<GET,POST>("/update_file", handler::upload_file); //Update file handler
}

int main() {
    // const char* port = std::getenv("PORT");
    // if(port == nullptr){
    //     port = "8080";
    // }
    // SPDLOG_INFO("PORT={}",port);
    // std::size_t max_thread_num = std::thread::hardware_concurrency();
    // http_server server(max_thread_num);
    // server.listen("0.0.0.0", port);
    
    // bind_http(server);

    // server.run();

    std::cout<<utils::read_template("../../template/html/index.html")<<std::endl;

    return 0;
}