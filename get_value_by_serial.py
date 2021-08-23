import serial
import time

def main():
    # '/dev/tty****' is your port ID
    con=serial.Serial('/dev/tty****', 9600)
    print('connected.')
    while 1:
        str=con.readline() # byte code
        print (str.strip().decode('utf-8')) # decoded string

if __name__ == '__main__':
    main()
