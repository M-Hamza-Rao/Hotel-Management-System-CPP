#include <iostream>
#include <mysql.h>
#include <mysqld_error.h>
#include "Room.h"
#include "HotelManager.h"

using namespace std;

int main() {
    MYSQL *conn;

    // MySQL database credentials
    const char *host = "127.0.0.1";
    const char *user = "root";
    const char *password = "1234";
    const char *dbname = "mydb";

    // Initialize MySQL
    conn = mysql_init(NULL);
    if (conn == NULL) {
        cerr << "mysql_init() failed!" << endl;
        return EXIT_FAILURE;
    }

    if (mysql_real_connect(conn, host, user, password, dbname, 0, NULL, 0) == NULL) {
        cerr << "mysql_real_connect() failed: " << mysql_error(conn) << endl;
        mysql_close(conn);
        return EXIT_FAILURE;
    }

    cout << "Connected to database successfully!" << endl;
    
    
    HotelManager manager("Hamza");
    
    /*
    for(int i=0;i<15;i++){
    	manager.addRoom("D", conn);
	}
	for(int i=0;i<10;i++){
    	manager.addRoom("S", conn);
	}
	for(int i=0;i<5;i++){
    	manager.addRoom("P", conn);
	}
	*/
	//manager.updateReservationPaymentStatus(conn);
	
	//manager.addReservation(conn);
	//manager.getRoomCountByType(conn);
	
	//manager.cancelReservation(conn);

    mysql_close(conn);

    return EXIT_SUCCESS;
}
