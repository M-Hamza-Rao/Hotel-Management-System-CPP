🏨 Hotel Management System (C++)
📖 Overview
This is a simple Hotel Management System developed in C++ using Dev-C++. The application allows hotel staff to manage customer data, room reservations, and bookings. It connects to a MySQL database using libmysql.dll for persistent data storage...

📂 Project Structure
File/Folder	Description
Customer.cpp/.h	Customer-related class implementation and header file.
HotelManager.cpp/.h	Manages hotel operations and administration logic.
Reservation.cpp/.h	Handles room reservation logic and functionalities.
Room.cpp/.h	Manages room information and availability.
main.cpp	Program entry point. Integrates and runs the system.
libmysql.dll	Required for MySQL database connection.
Makefile.win	Build configuration file for Dev-C++.
Hotel Management System.dev	Dev-C++ project file.
DataBase_SS.png	Screenshot of the database schema.
entryPage.ss	Entry page screenshot or resources.

🛠️ Features
📋 Customer Management (Add, View, Modify, Delete)

🛏️ Room Management (Availability, Details)

📅 Reservation Handling (Booking, Cancellation)

📑 Persistent Data Storage via MySQL

🖥️ Simple Console-based User Interface

📊 Database connection via libmysql.dll

💾 Requirements
Dev-C++ (or any C++ compiler supporting C++11+)

MySQL Server

libmysql.dll (included)

Basic knowledge of SQL (for database setup)

🚀 How to Run
Clone or download the project files.

Set up a MySQL database according to the provided DataBase_SS.png.

Update database connection details in the source files if necessary.

Open Hotel Management System.dev in Dev-C++.

Compile the project using the included Makefile.win.

Run the executable Hotel Management System.exe.

📸 Screenshots
Database schema: DataBase_SS.png

Entry page UI: entryPage.ss

📌 Notes
Make sure libmysql.dll is in the executable directory.

The project was built and tested in a Windows environment.

Use Dev-C++ for compatibility with Makefile.win and .dev project files.

📄 License
This project is for educational purposes only.
