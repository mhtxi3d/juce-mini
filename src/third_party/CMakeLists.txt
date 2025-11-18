# Third-party libraries CMake configuration

# nlohmann JSON (header-only library)
set(JSON_BuildTests OFF CACHE INTERNAL "")
set(JSON_Install OFF CACHE INTERNAL "")
add_subdirectory(json)

# Catch2 for unit testing
set(CATCH_BUILD_TESTING OFF CACHE INTERNAL "")
set(CATCH_INSTALL_DOCS OFF CACHE INTERNAL "")
set(CATCH_INSTALL_HELPERS OFF CACHE INTERNAL "")
add_subdirectory(Catch2)

# Easylogging++
add_library(easyloggingpp STATIC
    easyloggingpp/src/easylogging++.cc
)
target_include_directories(easyloggingpp PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/easyloggingpp/src
)
target_compile_definitions(easyloggingpp PUBLIC
    ELPP_NO_DEFAULT_LOG_FILE
    ELPP_THREAD_SAFE
)

# SQLite
add_library(sqlite3 STATIC
    sqlite/sqlite3.c
)
target_include_directories(sqlite3 PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/sqlite
)
if(UNIX AND NOT APPLE)
    target_link_libraries(sqlite3 PUBLIC pthread dl)
endif()
