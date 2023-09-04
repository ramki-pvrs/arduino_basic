//RAMKI goto Sketch-Include Libraries - Manage Libraries - search MySQL and click Install
// also install TrueRandom.zip file
//https://nicegear.nz/blog/autogenerated-random-persistent-mac-address-for-arduino-ethernet/
/*
  MySQL Connector/Arduino Example : basic insert

  This example demonstrates how to issue an INSERT query to store data in a
  table. For this, we will create a special database and table for testing.
  The following are the SQL commands you will need to run in order to setup
  your database for running this sketch.

  CREATE DATABASE test_arduino;
  CREATE TABLE test_arduino.hello_arduino (
    num integer primary key auto_increment,
    message char(40),
    recorded timestamp
  );

  Here we see one database and a table with three fields; a primary key that
  is an auto_increment, a string, and a timestamp. This will demonstrate how
  to save a date and time of when the row was inserted, which can help you
  determine when data was recorded or updated.

  INSTRUCTIONS FOR USE

  1) Create the database and table as shown above.
  2) Change the address of the server to the IP address of the MySQL server
  3) Change the user and password to a valid MySQL user and password
  4) Connect a USB cable to your Arduino
  5) Select the correct board and port
  6) Compile and upload the sketch to your Arduino
  7) Once uploaded, open Serial Monitor (use 115200 speed) and observe
  8) After the sketch has run for some time, open a mysql client and issue
     the command: "SELECT * FROM test_arduino.hello_arduino" to see the data
     recorded. Note the field values and how the database handles both the
     auto_increment and timestamp fields for us. You can clear the data with
     "DELETE FROM test_arduino.hello_arduino".

  Note: The MAC address can be anything so long as it is unique on your network.

  Created by: Dr. Charles A. Bell

  Ramki:
    - MySQL in Acer is not working with user id and password 
    - Error: Client does not support authentication protocol requested by server; consider upgrading MySQL client
    - so switched to Maria db 192.168.0.103  Docker instance port 6606 
    - cur_mem -> execute command in setup for testing only; if you want to continuously update/insert to dB
     - code should be in loop block

     - In IDE version 2.2 MySQL_MariadB_Generic was not working
     - installed 1.8.19 and loaded MySQL_Connector lib and through that able to connect to MariaDB
     
     Success Message in serial port is
     Connecting...
...trying...
...got: 0 retrying...
...trying...
Connected to server version 5.5.5-10.10.2-MariaDB-1:10.10.2+maria~ubu2204
Recording data.

*/

#include <Ethernet.h>
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>

byte mac_addr[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

IPAddress mariadb_docker_server_addr(192,168,0,103);  // IP of the MySQL *server* here Ramki: docker instance in ACER
int mariadb_docker_port = 6606;
char user[] = "root";              // MySQL user login username
char password[] = "root";        // MySQL user login password

// Sample query
char INSERT_SQL[] = "INSERT INTO test_arduino.hello_arduino (message) VALUES ('Hello, Arduino!')";

EthernetClient client;
MySQL_Connection conn((Client *)&client);

void setup() {
  Serial.begin(9600);
  while (!Serial); // wait for serial port to connect
  Ethernet.begin(mac_addr);
  Serial.println("Connecting...");
  if (conn.connect(mariadb_docker_server_addr, mariadb_docker_port, user, password)) {
    delay(1000);
  }
  else
    Serial.println("Connection failed.");
  
  Serial.println("Recording data.");

  // Initiate the query class instance
  MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);
  // Execute the query
  cur_mem->execute(INSERT_SQL);
  // Note: since there are no results, we do not need to read any data
  // Deleting the cursor also frees up memory used
  delete cur_mem;
}

void loop() {
 
}


