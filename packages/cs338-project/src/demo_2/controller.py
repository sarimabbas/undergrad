import pygame
import math
import serial
import json

# init joystick stuff
pygame.init()
pygame.joystick.init()
clock = pygame.time.Clock()
done = False

joystick_count = pygame.joystick.get_count()
if joystick_count < 1:
    print("No controller found")
    exit()

joystick = pygame.joystick.Joystick(0)
joystick.init()

# Get the name and axes from the OS for the controller/joystick
name = joystick.get_name()
print("Joystick name: {}".format(name))
axes = joystick.get_numaxes()
print("Number of axes: {}".format(axes))

# gripper flag
gripper = False
neutral = False

# attempt to open a Serial port to Arduino
ser = serial.Serial(port='/dev/cu.usbmodem14101', baudrate=9600)
print(ser.name)

# helper function
def approximateMovement(floating):
    if math.isclose(floating, 1.0, rel_tol=1e-1):
        return 1
    elif math.isclose(floating, -1.0, rel_tol=1e-1):
        return -1
    else:
        return 0

while done == False:

    # stick processing
    # axes:
    #   0 / left-thumb LR
    #   1 / left-thumb UP
    #   2 / right-thumb LR
    #   3 / right-thumb UP
    llr = approximateMovement(joystick.get_axis(0))
    lup = approximateMovement(joystick.get_axis(1))
    rlr = approximateMovement(joystick.get_axis(2))
    rup = approximateMovement(joystick.get_axis(3))

    # event-driven button processing
    for event in pygame.event.get():
        # quit condition
        if event.type == pygame.QUIT:
            done = True     
        # X button: 0
        if event.type == pygame.JOYBUTTONDOWN and event.button == 0:
            if event.button == 0:
                gripper = not gripper

    # construct json object
    controls = {
        "llr": llr,
        "lup" : lup,
        "rlr" : rlr,
        "rup": rup,
        "gr": gripper,
    }

    controls_json = json.dumps(controls)
    print(controls_json)
    ser.write(bytes(controls_json, encoding='utf-8'))

    # Limit frames per second
    clock.tick(15)

# Close the window and quit.
pygame.quit()

