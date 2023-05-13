import serial
# import pandas as pd
import serial.tools.list_ports
import time
from datetime import datetime
import logging as log
log.basicConfig(format='%(asctime)s %(message)s', level=log.INFO)


ROSALIA_BAUD = 115200
GS_BAUD = 19200

GS_TYPE = 0
ROSALIA_TYPE = 1

def show_serial_ports() -> list:
    ports = serial.tools.list_ports.comports()
    prt = []
    for i, (port, desc, hwid) in enumerate(sorted(ports)):
        print("Number {}: {} {} [{}]".format(i, port, desc, hwid))
        prt.append(port)
    return prt

def create_dataframe(sender_type : int):
    if sender_type == GS_TYPE:
        send_buf = bytes('GS;NOW;FRAME num.' + str(i) + '\n', 'ascii')
    elif sender_type == ROSALIA_TYPE:
        send_buf = bytes('ROSALIA;FRAME num.' + str(i) + '\n', 'ascii')
    return send_buf

available_ports = show_serial_ports()
# print(available_ports)

sender_num = int(input('Insert the sender serial port: '))
receiver_num = int(input('Insert the receiver serial port: '))

total_time = int(input('Insert the desired test time in seconds: '))

type_int = int(input('Insert the type of sender: 0 for GS, 1 for ROSALIA: '))

if type_int == GS_TYPE:
    sender_baud = GS_BAUD
    receiver_baud = ROSALIA_BAUD
elif type_int == ROSALIA_TYPE:
    sender_baud = ROSALIA_BAUD
    receiver_baud = GS_BAUD

sender_serial = serial.Serial(available_ports[sender_num], sender_baud)
receiver_serial = serial.Serial(available_ports[receiver_num], receiver_baud)

i = 0
good_frames = 0
bad_frames = 0

receive_buf = ''

while_start = time.time()
while sender_serial.is_open and receiver_serial.is_open and time.time() - while_start < total_time:
    send_buf = create_dataframe(type_int)
    sender_serial.write(send_buf)
    log.info('Writing %s', send_buf)
    time.sleep(1.)
    try:
        receive_buf = receiver_serial.readline()
        if receive_buf == send_buf:
            log.info('Good frame received: %s', receive_buf)
            good_frames += 1
        else:
            log.info('Bad frame received %s:', receive_buf.decode('ascii'))
            bad_frames += 1
    except:
        log.info('Frame not received')
    i += 1

print('\n Total number of sent frames: %d' % i)
good_frames_perc = float(good_frames / i)
print('Good frames: %d = %.2f %% of total frames' %
      (good_frames, good_frames_perc * 100.))
bad_frames_perc = float(bad_frames / i)
print('Bad frames: %d = %.2f %% of total frames' %
      (bad_frames, bad_frames_perc * 100.))
