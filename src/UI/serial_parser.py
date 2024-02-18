import re
# function for reading in serial data from module 5 in the form of a hex value
def parser(data: str):

    # p_id_idx = 0
    mod_idx = 0 #1
    cell_idx = 1 #2

    # remove any characters from data that aren't numeric or '.'
    data = re.sub(r'[^0-9.]', '', data)

    # p_id = data[p_id_idx]
    mod = data[mod_idx]
    cell = data[cell_idx]
    res = data[2:]

    # if p_id == 7:
        # convert res to binary
    res = format(int(res, 16), '08b')  # returns binary value as a string

    # return p_id, mod, cell, res
    return mod, cell, res

