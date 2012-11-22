
import bluetooth
import Tkinter as tk


target_name = "roboBT"
target_address = None

#nearby_devices = bluetooth.discover_devices()

#for bdaddr in nearby_devices:
#    if target_name == bluetooth.lookup_name( bdaddr ):
#        target_address = bdaddr
#        break

target_address = '00:12:6F:27:EF:A8'

if target_address is not None:
    print ("found target bluetooth device with address ", target_address)
else:
    print ("could not find target bluetooth device nearby")

port = 0x1


#sock=bluetooth.BluetoothSocket( bluetooth.RFCOMM )
#sock.connect((target_address, port))

#sock.send("MLRER\r")


#sock.close()



def keypress(event):
    if event.keysym == 'Escape':
        root.destroy()
        x = event.char
        if x == "w":
            print "blaw blaw blaw"
        elif x == "a":
            print "blaha blaha blaha"
        elif x == "s":
            print "blash blash blash"
        elif x == "d":
            print "blad blad blad"
        else:
            print x
            
root = tk.Tk()
print "Press a key (Escape key to exit):"
root.bind_all('<Key>', keypress)
# don't show the tk window
#root.withdraw()
root.mainloop()
