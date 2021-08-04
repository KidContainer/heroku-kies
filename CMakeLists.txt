cmake_minimum_required(VERSION 3.16)
project(heroku_kies)

set(CMAKE_CXX_STANDARD 17)

include(CTest)

if(LIMIT_MEM)
    message("Limit Compiler Memory Usage")
    add_compile_options("SHELL:--param ggc-min-expand=1" "SHELL:--param ggc-min-heapsize=131072")
endif()

add_compile_options(-fsanitize=address -g)
add_link_options(-fsanitize=address)

add_subdirectory(lib)
add_subdirectory(utils)

function(add_all_source src_files)
    aux_source_directory(${src_files} ${src_files})
    list(FILTER ${src_files} EXCLUDE REGEX "_test.cpp")
    set(${src_files} ${${src_files}} PARENT_SCOPE)
endfunction()


add_all_source(db)
add_all_source(handler)
add_all_source(utils)


include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(heroku_kies main.cpp ${db} ${handler} ${utils})
target_link_libraries(heroku_kies PRIVATE lib ${CONAN_LIBS})

install(TARGETS heroku_kies DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/..)

