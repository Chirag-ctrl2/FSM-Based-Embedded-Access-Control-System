# FSM-Based Embedded Access Control System 🔐

An Arduino-based embedded door lock system built using a **Finite State Machine (FSM)**.  
The system authenticates users via a keypad, stores credentials in EEPROM, controls a servo-based locking mechanism, and enforces a security lockout after failed attempts.

---

## ✨ Features
- Finite State Machine (FSM)–based system design  
- 4×4 keypad PIN authentication  
- Masked PIN entry on LCD  
- EEPROM-based password persistence  
- Servo-controlled door lock  
- Lockout mechanism after multiple failed attempts  
- Non-blocking timing using `millis()`

---

## 🧠 FSM Design

The system behavior is structured around a Finite State Machine with the following states:

| State | Purpose |
|------|--------|
| **IDLE** | Waiting for user input |
| **INPUT** | Collecting keypad characters |
| **VERIFY** | Comparing entered PIN with stored password |
| **ERROR** | Handling incorrect PIN attempts |
| **LOCKED_OUT** | Temporary lockout after repeated failures |

FSM-based design ensures deterministic control flow, improved reliability, and easier debugging compared to ad-hoc logic.

---

## ⌨️ Keypad Controls

The keypad includes two special control keys that influence system behavior:

### `#` (Enter / Confirm)
- Submits the entered PIN for verification
- If pressed without entering any digits, the system prompts the user to enter a PIN
- Transitions the FSM from **INPUT → VERIFY**

### `*` (Clear / Reset)
- Clears the current PIN input buffer
- Cancels the current input operation
- Returns the system to the **IDLE** state

These control keys improve usability and prevent accidental or partial PIN verification.

---

## 🛠️ Hardware Components
- Arduino Uno  
- 4×4 Matrix Keypad  
- Servo Motor  
- I2C LCD Display  

---

## 🔐 Security Highlights
- Input buffer overflow protection  
- EEPROM-backed credential storage  
- Rate limiting using lockout after failed attempts  
- Non-blocking timing logic for lockout enforcement  

---

## 🔑 Default Access Credentials
- **Default PIN:** `1234`
- The PIN is stored in **EEPROM**, allowing persistence across power cycles
- The default PIN will be written only during initial EEPROM initialization if you use the initEEPROM() in the code and not comment it out.

> ⚠️ **Note:** Default credentials are provided strictly for demonstration and testing purposes.

---

## 🧪 Simulation & Testing

This project was developed and tested using **Tinkercad Circuits**.

🔗 **Tinkercad Simulation:**  
https://www.tinkercad.com/things/4qVpwspqtIz-dazzling-vihelmo

### Expected Behavior
- Enter correct PIN (`1234`) → door unlocks
- Enter incorrect PIN repeatedly → system enters lockout state
- During lockout → keypad input is ignored until timeout expires

---

## 📸 Project Screenshots

*(Screenshots from the Tinkercad simulation — stored in the `images/` directory)*

### Circuit Overview
![Circuit Overview](images/circuit_overview.png)

### Normal Operation
![Normal Operation](images/normal_operation.png)

### Lockout State
![Lockout State](images/lockout_state.png)

---

## 🚀 Future Improvements
- Password change mode
- EEPROM-backed lockout persistence
- Interrupt-driven keypad input
- Alarm / buzzer integration

---

## 👤 Author
**Chirag**
