## Design and Development of Online Railway Ticket Booking System
The project aims to develop a ticket booking system that is user-friendly and multi-functional. The project has following functionalities: 
1. All account transactional details and customer information are stored in files.
2. Account holders have to pass through a login system to enter their accounts.
3. The application possess a password-protected administrative access; thus preventing the whole management system from unauthorized access.
4. Three types of accounts are created: normal account, agent account(multiple simultaneous logins), administrator.
5. The admin is able to add, delete, modify, search for a specific account and train details.
6. When the customer connects to the server, he gets a login and password prompt. After successful login, he gets the menu for •Book Ticket
•View Previous Bookings
•Update Booking
•CancelBooking
7. If Agent account holder logs in, proper file locking is implemented. For example, if 'booking details' is required then read lock is set. If ' new booking' or 'modify booking' is selected, then write lock is implemented to protect the critical data section.
8. Socket programming is used, i.e. Server maintains the database and service multiple clients concurrently. Client program can connect to the server and access their specific account details.
9. Linux system calls are used instead of Library functions in the project for Process Management, File Management, File Locking, Multithreading and Inter Process Communication Mechanisms.


### Code Walkthrough:
1. Run reset.sh to create essential data files.
2. To create object file of server and client.
3. Run the server.
4. Run the client.
5. Sign up and create one admin account. The secret pin for admin account is "sri".
6. Login into admin account and add trains.
7. Create customers/agents and login to book ticket(s) as required.
