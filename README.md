# eToll-System

## Problem statement:
Design and implement an Arduino based Toll tax collection system utilizing RFID technology for payment and Infrared Sensor for vehicle detection.

## Components used:

### Hardware
![image](https://github.com/user-attachments/assets/fde58564-8213-4a5c-978c-a998f1ed2210)

### Software
Arduino IDE -The Arduino Integrated Development Environment - or Arduino
Software (IDE) - contains a text editor for writing code, a message area, a text console, a
toolbar with buttons for common functions and a series of menus. It connects to the Arduino
hardware to upload programs and communicate with them


## Circuit Diagram
![image](https://github.com/user-attachments/assets/6eda41f8-beb7-4243-83e5-9f4aa41e0783)

### Working
When a vehicle passes through the toll plaza, the RFID reader reads the RFID tag on the car and sends the data to the Arduino.The Arduino then uses the data to retrieve the necessary information from the central database, such as the toll fee and the vehicle owner’s account balance.In our Mini project, the RFID reader checks if the access is enabled or not for the particular tag.If the vehicle owner has sufficient balance, the Arduino deducts the toll fee from the account balance and opens the toll gate.If the balance is insufficient, the Arduino denies access and alerts that the owner’s account has insufficient balance
