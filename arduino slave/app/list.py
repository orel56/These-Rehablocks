import os
import glob
import config as conf
import tkinter as tk

class list (tk.Frame): 
    def __init__(self) : 
        super().__init__()
        self.list_devices =[]
        devices = glob.glob('../src/devices/*.cpp')
        for device in devices : 
            device_name = os.path.basename(device).split('.')[0]
            if device_name not in ["sensor","actuator"] : 
                self.list_devices.append(device_name)
        
        self.box=[tk.Listbox()]