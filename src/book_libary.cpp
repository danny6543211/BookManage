#include "book_libary.h"

// 书的数据库操作
static int check_book_callback(void *data, int argc, char **argv, char **col_name)
{
    if (argc != 0)
        *(int *)data = 1;
    else
        *(int *)data = 0;
    return 0;
}

static int book_status_callback(void *data, int argc, char **argv, char **col_name)
{
    *(int *)data = std::stoi(argv[0]);
    return 0;
}

book_libary::book_libary()
{
    this->choice_database();
}

void book_libary::add_book(std::string book_name, int book_status)
{
    std::string sql_command("insert into books values");
    sql_command += "('" + book_name + "'," + std::to_string(book_status) + ")";
    char *err_msg;
    int res = sqlite3_exec(db, sql_command.c_str(), nullptr, 0, &err_msg);
    if (res != SQLITE_OK)
        std::cout << err_msg << std::endl;
}

int book_libary::check_book(std::string book_name)
{
    std::string sql_command = "select * from books where book_name == ";
    sql_command += "'" + book_name + "';";
    char *err_msg;
    int *find = (int *)calloc(1, sizeof(int));
    int res = sqlite3_exec(db, sql_command.c_str(), check_book_callback, find, &err_msg);
    if (res != SQLITE_OK)
    {
        std::cout << err_msg << std::endl;
        return -1;
    }
    res = *find;
    free(find);
    return res;
}

int book_libary::book_status(std::string book_name)
{
    std::string sql_command = "select status from books where book_name == ";
    sql_command += "'" + book_name + "';";
    char *err_msg;
    int *status = (int *)calloc(1, sizeof(int));
    int res = sqlite3_exec(db, sql_command.c_str(), book_status_callback, status, &err_msg);
    if (res != SQLITE_OK)
    {
        std::cout << err_msg << std::endl;
        return -1;
    }
    res = *status;
    free(status);
    return res;
}

void book_libary::change_book_status(std::string book_name, int new_status)
{
    std::string sql_command = "update books set status = ";
    sql_command += std::to_string(new_status) + " where book_name = '" + book_name + "';";
    char *err_msg;
    int res = sqlite3_exec(db, sql_command.c_str(), nullptr, nullptr, &err_msg);
    if (res != SQLITE_OK)
    {
        std::cout << err_msg << std::endl;
    }
}

void book_libary::delete_book(std::string book_name)
{
    std::string sql_command = "delete from books where book_name == '";
    sql_command += book_name + "';";
    char *err_msg;
    int res = sqlite3_exec(db, sql_command.c_str(), nullptr, nullptr, &err_msg);
    if (res != SQLITE_OK)
    {
        std::cout << err_msg << std::endl;
    }
}

void book_libary::choice_database()
{
    int res = sqlite3_open("book_libary.db", &db);
    if (res)
        std::cout << "can't open database: " << sqlite3_errmsg(db) << std::endl;
}