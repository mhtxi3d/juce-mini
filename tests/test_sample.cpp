#include <catch2/catch_all.hpp>
#include <nlohmann/json.hpp>
#include <sqlite3.h>
#include <string>

TEST_CASE("JSON library works correctly", "[json]")
{
    SECTION("Create and parse JSON object")
    {
        nlohmann::json j;
        j["name"] = "test";
        j["value"] = 42;
        j["active"] = true;

        REQUIRE(j["name"] == "test");
        REQUIRE(j["value"] == 42);
        REQUIRE(j["active"] == true);
    }

    SECTION("JSON array operations")
    {
        nlohmann::json arr = nlohmann::json::array();
        arr.push_back(1);
        arr.push_back(2);
        arr.push_back(3);

        REQUIRE(arr.size() == 3);
        REQUIRE(arr[0] == 1);
        REQUIRE(arr[2] == 3);
    }

    SECTION("JSON serialization")
    {
        nlohmann::json j = {{"key", "value"}};
        std::string str = j.dump();
        REQUIRE(str == R"({"key":"value"})");
    }
}

TEST_CASE("SQLite library works correctly", "[sqlite]")
{
    SECTION("SQLite version is available")
    {
        const char* version = sqlite3_libversion();
        REQUIRE(version != nullptr);
        REQUIRE(std::string(version).length() > 0);
    }

    SECTION("In-memory database operations")
    {
        sqlite3* db;
        int rc = sqlite3_open(":memory:", &db);
        REQUIRE(rc == SQLITE_OK);

        // Create a table
        const char* createSql = "CREATE TABLE test (id INTEGER PRIMARY KEY, name TEXT);";
        rc = sqlite3_exec(db, createSql, nullptr, nullptr, nullptr);
        REQUIRE(rc == SQLITE_OK);

        // Insert data
        const char* insertSql = "INSERT INTO test (name) VALUES ('sample');";
        rc = sqlite3_exec(db, insertSql, nullptr, nullptr, nullptr);
        REQUIRE(rc == SQLITE_OK);

        // Verify insertion
        sqlite3_stmt* stmt;
        rc = sqlite3_prepare_v2(db, "SELECT COUNT(*) FROM test;", -1, &stmt, nullptr);
        REQUIRE(rc == SQLITE_OK);

        rc = sqlite3_step(stmt);
        REQUIRE(rc == SQLITE_ROW);
        REQUIRE(sqlite3_column_int(stmt, 0) == 1);

        sqlite3_finalize(stmt);
        sqlite3_close(db);
    }
}

TEST_CASE("C++20 features work", "[cpp20]")
{
    SECTION("Designated initializers")
    {
        struct Point {
            int x;
            int y;
        };
        Point p{.x = 10, .y = 20};
        REQUIRE(p.x == 10);
        REQUIRE(p.y == 20);
    }

    SECTION("Ranges concepts")
    {
        std::vector<int> v = {1, 2, 3, 4, 5};
        REQUIRE(std::ranges::size(v) == 5);
    }
}
