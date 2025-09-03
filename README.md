# 🔥 Capstone Project – MCU-Based Charcoal Briquette Machine

## 📖 Overview
This project was developed as our **Capstone Project** in BS Computer Engineering.  
We designed and built an **MCU-Based Charcoal Briquette Machine** that automates the process of **grinding, mixing, compressing, and forming charcoal briquettes**.  

The system reduces manual labor, improves efficiency, and supports the use of **sustainable fuel alternatives** for communities.

---

## ✨ Key Features
- 🔄 Automated **charcoal feeding, grinding, mixing, and compression**
- ⚖️ **Load cell integration** (real-time weight monitoring)
- 📡 **Ultrasonic sensors** for starch & water level detection
- 🖥️ **LCD I2C display** for live system feedback
- 🎛️ **Servo motor & linear actuator** for controlled dispensing
- ⚡ **Relays** for pump and DC motor control
- 🚨 **Safety indicators**: LEDs & buzzer alerts

---

## ⚙️ Hardware & Software
- **Microcontroller:** Arduino Mega (ATmega2560)  
- **Sensors & Actuators:** IR sensor, servo motor, stepper motors, HX711 load cell, linear actuator, ultrasonic sensors, relays, LCD I2C  
- **Programming Language:** C/C++ (Arduino IDE)  
- **Design Tools:** AutoCAD (2D design), SketchUp (3D design)  

---

## 🖼️ System Diagrams
### 🔲 Block Diagram
🔲[Block Diagram](Block_Diagram.png)

### 🔄 Flowchart
🔄[Flowchart](Flow_Chart.png)

---

## 🏗️ Prototype & Designs
- 📐 **3D Model (SketchUp):**  
  📐[Prototype 3D Design](Prototype)

- ⚙️ **Actual Prototype:**  
  ⚙️[Prototype](Actual_Prototype)

---

## 💻 Firmware (Arduino Code)
The complete firmware is available here:  
👉 [`/firmware/charcoal_briquette_machine.ino`](Finals_codes1desu)

For documentation purposes, we also included the original Word file:  
👉 [`C++_Capstone_Code.docx`](C++_Capstone_Code.docx)


```cpp
// IR Sensor + Servo Feeder
int sensorValue = digitalRead(IR_SENSOR);

if (sensorValue == LOW) { // Object detected
    feeder.write(0);      // Open feeder
    delay(500);
    feeder.write(90);     // Return to idle
    delay(500);
}
