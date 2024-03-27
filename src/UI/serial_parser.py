import re
import serial

# function for reading in serial data from module 5 in the form of a hex value
def parser(data: str):

    p_id_idx = 0
    mod_idx = 1
    cell_idx = 2

    # remove any characters from data that aren't numeric or '.'
    data = re.sub(r'[^0-9A-Fa-f.]', '', data)

    p_id = data[p_id_idx]
    mod = data[mod_idx]
    cell = data[cell_idx]
    res = data[3:]

    hex_map = {'A': 10, 'B': 11, 'C': 12, 'D': 13, 'E': 14, 'F': 15}
    if cell.upper() == 'A' or cell.upper() == 'B' or cell.upper() == 'C' or cell.upper() == 'D' or cell.upper() == 'E' or cell.upper() == 'F':
        cell = hex_map.get(cell.upper())

    if p_id == '7':
        # if the packet ID indicates a fault, then convert res to binary
        res = format(int(res, 16), '08b')  # returns binary value as a string
    else:
        # otherwise, for all other packet IDs, convert res to decimal
        # Split the string into whole and fractional parts
        whole_part, fractional_part = res.split('.')

        # Convert whole part from hexadecimal to decimal
        whole_decimal = int(whole_part, 16)

        # Convert fractional part from hexadecimal to decimal
        fractional_decimal = int(fractional_part, 16) / (16 ** len(fractional_part))

        # Combine whole and fractional parts
        res = whole_decimal + fractional_decimal

    # print(data)

    return int(p_id), int(mod), int(cell), res

# note: this code below is here to test just the parser
#
# def main():
#     # Connect to the COM port at the specified baud rate
#     com_port = "COM5"  # Change this to your COM port
#     baud_rate = 9600  # Change this to your desired baud rate
#     ser = serial.Serial(com_port, baud_rate)
#     print(f"Connected to {com_port} at {baud_rate} baud rate.")
#
#     try:
#         while True:
#             # Read data from the serial port
#             data = ser.readline().decode().strip()
#
#             # Call the parser function to process the data
#             parsed_data = parser(data)
#             print("Parsed data:", parsed_data)
#
#     except KeyboardInterrupt:
#         print("Keyboard Interrupt. Closing serial connection.")
#         ser.close()
#
# if __name__ == "__main__":
#     main()