'''
Created on Nov 30, 2012

@author: alexs
'''
import pygame
from pygame.locals import *
import bluetooth

target_name = "roboBT"
target_address = None
sock = 0

def search_roboBT():
    #nearby_devices = bluetooth.discover_devices()
    #for bdaddr in nearby_devices:
    #    if target_name == bluetooth.lookup_name( bdaddr ):
    #        target_address = bdaddr
    #        break
    
    global target_address 
    
    target_address = '00:12:6F:27:EF:A8'

    if target_address is not None:
        print ("found target bluetooth device with address ", target_address)
    else:
        print ("could not find target bluetooth device nearby")
        exit()
    pass


def connect_roboBT():
    global sock 
    sock=bluetooth.BluetoothSocket( bluetooth.RFCOMM )
    sock.connect((target_address, 1))
    pass
 
def disconnect_roboBT():
    global sock
    sock.close()
    pass
 
def init_keyloop():
    pygame.init()
    pygame.display.set_mode((64, 48))
    pygame.display.set_caption('Pygame Caption')
    pygame.mouse.set_visible(0)
    pass

def mainloop():
    done = False
    while not done:
        for event in pygame.event.get():
            if (event.type == KEYDOWN):
                if (event.key == K_UP):
                    
                    print event
                    go_forward()
                if (event.key == K_DOWN):
                    print event
                    go_backward()
                if (event.key == K_LEFT):
                    print event
                    go_rotate_left()
                if (event.key == K_RIGHT):
                    print event
                    go_rotate_right()
            if (event.type == KEYUP):
                if (event.key == K_UP or event.key == K_DOWN or event.key == K_LEFT or event.key == K_RIGHT):
                    print event
                    go_stop()
            if (event.type == KEYUP) or (event.type == KEYDOWN):
                print event
                if (event.key == K_ESCAPE):
                    done = True

goString = "M_LR"

def convToString(left, right):
    retstring = ""
    
    if left >= 0:
        retstring = retstring + "+" + repr(left).zfill(3)
    else:
        retstring = retstring + repr(left).zfill(4)
    
    if right >= 0:
        retstring = retstring + "+" + repr(right).zfill(3)
    else:
        retstring = retstring + repr(right).zfill(4)    
        
    return retstring
    pass

def m_lr_command(left, right):
    global goString
    
    command = goString + convToString(left, right) + "\r"
    
    print command 
    
    return command
    pass



def go_forward():
    global sock
    sock.send(m_lr_command(+999, +999))
    pass

def go_rotate_left():
    global sock
    sock.send(m_lr_command(-999, +999))
    pass

def go_rotate_right():
    global sock
    sock.send(m_lr_command(+999, -999))
    pass

def go_backward():
    global sock
    sock.send(m_lr_command(-999, -999))
    pass

def go_backward_left():
    global sock
    sock.send(m_lr_command(-500, -999))
    pass

def go_backward_right():
    global sock
    sock.send(m_lr_command(-999, -500))
    pass

def go_stop():
    global sock
    sock.send(m_lr_command(0, 0))
    pass



if __name__ == '__main__':
    
    search_roboBT()
    connect_roboBT()
 
    init_keyloop()
    mainloop()
    
    disconnect_roboBT()
    pass
 
                
