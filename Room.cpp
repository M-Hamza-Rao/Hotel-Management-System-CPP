#include "Room.h"
#include "Room.h"
#include <iostream>
#include <string>
#include <mysql.h>
using namespace std;

#include <stdexcept>  

Room::Room(string type) : room_id(0), status("free") {

    if (type == "P") {
    	room_type = "Premium Suite";
        cost_per_night = 50.0;
    } else if (type == "S") {
    	room_type = "Super Delux";
        cost_per_night = 35.0;
    } else if (type == "D") {
    	room_type = "Delux";
        cost_per_night = 20.0;
    } else {
        throw std::invalid_argument("Invalid room type provided: " + room_type);
    }
}

void Room::displayRoomInfo() {
    cout << "Room ID: " << room_id << ", Type: " << room_type 
         << ", Cost: " << cost_per_night << " Euros, Status: " << status << endl;
}

void Room::changeStatus(string new_status) {
    status = new_status;
}

void Room::addToDatabase(MYSQL *conn) {
    string query = "INSERT INTO Rooms (room_type, cost_per_night, status) VALUES ('" 
                + room_type + "', " + to_string(cost_per_night) + ", '" + status + "')";

    if (mysql_query(conn, query.c_str())) {
        cerr << "Room insert failed: " << mysql_error(conn) << endl;
    } 
}

void Room::setStatus(MYSQL *conn, int room_id, string new_status) {
    std::string query = "UPDATE Rooms SET status = '" + new_status + "' WHERE room_id = " + std::to_string(room_id);
    if (mysql_query(conn, query.c_str())) {
        std::cerr << "Error updating room status: " << mysql_error(conn) << std::endl;
    } else {
        std::cout << "Room status updated to '" << new_status << "' for room ID " << room_id << " in database." << std::endl;
    }
}



