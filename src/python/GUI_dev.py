import serial

# Define the serial port settings
SERIAL_PORT = 'COM3'  # Adjust this according to your system
BAUD_RATE = 9600  # Match this with your STM32 board configuration

# Open serial port
ser = serial.Serial(SERIAL_PORT, BAUD_RATE)

try:
    while True:
        # Read data from serial port
        data = ser.readline().strip().decode('utf-8')

        # Check if the button is pressed
        if data == "Button Pressed":
            print("Button pressed!")

except KeyboardInterrupt:
    # Close the serial port when Ctrl+C is pressed
    ser.close()
