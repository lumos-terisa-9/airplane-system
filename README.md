# airplane-system
A simple airplane ticket booking system

//Computer Architecture Design (Technologies and Platforms Used)
//Programming Environment: QT Creator
//Programming Language: C++
//Database Used: SQLite
　
1. Business Requirements Analysis
1) Main Requirements
Protect User Information: Ensure that users can log in to their own accounts correctly and access their account data without reading data from other users or having their own data read by others, to prevent user information leakage.
Display Flight Information and Purchase Tickets: The program should be able to correctly read and display flight information and search for flights based on user-specified times and locations. Users should be able to select flights and choose corresponding passengers, seats, and other information.
Query Orders: Users should be able to view orders they have paid for, as well as orders purchased by others on their behalf, i.e., they can view orders associated with their account. They should not be able to view orders not associated with their account to protect user privacy.
Reschedule and Refund: Sometimes users' itineraries may change. The program should provide functions for rescheduling and refunding tickets to make travel arrangements more convenient and flexible.

3) Secondary Requirements
Log Out: Logging out promptly after use can effectively enhance account security.
Password Recovery: Password loss is a common issue. To facilitate user access to the program, a password recovery function should be provided.
Change Password: Changing the user password at a certain frequency can enhance account security.
Edit Passenger Information: Passenger information is not static, such as age and phone number, which may change. The program should provide a function to edit passenger information.

2. Basic Module Functions
1) Login, Registration, and Password Recovery
If a user has already registered an account, they can directly log in through the login interface. If not registered, users can click the “register” button to enter the registration interface, complete their personal information, create a personal account, and then return to the login interface to log in. If a user enters the wrong password multiple times during login, the window will remind them and pop up a “password recovery” page. Users can recover their account password by verifying their personal identity information and then return to the login interface to try again.

2) Query, Purchase Tickets, and Payment
After successful login, users will be directed to the homepage. On the homepage, users can input or select departure and destination locations, as well as departure times, to search for corresponding flights. After searching, users can select a flight and proceed to the order form page. On this page, users need to fill in and select passengers, check seats, and choose a payment method. After confirming the order information, users will enter the payment interface. After clicking “pay,” the order information will be saved in both the user’s and the corresponding passenger’s accounts for future reference. Subsequently, all other pages will be closed.

3) Rescheduling, Refunding, and Re-Payment
Considering that users' itineraries may change, we have provided functions for rescheduling and refunding tickets. Users can reselect flights and change departure times while keeping the departure and destination locations unchanged, allowing for flexible travel. If a user no longer needs the trip, they can also choose to refund the ticket. Considering that users may need to confirm their itinerary before payment and cannot pay immediately, unpaid orders will be temporarily saved, waiting for the user to complete the payment later.

5) View Account Information
On the “My” page, users can view “My Passengers” and “My Orders,” and they can also change their account password.

6) Add, Edit, and Delete Passengers
Considering that users may purchase tickets for multiple passengers at once, we have added an option to select passengers when filling in the order. Therefore, the function to add, edit, and delete passenger information is also important. On the passenger information page, users can add passenger information and make changes for passengers whose information has changed.

7) Log Out
After the user selects to log out, the program will return to the login interface.

3. Detailed Functional Design and Database Dictionary
a. Detailed Functional Design
i. Login, Registration, and Password Recovery
Login: Users input their username and password. The system verifies the information and allows login if correct.
Registration: Users fill in their username, password, and other information. The system verifies the information and creates a new account if correct.
Password Recovery: Users verify their identity by inputting personal information. The system recovers the user’s password based on the personal information.
ii. Flight Query, Ticket Purchase, and Payment
Flight Query: Users input departure location, destination, and date information. The system queries the database for matching flight information and displays it.
Ticket Purchase: Users select a flight, choose passengers, and check seats. The system generates an order.
Payment: Users pay for the order. The system detects the completion of the payment process, updates the order status, and adds the order information to the user’s account.
iii. Rescheduling, Refunding, and Re-Payment
Rescheduling: Users can view order details in “My Orders.” If they need to reschedule, they click the “Reschedule” button. The system displays flights between the departure and destination locations, allowing users to select a different flight time to change their itinerary.
Refunding: If users no longer need the trip, they can click the “Refund” button on the order details page to cancel the order.
Re-Payment: When viewing the details of an unpaid order, users can click the “Confirm Payment” button to proceed with payment if they decide to proceed with the trip. Alternatively, they can click “Cancel Order” to refund the ticket if they decide not to proceed.
iv. Viewing and Modifying Account Information
My Passengers: Users can view and manage the information of added passengers and add new passenger information.
My Orders: Users can view order information, including orders they paid for and those purchased by others on their behalf.
Change Password: After verifying their identity, users can change their account password.
v. Log Out
Log Out: Users click the log-out button. The system clears the current user’s login status and returns to the login interface.
b. Database Dictionary
i. Cities Table
City_ID: City ID, primary key
City_Name: City name
ii. Users Table
User_ID: User ID, primary key
User_Name: Username
User_Pwd: User password
ID_Number: User ID number
iii. Passengers Table
Passenger_ID: Passenger ID, primary key
Name: Passenger name
ID_Number: Passenger ID number
Age: Passenger age
Gender: Passenger gender
Phone_Number: Passenger phone number
User_Name: Username associated with the passenger (which user added this passenger)
iv. Orders Table
Order_ID: Order ID, primary key
Flight_ID: Flight ID
BeginTime: Departure time
User_Name: Username who paid for the order
ID_Number: Passenger ID number
Price: Order price
Seat: Seat number
State: Order status (Paid/Pending Payment)
v. Flights Table
Flight_ID: Flight ID, primary key
BeginLocation: Departure location
EndLocation: Destination
BeginTime: Departure time
EndTime: Arrival time
3. Object Abstraction and Class Hierarchy Diagram (UML)
a. User
Attributes:
User_ID
User_Name
User_Pwd
ID_Number
Behaviors:
login
register
retrievePassword
changePassword
viewOrders
managePassengers
b. Passenger
Attributes:
Passenger_ID
Name
ID_Number
Age
Gender
Phone_Number
User_Name
Behaviors:
addPassenger
updatePassenger
deletePassenger
c. Flight
Attributes:
Flight_ID
BeginLocation
EndLocation
BeginTime
EndTime
Behaviors:
searchFlights
getFlightInfo
d. Order
Attributes:
Order_ID
Flight_ID
BeginTime
User_Name
ID_Number
Price
Seat
State
Behaviors:
createOrder
payOrder
updateOrderState
