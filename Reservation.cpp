#include "Reservation.h"
#include <iostream>
#include "Room.h"
#include <mysql.h>
#include <iomanip>
#include "Customer.h"
using namespace std;

Reservation::Reservation(int res_id, string cus_id, int room_id, string check_in, string check_out, string payment_status, double amount)
    : reservation_id(res_id), customer_id(cus_id), room_id(room_id), check_in_date(check_in), check_out_date(check_out),
      payment_status(payment_status), total_amount(amount) {}

void Reservation::makeReservation(MYSQL *conn) {
    string query = "INSERT INTO Reservations (customer_id, room_id, check_in_date, check_out_date, payment_status, total_amount, booking_date) "
               "VALUES ('" + customer_id + "', " + to_string(room_id) + ", '" + check_in_date + "', '" + check_out_date + "', '"
               + payment_status + "', " + to_string(total_amount) + ", CURDATE())";


    if (mysql_query(conn, query.c_str())) {
        cerr << "Error inserting reservation: " << mysql_error(conn) << endl;
    } else {
        cout << "Reservation made successfully!" << endl;
        
        Room::setStatus(conn, room_id, "booked");
    }
}


void Reservation::removeReservation(MYSQL *conn, int room_id) {
    string query = "UPDATE Reservations SET check_in_date = NULL, check_out_date = NULL, payment_status = 'cancelled' WHERE room_id = " + to_string(room_id);

    if (mysql_query(conn, query.c_str())) {
        cerr << "Error updating reservation status: " << mysql_error(conn) << endl;
        return;
    } else {
        cout << "Reservation canceled successfully!" << endl;
    }

    Room::setStatus(conn, room_id, "free");
}


void Reservation::updatePaymentStatus(MYSQL *conn, string customer_id, string status) {
    // status should be either 'paid' or 'unpaid'
    if (status != "paid" && status != "unpaid") {
        cerr << "Invalid payment status. Use 'paid' or 'unpaid'." << endl;
        return;
    }

    string query = "SELECT reservation_id FROM Reservations WHERE customer_id = '" + customer_id + "' AND payment_status = 'unpaid'";

    if (mysql_query(conn, query.c_str())) {
        cerr << "Error checking reservations: " << mysql_error(conn) << endl;
        return;
    }

    MYSQL_RES *res = mysql_store_result(conn);
    MYSQL_ROW row = mysql_fetch_row(res);

    if (!row) {
        cout << "No unpaid reservations found for customer ID: " << customer_id << endl;
        return;
    }

    query = "UPDATE Reservations SET payment_status = '" + status + "' WHERE customer_id = '" + customer_id + "' AND payment_status = 'unpaid'";

    if (mysql_query(conn, query.c_str())) {
        cerr << "Error updating payment status: " << mysql_error(conn) << endl;
    } else {
        cout << "Payment status updated to '" << status << "' for customer ID: " << customer_id << endl;
    }
}

void Reservation::getReservationByID(MYSQL *conn, int reservation_id) {
    string query = "SELECT * FROM Reservations WHERE reservation_id = " + to_string(reservation_id);

    if (mysql_query(conn, query.c_str())) {
        cerr << "Error fetching reservation: " << mysql_error(conn) << endl;
        return;
    }

    MYSQL_RES *res = mysql_store_result(conn);
    MYSQL_ROW row = mysql_fetch_row(res);

    if (!row) {
        cout << "No reservation found with ID: " << reservation_id << endl;
        return;
    }

    cout << "Reservation ID: " << row[0]
         << "\nCustomer ID: " << row[1]
         << "\nRoom ID: " << row[2]
         << "\nCheck-in Date: " << (row[3] ? row[3] : "-") 
		 << "\nCheck-out Date: " << (row[3] ? row[3] : "-")
         << "\nPayment Status: " << row[5]
         << "\nTotal Amount: " << row[6]
         << "\nBooking Date: " << row[7] << endl;

    mysql_free_result(res);
}

void Reservation::viewReservationsByCustomer(MYSQL *conn, string customer_id) {
    string query = "SELECT reservation_id, room_id, check_in_date, check_out_date, payment_status, total_amount, booking_date "
                   "FROM Reservations WHERE customer_id = '" + customer_id + "'";

    if (mysql_query(conn, query.c_str())) {
        cerr << "Error fetching reservations: " << mysql_error(conn) << endl;
        return;
    }

    MYSQL_RES *res = mysql_store_result(conn);
    if (!res) {
        cerr << "No reservations found for customer ID: " << customer_id << endl;
        return;
    }

    MYSQL_ROW row;
    int num_fields = mysql_num_fields(res);

    cout << "\nReservations for Customer ID: " << Customer::getCustomerNameByID(conn, customer_id) << endl;
    cout << "--------------------------------------------------------------------------------------------------------------------" << endl;
    cout << left << setw(15) << "Res. ID"
         << setw(10) << "Room ID"
         << setw(15) << "Check-in"
         << setw(15) << "Check-out"
         << setw(12) << "Status"
         << setw(12) << "Amount"
         << setw(15) << "Booking Date" << endl;
    cout << "--------------------------------------------------------------------------------------------------------------------" << endl;

    bool hasResults = false;
    while ((row = mysql_fetch_row(res))) {
        hasResults = true;
        cout << left << setw(15) << (row[0] ? row[0] : "-")
             << setw(10) << (row[1] ? row[1] : "-")
             << setw(15) << (row[2] ? row[2] : "-")
             << setw(15) << (row[3] ? row[3] : "-")
             << setw(12) << (row[4] ? row[4] : "-")
             << setw(12) << (row[5] ? row[5] : "-")
             << setw(15) << (row[6] ? row[6] : "-")
             << endl;
    }

    if (!hasResults) {
        cout << "No reservations found for this customer." << endl;
    }

    cout << "--------------------------------------------------------------------------------------------------------------------" << endl;

    mysql_free_result(res);
}

void Reservation::viewAllActiveReservations(MYSQL *conn) {
    string query = "SELECT reservation_id, customer_id, room_id, check_in_date, check_out_date, "
                   "payment_status, total_amount, booking_date FROM Reservations "
                   "WHERE payment_status != 'cancelled'";

    if (mysql_query(conn, query.c_str())) {
        cerr << "Error fetching reservations: " << mysql_error(conn) << endl;
        return;
    }

    MYSQL_RES *res = mysql_store_result(conn);
    MYSQL_ROW row;

    cout << "--------------------------------------------------------------------------------------------------------------------" << endl;
    cout << left << setw(15) << "Res. ID"
         << setw(15) << "Customer ID"
         << setw(10) << "Room ID"
         << setw(15) << "Check-in"
         << setw(15) << "Check-out"
         << setw(12) << "Status"
         << setw(12) << "Amount"
         << setw(15) << "Booking Date" << endl;
    cout << "--------------------------------------------------------------------------------------------------------------------" << endl;

    bool hasResults = false;
    while ((row = mysql_fetch_row(res))) {
        hasResults = true;

        cout << left << setw(15) << (row[0] ? row[0] : "-")         
             << setw(15) << (row[1] ? row[1] : "-")                 
             << setw(10) << (row[2] ? row[2] : "-")                
             << setw(15) << (row[3] ? row[3] : "-")                 
             << setw(15) << (row[4] ? row[4] : "-")                 
             << setw(12) << (row[5] ? row[5] : "-")                 
             << setw(12) << (row[6] ? row[6] : "-")                 
             << setw(15) << (row[7] ? row[7] : "-")                 
             << endl;
    }

    if (!hasResults) {
        cout << "No active reservations found!" << endl;
    }

    cout << "--------------------------------------------------------------------------------------------------------------------" << endl;
    mysql_free_result(res);
}


void Reservation::viewAllCancelledReservations(MYSQL *conn) {
    string query = "SELECT reservation_id, customer_id, room_id, check_in_date, check_out_date, "
                   "payment_status, total_amount, booking_date FROM Reservations "
                   "WHERE payment_status = 'cancelled'";

    if (mysql_query(conn, query.c_str())) {
        cerr << "Error fetching reservations: " << mysql_error(conn) << endl;
        return;
    }

    MYSQL_RES *res = mysql_store_result(conn);
    MYSQL_ROW row;

    cout << "--------------------------------------------------------------------------------------------------------------------" << endl;
    cout << left << setw(15) << "Res. ID"
         << setw(15) << "Customer ID"
         << setw(10) << "Room ID"
         << setw(15) << "Check-in"
         << setw(15) << "Check-out"
         << setw(12) << "Status"
         << setw(12) << "Amount"
         << setw(15) << "Booking Date" << endl;
    cout << "--------------------------------------------------------------------------------------------------------------------" << endl;

    bool hasResults = false;
    while ((row = mysql_fetch_row(res))) {
        hasResults = true;

        cout << left << setw(15) << (row[0] ? row[0] : "-")         
             << setw(15) << (row[1] ? row[1] : "-")                 
             << setw(10) << (row[2] ? row[2] : "-")                
             << setw(15) << (row[3] ? row[3] : "-")                 
             << setw(15) << (row[4] ? row[4] : "-")                 
             << setw(12) << (row[5] ? row[5] : "-")                 
             << setw(12) << (row[6] ? row[6] : "-")                 
             << setw(15) << (row[7] ? row[7] : "-")                 
             << endl;
    }

    if (!hasResults) {
        cout << "No active reservations found!" << endl;
    }

    cout << "--------------------------------------------------------------------------------------------------------------------" << endl;
    mysql_free_result(res);
}
