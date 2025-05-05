#include "Reservation.h"
#include <iostream>
#include "Room.h"
using namespace std;
#include <mysql.h>
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

