#   _    _ _______   __  __ _____ _____ _____   ____  _   _
#  | |  | |__   __| |  \/  |_   _/ ____|  __ \ / __ \| \ | |
#  | |__| |  | |    | \  / | | || |    | |__) | |  | |  \| |
#  |  __  |  | |    | |\/| | | || |    |  _  /| |  | | . ` |
#  | |  | |  | |    | |  | |_| || |____| | \ \| |__| | |\  |
#  |_|  |_|  |_|    |_|  |_|_____\_____|_|  \_\\____/|_| \_|
#  =================== Advanced R&D ========================

#  Copyright (c) 2023 HT Micron Semicondutors S.A.
#  Licensed under the Apache License, Version 2.0 (the "License");
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#  http://www.apache.org/licenses/LICENSE-2.0
#  Unless required by applicable law or agreed to in writing, software
#  distributed under the License is distributed on an "AS IS" BASIS,
#  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#  See the License for the specific language governing permissions and
#  limitations under the License.

# file: ittapp.py
# brief: ittchip Example backend. Comunicates with PPI board.
# author: Matheus Zorzeto
# link: https://github.com/htmicron
# version: 1
# date: April 26, 2024

import sys

from PyQt5 import QtWidgets
from PyQt5 import QtCore
from PyQt5.QtWidgets import *
from PyQt5.QtCore import *

from front import Ui_MainWindow

import random
import time

from paho.mqtt import client as mqtt_client
import paho.mqtt.client as mqtt

#MQTT broker host
broker = 'broker.hivemq.com'

#Enable TLS
MQTT_TLS_ENABLE = False

#MQTT port'
if MQTT_TLS_ENABLE == True:
    port = 8883
else:
    port = 1883 

# generate client ID with pub prefix randomly
client_id = f'HTNB32L-XXX_MQTT_Client-{random.randint(0, 1000)}'
username = 'HTNB32L-XXX_MQTT_Client' 
password = 'HTmicron'

subscribe_buff = ""

class front(QtWidgets.QMainWindow, Ui_MainWindow):

    LED1_status = 0
    LED2_status = 0

    def __init__(self) -> None:
        super(front, self).__init__()

        self.ui = Ui_MainWindow()
        self.ui.setupUi(self)

        self.ui.lcd1.display("--")

        self.ui.Button1.clicked.connect(self.clicked1)

        self.client = self.connect_mqtt()
        self.client.loop_start()

        self.subscribe()

    def clicked1(self):

        payload = "White"
        topic = "htnb32l_whitebutton_sw"       
        self.publish(payload, topic)

        if(self.LED1_status == 0):
            self.ui.Button1.setText("LED ON")
            self.LED1_status = 1
        else:
            self.ui.Button1.setText("LED OFF")
            self.LED1_status = 0

    def receivedled1(self):
        if(self.LED1_status == 0):
            self.ui.Button1.setText("LED ON")
            self.LED1_status = 1
        else:
            self.ui.Button1.setText("LED OFF")
            self.LED1_status = 0

    def send_updated_status(self):
        payload = "{};{}".format(self.LED1_status, self.LED2_status)
        topic = "htnb32l_set_state"

        self.publish(payload, topic)

    def connect_mqtt(self):
        def on_connect(client, userdata, flags, rc):
            if rc == 0:
                print("Connected to MQTT Broker!")
            else:
                print("Failed to connect, return code %d\n", rc)
        
        def on_message(client, usardata, message):
            global subscribe_buff

            if message.topic == "htnb32l_whitebutton_fw":
                if str(message.payload.decode("utf-8")) == "White":
                    self.receivedled1()        
            elif message.topic == "htnb32l_get_state":
                if str(message.payload.decode("utf-8")) == "GetState":
                    time.sleep(1)
                    self.send_updated_status()
            elif message.topic == "htnb32l_timer_fw":
                self.ui.lcd1.display(str(message.payload.decode("utf-8")))

            subscribe_buff = str(message.payload.decode("utf-8"))
            print("Subscribe received: ", subscribe_buff)

        client = mqtt_client.Client(client_id)
        client.username_pw_set(username, password)
        if MQTT_TLS_ENABLE == True:
            client.tls_set(tls_version=mqtt.ssl.PROTOCOL_TLSv1_2)
        client.on_connect = on_connect
        client.on_message = on_message
        client.connect(broker, port)
        return client

    def publish(self, msg, topic):

        result = self.client.publish(topic, msg)
        # result: [0, 1]
        status = result[0]
        
        if status == 0:
            print(f"Send `{msg}` to topic `{topic}`")
        else:
            print(f"Failed to send message to topic {topic}")
    
    def subscribe(self):
        self.client.subscribe("htnb32l_whitebutton_fw", 0)
        self.client.subscribe("htnb32l_get_state", 0)
        self.client.subscribe("htnb32l_timer_fw", 0)

if __name__=="__main__":
    app = QtWidgets.QApplication(sys.argv)
    window = front()
    window.show()
    sys.exit(app.exec_())
