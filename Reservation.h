#ifndef RESERVATION_H
#define RESERVATION_H
#include <mysql.h>
#include <string>

class Reservation {
private:
    int reservation_id;
    std::string customer_id;
    int room_id;
    std::string check_in_date;
    std::string check_out_date;
    std::string payment_status;
    double total_amount;

public:
    Reservation(int res_id, std::string cus_id, int room_id, std::string check_in, std::string check_out, std::string payment_status, double amount);
    void makeReservation(MYSQL *conn);
    static void removeReservation(MYSQL *conn, int room_id);
    static void updatePaymentStatus(MYSQL *conn, std::string customer_id, std::string status);
    void displayReservationInfo();
};

#endif // RESERVATION_H
