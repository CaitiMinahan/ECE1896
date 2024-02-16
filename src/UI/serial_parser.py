# function for reading in serial data from module 5 in the form of a hex value

# TODO: create a function named parser
    # TODO: check the given hex value and index of each of its digits (can over the length of the input hex value)
    # take in the hex value input and use a Python library to convert the hex -> binary
        # TODO: depending on the ID
        # (i.e., if and elif statements checking if the ID == 000 through 110)
        # NOTE: ID's for each input type
        # NOTE: hex values with ID's 000-100 are 27 bits in length, so the 3 digits representing the ID's would be input[26], input[25], input[24]
        # NOTE: hex values with ID's 101-110 are 19 bits in length, so the 3 digits representing the ID'S would be input[18], input[17], input[16]
        # 000 - VOLTAGE
        # 001 - CURRENT
        # 010 - TEMP
        # 011 - SOC
        # 100 - SOP
        # 101 - SOH
        # 110 - FAULTS
            # TODO: after checking the ID, read in the module number (modules 1-16)
            # (i.e., if and elif statements or switch case checking if module == 0000 through 1111 (0-F))
            # then, assign the module number to a variable (i.e., module_number = 1 through 16 depending on the condition)
            # NOTE: for ID'S 000-100, module = input[23], input[22], input[21], input[20]
            # NOTE: for I'D's 101-110, module = input[15], input[14], input[13], input[12]
                # TODO: after checking the module number, read in the cell number (1-12)
                # (i.e., if and elif statements or switch case checking if cell == 0000 through 1100 (0-C))
                # then, assign the cell number to a variable (i.e., cell_number = 1 through 12 depending on the condition)
                # NOTE: for ID'S 000-100, module = input[19], input[18], input[17], input[16]
                # NOTE: for I'D's 101-110, module = input[11], input[10], input[9], input[8]
                    # TODO: after checking the cell number, check if there is a decimal point (pertains to ID's 000 - 100)
                    # (i.e., use a python library for checking if the entire digits contains a decimal or not)
                        # TODO: if there is a decimal, assign the V, I, T, SOC or SOP to a variable containing the reported value
                        # read in the last portion of the input data and assign it to a variable
                        # (i.e., value = hex_to_decimal_value)
                        # NOTE: for ID'S 000-100, module = input[15], through input[0]
                        # TODO: if there is NOT a decimal, assign SOH or FAULTS to a variable containing the reported value
                        # read in the last portion of the input data and assign it to a variable
                        # (i.e., value = hex_to_decimal_value)
                        # NOTE: for I'D's 101-110, module = input[7] through input[0]
# TODO: return module_number, cell_number and value variables assigned above for the function to send to the GUI

