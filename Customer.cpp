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

string Customer::getCustomerNameByID(MYSQL *conn, string customer_id) {
    string query = "SELECT name FROM Customers WHERE customer_id = '" + customer_id + "'";

    if (mysql_query(conn, query.c_str())) {
        cerr << "Error fetching customer name: " << mysql_error(conn) << endl;
        return "";
    }

    MYSQL_RES *res = mysql_store_result(conn);
    MYSQL_ROW row = mysql_fetch_row(res);

    string name = "";
    if (row) {
        name = row[0];
    }

    mysql_free_result(res);
    return name;
}

void Customer::showCustomerInfoByID(MYSQL *conn) {
	string customer_id;
    cout << "Enter customer ID: ";
    cin >> customer_id;
    string query = "SELECT customer_id, name, address, phone, email FROM Customers WHERE customer_id = '" + customer_id + "'";

    if (mysql_query(conn, query.c_str())) {
        cerr << "Error fetching customer details: " << mysql_error(conn) << endl;
        return;
    }

    MYSQL_RES *res = mysql_store_result(conn);
    MYSQL_ROW row = mysql_fetch_row(res);

    if (!row) {
        cout << "No customer found with ID: " << customer_id << endl;
    } else {
        // Print customer details
        cout << "\nCustomer Details:" << endl;
        cout << "Customer ID: " << row[0] << endl;
        cout << "Name: " << row[1] << endl;
        cout << "Address: " << row[2] << endl;
        cout << "Phone: " << row[3] << endl;
        cout << "Email: " << row[4] << endl;
    }

    mysql_free_result(res);
}



