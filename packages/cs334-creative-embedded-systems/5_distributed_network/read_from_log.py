import OSC  # sudo pip install pyosc (works with py2)
import serial  # sudo pip install pyserial
import re
import gpiozero as gp
import random
import sys
import time
import json


def parseLogEntry(raw_input):
    try:
        input = json.loads(raw_input)
        return [input["msg"]]
    except Exception as e:
        return [raw_input]




def generateSimulatedMessage():
    val = random.randint(0, 4)

    """
        format:
        [NAME]--value--other_value--other_value
    """

    msg = ""
    if val == 0:
        # generate sarim value
        sensorval = random.randint(0, 1)
        if sensorval == 1:
            msg = "sarim--completed"
    elif val == 1:
        # generate daniel value
        sensorval = random.randint(0, 4095)
        msg = "daniel--%d" % sensorval
    elif val == 2:
        # generate sam value
        sensor1val = random.randint(0, 1500)
        sensor2val = random.randint(0, 1500)
        sensor3val = random.randint(0, 1500)
        msg = "sam--%d--%d--%d" % (sensor1val, sensor2val, sensor3val)
    elif val == 3:
        # generate sabrina value
        val1 = random.randint(0, 4095)
        val2 = 15
        # sabrina_sim_time += 1
        # if sabrina_sim_time > 15:
            # sabrina_sim_time = 0
        msg = "sabrina--%d--%d" % (val1, val2)
    elif val == 4:
        # generate alexi value
        sensor = random.randint(0, 40)
        on = random.randint(0, 1)
        if on:
            sensor = 4095

        msg = "alexi--%d" % sensor

    return [msg]


class Listener:
    osc_client = None

    def add_osc_connect(self, client_connection):
        self.osc_client = client_connection
        print(self.osc_client)

    def sendOSC(self, content):
        msg = OSC.OSCMessage()
        msg.setAddress("/wand")
        for c in content:
            msg.append(c)
        self.osc_client.send(msg)


def main():

    mode = sys.argv[1]

    c = OSC.OSCClient()
    c.connect(("127.0.0.1", 44444))

    listener = Listener()
    listener.add_osc_connect(c)

    if mode == "sim":
        # generate data
        while True:
            msg = generateSimulatedMessage()

            try:
                listener.sendOSC(msg)
            except Exception as e:
                print(e)

    else:
        file = open("mesh_logs.log", "r")
        while True:
            where = file.tell()
            line = file.readline()
            if not line:
                time.sleep(1)
                file.seek(where)
            else:
                data = parseLogEntry(line)

                try:
                    listener.sendOSC(data)
                except Exception as e:
                    print(e)


if __name__ == "__main__":
    main()

