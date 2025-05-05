#ifndef CUSTOMER_H
#define CUSTOMER_H
#include <mysql.h>
#include <string>

class Customer {
private:
    std::string customer_id;
    std::string name;
    std::string address;
    std::string phone;
    std::string email;

public:
    Customer(std::string id, std::string name, std::string address, std::string phone, std::string email);
    void displayCustomerInfo();
    std::string getCustomerID();
    void addCustomer(MYSQL *conn);
};

#endif // CUSTOMER_H
