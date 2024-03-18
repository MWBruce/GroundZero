#include <iostream>
#include <stdexcept>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>

int main() {
    const std::string server = "unix:///var/run/mysqld/mysqld.sock";

    const std::string username = "root";
    const std::string password = "Test_password1.";  // Not my actual password for anything but i recognise this as a potential security issue

    try {
        sql::Driver* driver = get_driver_instance();
        std::unique_ptr<sql::Connection> con(driver->connect(server, username, password));
        con->setSchema("userDatabase");

        // Insert a new user
        std::unique_ptr<sql::PreparedStatement> pstmt(con->prepareStatement("INSERT INTO users(username, passwordHash) VALUES (?, ?)"));
        pstmt->setString(1, "newuser");
        pstmt->setString(2, "hashed_password");
        pstmt->executeUpdate();

        // Retrieve and print the user
        std::unique_ptr<sql::Statement> stmt(con->createStatement());
        std::unique_ptr<sql::ResultSet> res(stmt->executeQuery("SELECT * FROM users WHERE username = 'newuser'"));
        while (res->next()) {
            std::cout << "User ID: " << res->getInt("userId") << std::endl;
            std::cout << "Username: " << res->getString("username") << std::endl;
            std::cout << "Password Hash: " << res->getString("passwordHash") << std::endl;
        }
    } catch (sql::SQLException& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
