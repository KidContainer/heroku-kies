cmake_minimum_required(VERSION 3.16)
project(heroku_kies)

set(CMAKE_CXX_STANDARD 17)

if(LIMIT_MEM)
    message("Limit Compiler Memory Usage")
    add_compile_options("SHELL:--param ggc-min-expand=1" "SHELL:--param ggc-min-heapsize=131072")
endif()

add_compile_options(-g)

add_subdirectory(lib)

aux_source_directory(db db)
aux_source_directory(handler handler)
aux_source_directory(utils utils)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(heroku_kies main.cpp ${db} ${handler} ${utils})
target_link_libraries(heroku_kies PRIVATE lib ${CONAN_LIBS})

install(TARGETS heroku_kies DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/..)