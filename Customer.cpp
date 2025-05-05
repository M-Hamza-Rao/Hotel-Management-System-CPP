#include "Customer.h"
#include <iostream>
#include <mysql.h>
#include <string>
using namespace std;

Customer::Customer(string id, string name, string address, string phone, string email)
    : customer_id(id), name(name), address(address), phone(phone), email(email) {}




void Customer::addCustomer(MYSQL *conn) {
    string query = "INSERT INTO Customers (customer_id, name, address, phone, email) VALUES ('" 
                    + customer_id + "', '" + name + "', '" + address + "', '" + phone + "', '" + email + "')";

    if (mysql_query(conn, query.c_str())) {
        cerr << "Customer insert failed: " << mysql_error(conn) << endl;
    } else {
        cout << "Customer added to database successfully!" << endl;
    }
}

