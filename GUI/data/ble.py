
"""
SERVICE_UUID=    "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
CHARACTERISTIC_UUID= "beb5483e-36e1-4688-b7f5-ea07361b26a8"

def heart_rate_callback(handle, data):
  print(handle, int.from_bytes(data,byteorder='little'))

async def main():
    devices = await BleakScanner.discover()
    for d in devices:
        try :
           if  d.details["props"]["Name"]=="ESP32_BLE_Device" : 
                print("found it")
                mydevice=d
                break
        except : 
            print("no name")
    
    address = mydevice.details["props"]["Address"]
    
    async with BleakClient(address) as client : 
        services = client.services
       
        await client.start_notify(CHARACTERISTIC_UUID, heart_rate_callback)
        await asyncio.sleep(100)
        await client.stop_notify(CHARACTERISTIC_UUID)


asyncio.run(main())
 """
#############

import asyncio
import json
from datetime import datetime
from typing import Callable, Any
from aioconsole import ainput
from bleak import BleakClient, discover

read_characteristic = "beb5483e-36e1-4688-b7f5-ea07361b26a8"

selected_device = []

typecode_dictionnary = { 1:"Accelerometer",
                         2: "screen",
                         3:"Led"
                       }

subject_dictionnary = {"1": "mouvement",
                       "2": "angle_x",
                       "3": "angle_y",
                       "4": "direction",
                       "16": "potar_value",
                       "35": "obstacle",
                       "60": "led_status"
                       }


class Connection:
    
    client: BleakClient = None
    
    def __init__(
        self,
        loop: asyncio.AbstractEventLoop,
        read_characteristic: str,
        data_dump_size: int = 256,
    ):
        self.loop = loop
        self.read_characteristic = read_characteristic

        self.last_packet_time = datetime.now()
        self.dump_size = data_dump_size
        self.connected = False
        self.connected_device = None

        self.data_emulate="{\"Master\":\"BLE_DEVICE\",\"0\":{\"Nom\":\"Accelerometre 1\",\"Sujets produits\":{\"Mouvement\":1},\"Sujets recut\":{\"Led_status\":0}}, \"1\":{\"Nom\":\"Led 1\",\"Sujets produits\":{\"Led_status\":0},\"Sujets recut\":{\"Mouvement\":0}}}"
        
        self.rx_data = {}
        self.data_format={}
        self.rx_timestamps = []
        self.rx_delays = []

    def on_disconnect(self, client: BleakClient, future: asyncio.Future):
        self.connected = False
        # Put code here to handle what happens on disconnet.
        print(f"Disconnected from {self.connected_device.name}!")
    async def cleanup(self):
        if self.client:
            await self.client.stop_notify(read_characteristic)
            await self.client.disconnect()

    async def manager(self):
        print("Starting connection manager.")
        while True:
            if self.client:
                await self.connect()
            else:
                await self.select_device()
                await asyncio.sleep(15.0, loop=self.loop)       

    async def connect(self):
        if self.connected:
            return
        try:
            await self.client.connect()
            self.connected = await self.client.is_connected()
            if self.connected:
                print(F"Connected to {self.connected_device.name}")
                self.client.set_disconnected_callback(self.on_disconnect)
                await self.client.start_notify(
                    self.read_characteristic, self.notification_handler,
                )
                while True:
                    if not self.connected:
                        break
                    await asyncio.sleep(3.0, loop=self.loop)
            else:
                print(f"Failed to connect to {self.connected_device.name}")
        except Exception as e:
            print(e)

    async def select_device(self):
        print("Bluetooh LE hardware warming up...")
        await asyncio.sleep(2.0, loop=self.loop) # Wait for BLE to initialize.
        devices = await discover()

        print("Please select device: ")
        for i, device in enumerate(devices):
            print(f"{i}: {device.name}")

        response = -1
        while True:
            response = await ainput("Select device: ")
            try:
                response = int(response.strip())
            except:
                print("Please make valid selection.")
            
            if response > -1 and response < len(devices):
                break
            else:
                print("Please make valid selection.")

        print(f"Connecting to {devices[response].name}")
        self.connected_device = devices[response]
        self.client = BleakClient(devices[response].address, loop=self.loop)

    def record_time_info(self):
        present_time = datetime.now()
        self.rx_timestamps.append(present_time)
        self.rx_delays.append((present_time - self.last_packet_time).microseconds)
        self.last_packet_time = present_time

    def clear_lists(self):
        self.rx_delays.clear()
        self.rx_timestamps.clear()

    def notification_handler(self, sender: str, data: Any):
        my_data = data.decode('utf-8')  
        self.rx_data =  json.loads(my_data)
        self.record_time_info()
        
    def data_formatting(self):
        if self.rx_data !={}: 
            self.data_format["Master"]=self.rx_data["Device"]
            for node in self.rx_data["Nodes"] : 
                self.data_format[node]={"Nom":typecode_dictionnary[self.rx_data["Nodes"][node]["type"]]+" "+str(self.rx_data["Nodes"][node]["id"])}
                for subject in self.rx_data["Nodes"][node]["Produced subject"] :
                    self.data_format[node]["Sujets produits"]={subject_dictionnary[subject]:self.rx_data["Nodes"][node]["Produced subject"][subject]}
                for subject in self.rx_data["Nodes"][node]["Received subject"] :
                    self.data_format[node]["Sujets recut"]={subject_dictionnary[subject]:self.rx_data["Nodes"][node]["Received subject"][subject]}
        else : 
            self.data_format={}
            
    def request(self) : 
        self.data_formatting()
        print(self.data_format)

        if self.data_format!={} : 
            return self.data_format
        else : 
            return {"Master" :"No nodes connected"}
        

#############
# Loops
#############


if __name__=="__main__":
    loop = asyncio.get_event_loop()
    connection = Connection(
        loop, read_characteristic
    )
    
    connection.data_formatting();