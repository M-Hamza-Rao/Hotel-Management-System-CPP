#ifndef ROOM_H
#define ROOM_H

#include <iostream>
#include <string>
#include <mysql.h>

class Room {
private:
    int room_id;
    std::string room_type;
    double cost_per_night;
    std::string status;

public:
	static void setStatus(MYSQL *conn, int room_id, std::string new_status);
    Room(std::string type);
    void displayRoomInfo();
    void changeStatus(std::string new_status);
    void addToDatabase(MYSQL *conn);
    int getRoomID();
    std::string getStatus();
};

#endif // ROOM_H
