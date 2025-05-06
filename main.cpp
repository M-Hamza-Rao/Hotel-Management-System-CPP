#include <iostream>
#include <mysql.h>
#include <mysqld_error.h>
#include "Room.h"
#include "HotelManager.h"
#include <cstdlib>
#include <unistd.h>

using namespace std;

void displayMenu() {
    cout << "==================== Hotel Management System ====================" << endl;
    cout << "1. Add Room" << endl;
    cout << "2. Update Reservation Payment Status" << endl;
    cout << "3. Add Reservation" << endl;
    cout << "4. Get Room Count by Type" << endl;
    cout << "5. Cancel Reservation" << endl;
    cout << "6. View Reservations" << endl;
    cout << "7. Get Customer Info" << endl;
    cout << "0. Exit" << endl;
    cout << "Please enter your choice: ";
}

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

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
    
    int choice;
    string t;

    while (true) {
    	cout << "Press Enter to continue...";
		cin.ignore();  
		cin.get();
    	clearScreen();
        displayMenu();
        cin >> choice;
        clearScreen();

        switch (choice) {
            case 1:
                
                cout << "Enter room type: ";
                cin >> t;
                manager.addRoom(t, conn);
                break;

            case 2:
                manager.updateReservationPaymentStatus(conn);
                break;

            case 3:
                manager.addReservation(conn);
                break;

            case 4:
                manager.getRoomCountByType(conn);
                break;

            case 5:
                manager.cancelReservation(conn);
                break;

            case 6:
                manager.viewReservations(conn);
                break;

            case 7:
                manager.getCustomerInfo(conn);
                break;

            case 0:
                cout << "Exiting the system." << endl;
                return EXIT_SUCCESS;

            default:
                cout << "Invalid choice, please try again." << endl;
                break;
        }

        
    }
}
