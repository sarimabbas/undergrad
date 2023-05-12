import time
import platform
import socket

import pyautogui
from pynput.keyboard import Key, Controller

LOCAL_UDP_IP = "192.168.1.2"
SHARED_UDP_PORT = 57222
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)  # Internet  # UDP
sock.bind((LOCAL_UDP_IP, SHARED_UDP_PORT))

keyboard = Controller()

global_vars = {
    "prev_parsed": "",
    "parsed": "",
    "joyLeftHeld": False,
    "joyRightHeld": False,
}


def loop():
    while True:
        # read from udp
        get_val, addr = sock.recvfrom(2048)
        # print(get_val)
        # update globals
        global_vars["prev_parsed"] = global_vars["parsed"]
        global_vars["parsed"] = get_val.decode("utf-8")
        # emulate inputs
        handleJoystick()
        handleButtons()


def handleJoystick():

    if "joyRight" in global_vars["parsed"] and not global_vars["joyRightHeld"]:
        keyboard.press("d")
        global_vars["joyRightHeld"] = True

    elif "joyLeft" in global_vars["parsed"] and not global_vars["joyLeftHeld"]:
        keyboard.press("a")
        global_vars["joyLeftHeld"] = True

    elif "joyNone" in global_vars["parsed"]:
        keyboard.release("a")
        keyboard.release("d")
        global_vars["joyRightHeld"] = False
        global_vars["joyLeftHeld"] = False


def handleButtons():
    if "momentary" in global_vars["parsed"]:
        sendJump()

    if "switch" in global_vars["parsed"]:
        sendSwitchCharacter()


def sendLeft():
    pyautogui.press("a")


def sendRight():
    pyautogui.press("d")


def sendJump():
    pyautogui.keyDown("w")
    time.sleep(0.02)
    pyautogui.keyUp("w")


def sendSwitchCharacter():
    pyautogui.keyDown("s")
    time.sleep(0.02)
    pyautogui.keyUp("s")


if __name__ == "__main__":
    loop()
