#ifndef HOTELMANAGER_H
#define HOTELMANAGER_H

#include <string>
#include "Room.h"
#include "Customer.h"
#include "Reservation.h"

class HotelManager {
private:
    std::string manager_name;

public:
	HotelManager(std::string name);
    void addCustomer(Customer customer);
    void addReservation(MYSQL *conn);
    void cancelReservation(MYSQL *conn);
    void viewReservations(MYSQL *conn);
    void viewPaymentStatus();
    void addRoom(std::string type, MYSQL *conn); 
    void getRoomCountByType(MYSQL *conn);
    void updateReservationPaymentStatus(MYSQL *conn);
    void getCustomerInfo(MYSQL *conn);
};

#endif // HOTELMANAGER_H
