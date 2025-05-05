#include "HotelManager.h"
#include <iostream>
using namespace std;

HotelManager::HotelManager(string name) {
    manager_name = name;  // Initialize the manager's name
    cout << "Hotel Manager " << manager_name << " is now managing the hotel." << endl;
}


void HotelManager::viewReservations() {
	//View Reservation
    }
    
void HotelManager::addRoom(string type, MYSQL *conn) {
    try {
        Room newRoom(type);
        newRoom.addToDatabase(conn);

        cout << "Room of type " << type << " added successfully!" << endl;
    } catch (const std::invalid_argument& e) {
        cerr << "Error: " << e.what() << endl;  
    }
}

void HotelManager::updateReservationPaymentStatus(MYSQL *conn) {
    std::string customer_id;
    
    cout << "Enter the customer ID to update payment status: ";
    cin >> customer_id;

    string status = "paid";

    Reservation::updatePaymentStatus(conn, customer_id, status);
}

void HotelManager::cancelReservation(MYSQL *conn) {
    int room_id;
    cout << "Enter the room id for which you want to cancel the reservation: ";
    cin >> room_id;

    Reservation::removeReservation(conn, room_id);
}


void HotelManager::addReservation(MYSQL *conn) {
    // heck availability
    string room_type;
    cout << "Enter the room type to reserve (P: Premium Suite, S: Super Delux, D: Delux): ";
    cin >> room_type;

    // Check if the room type is valid
    if (room_type != "P" && room_type != "S" && room_type != "D") {
        cout << "Invalid room type!" << endl;
        return;
    }

    // Finding the next available room of that type
    string query = "SELECT room_id FROM Rooms WHERE room_type = '";
    query += (room_type == "P" ? "Premium Suite" : (room_type == "S" ? "Super Delux" : "Delux"));
    query += "' AND status = 'free' LIMIT 1"; 

    if (mysql_query(conn, query.c_str())) {
        cerr << "Error querying rooms: " << mysql_error(conn) << endl;
        return;
    }

    MYSQL_RES *res = mysql_store_result(conn);
    MYSQL_ROW row = mysql_fetch_row(res);
    if (!row) {
        cout << "No available rooms of this type." << endl;
        mysql_free_result(res);
        return;
    }

    //get room_id
    int room_id = atoi(row[0]);

    //Customer details
    string name, address, phone, email, passport_number;
    
    cout << "Enter customer passport number (ID): ";
    cin >> passport_number;
    
    cout << "Enter customer name: ";
    cin.ignore();
    getline(cin, name);
    
    cout << "Enter customer address: ";
    getline(cin, address);
    
    cout << "Enter customer phone: ";
    getline(cin, phone);
    
    cout << "Enter customer email: ";
    getline(cin, email);
    
    //Checking if customer already exists by passport_number (cus_id)
    query = "SELECT customer_id FROM Customers WHERE customer_id = '" + passport_number + "'";
    if (mysql_query(conn, query.c_str())) {
        cerr << "Error checking customer: " << mysql_error(conn) << endl;
        return;
    }

    res = mysql_store_result(conn);
    row = mysql_fetch_row(res);

    string cus_id;
    if (row) {
        //get customer_id
        cus_id = row[0];
        mysql_free_result(res);
    } else {
        //add new customer
        Customer new_customer(passport_number, name, address, phone, email);
        new_customer.addCustomer(conn); 
        cus_id = passport_number;  
        mysql_free_result(res);
    }

    //Create new reservation
    string check_in, check_out, payment_status;
    double total_amount;

    cout << "Enter check-in date (YYYY-MM-DD): ";
    cin >> check_in;

    cout << "Enter check-out date (YYYY-MM-DD): ";
    cin >> check_out;

	cout << "Enter total amount: ";
    cin >> total_amount;
    
    cout << "Enter payment status (Paid/Unpaid): ";
    cin >> payment_status;

    // Create a Reservation 
    Reservation reservation(0, cus_id, room_id, check_in, check_out, payment_status, total_amount);
    reservation.makeReservation(conn);

}

void HotelManager::getRoomCountByType(MYSQL *conn) {
    string room_types[] = {"Premium Suite", "Super Delux", "Delux"};
    int count;
    
    for (const string &room_type : room_types) {
        string query = "SELECT COUNT(*) FROM Rooms WHERE room_type = '" + room_type + "' AND status = 'free'";

        if (mysql_query(conn, query.c_str())) {
            cerr << "Error querying room count: " << mysql_error(conn) << endl;
            return;
        }

        MYSQL_RES *res = mysql_store_result(conn);
        MYSQL_ROW row = mysql_fetch_row(res);

        if (row) {
            count = atoi(row[0]); 
            cout << "Available " << room_type << " rooms: " << count << endl;
        } else {
            cout << "Error fetching count for " << room_type << " rooms." << endl;
        }

        mysql_free_result(res);  
    }
}
