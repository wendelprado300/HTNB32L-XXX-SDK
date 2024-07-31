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

# file: backend.py
# brief: MQTT Example backend. Creates a digital twin of HTNB32L demo board.
# author: HT Micron Advanced R&D
# link: https://github.com/htmicron
# version: 0.1
# date: April 11, 2023

import sys

from PyQt5 import QtWidgets
from PyQt5 import QtCore
from PyQt5.QtWidgets import *
from PyQt5.QtCore import *
import threading

from Demo_GUI import Ui_MainWindow

import random
import time

from paho.mqtt import client as mqtt_client
import paho.mqtt.client as mqtt

#MQTT broker host
broker = 'test.mosquitto.org'

#MQTT port'
port = 8883 

# generate client ID with pub prefix randomly
client_id = f'HTNB32L-XXX_MQTT_Client-{random.randint(0, 1000)}'
username = 'HTNB32L-XXX_MQTT_Client' 
password = 'HTmicron'

subscribe_buff = ""

class Worker(QObject):
    green_led = pyqtSignal(bool)

    def run(self):
        while True:
            self.green_led.emit(True)
            time.sleep(1)
            self.green_led.emit(False)
            time.sleep(1)

class Backend(QtWidgets.QMainWindow, Ui_MainWindow):
    
    def __init__(self) -> None:
        super(Backend, self).__init__()

        self.ui = Ui_MainWindow()
        self.ui.setupUi(self)

        self.ui.blue_button.clicked.connect(self.blue_button_clicked)
        self.ui.white_button.clicked.connect(self.white_button_clicked)

        self.thread = QThread()
        self.worker = Worker()
        self.worker.moveToThread(self.thread)
        self.thread.started.connect(self.worker.run)
        self.worker.green_led.connect(self.blink_green_led)

        self.blue_button_state = 0
        self.white_button_state = 0
       
        self.client = self.connect_mqtt()
        self.client.loop_start()
        self.subscribe()
        self.start_periodic_ping()

    def blue_button_clicked(self):
        self.blue_button_state ^= 1

        payload = "Blue"
        topic = "htnb32l_bluebutton_sw"
        
        self.publish(payload, topic)

        if self.blue_button_state == 1:
            self.blue_led_on()
        else:
            self.blue_led_off()

    def white_button_clicked(self):
        self.white_button_state ^= 1

        payload = "White"
        topic = "htnb32l_whitebutton_sw"
        self.publish(payload, topic)

        if self.white_button_state == 1:
            self.white_led_on()
        else:
            self.white_led_off()


    def blue_led_on(self):
        self.ui.blue_led.setStyleSheet("background-color: rgb(0, 85, 255);")
    
    def blue_led_off(self):
        self.ui.blue_led.setStyleSheet("background-color: rgb(0, 33, 99);")

    def white_led_on(self):
        self.ui.white_led.setStyleSheet("background-color: rgb(255, 255, 255);")

    def white_led_off(self):
        self.ui.white_led.setStyleSheet("background-color: rgb(141, 141, 141);")

    def blink_green_led(self, led_status):
        if led_status:
            self.ui.green_led.setStyleSheet("background-color: rgb(0, 197, 0);")
        else:
            self.ui.green_led.setStyleSheet("background-color: rgb(0, 86, 0);")

    def blink_blue_led(self):
        cnt = 0

        while cnt < 5:
            self.ui.blue_led.setStyleSheet("background-color: rgb(0, 85, 255);")
            time.sleep(0.25)
            self.ui.blue_led.setStyleSheet("background-color: rgb(0, 33, 99);")
            time.sleep(0.25)
            cnt += 1
    
    def blink_white_led(self):
        cnt = 0

        while cnt < 5:
            self.ui.white_led.setStyleSheet("background-color: rgb(255, 255, 255);")
            time.sleep(0.25)
            self.ui.white_led.setStyleSheet("background-color: rgb(141, 141, 141);")
            time.sleep(0.25)
            cnt += 1

    def send_updated_status(self):
        payload = "{};{}".format(self.blue_button_state, self.white_button_state)
        topic = "htnb32l_set_state"

        self.publish(payload, topic)

    def connect_mqtt(self):
        def on_connect(client, userdata, flags, rc):
            if rc == 0:
                self.thread.start()
                print("Connected to MQTT Broker!")
            else:
                print("Failed to connect, return code %d\n", rc)
        
        def on_disconnect(client, userdata, rc):
            print(f"Disconnected with result code {rc}")
            if rc != 0:
                print("Unexpected disconnection. Attempting to reconnect...")
                reconnect(client)
        
        def reconnect(client):
            while True:
                try:
                    client.reconnect()
                    print("Reconnected to MQTT Broker!")
                    break
                except Exception as e:
                    print(f"Reconnection failed due to {e}, retrying in 5 seconds...")
                    time.sleep(5)
        
        def on_message(client, usardata, message):
            global subscribe_buff

            if message.topic == "htnb32l_bluebutton_fw":
                if str(message.payload.decode("utf-8")) == "Blue":
                    self.blue_button_state ^= 1
                    if self.blue_button_state == 1:
                        self.blue_led_on()
                    else:
                        self.blue_led_off()
            
            elif message.topic == "htnb32l_whitebutton_fw":
                if str(message.payload.decode("utf-8")) == "White":
                    self.white_button_state ^= 1
                    if self.white_button_state == 1:
                        self.white_led_on()
                    else:
                        self.white_led_off()

            elif message.topic == "htnb32l_get_state":
                if str(message.payload.decode("utf-8")) == "GetState":
                    time.sleep(1)
                    self.send_updated_status()

            subscribe_buff = str(message.payload.decode("utf-8"))
            print("Subscribe received: ", subscribe_buff)

        client = mqtt_client.Client(client_id)
        # client.username_pw_set(username, password)
        client.tls_set(tls_version=mqtt.ssl.PROTOCOL_TLSv1_2)
        client.on_connect = on_connect
        client.on_disconnect = on_disconnect
        client.on_message = on_message
        client.keep_alive = 60
        client.connect(broker, port)
        return client

    def publish(self, msg, topic):

        result = self.client.publish(topic, msg, qos=2)
        # result: [0, 1]
        status = result[0]
        
        if status == 0:
            print(f"Send `{msg}` to topic `{topic}`")
        else:
            print(f"Failed to send message to topic {topic}")
    
    def subscribe(self):
        self.client.subscribe("htnb32l_bluebutton_fw", qos=2)
        self.client.subscribe("htnb32l_whitebutton_fw", qos=2)
        self.client.subscribe("htnb32l_get_state", qos=2)

    def start_periodic_ping(self):
        def send_periodic_ping(client):
            while True:
                time.sleep(30)  # Send a ping every 30 seconds
                client.publish("ping_topic", "ping", qos=2)

        ping_thread = threading.Thread(target=send_periodic_ping, args=(self.client,))
        ping_thread.daemon = True
        ping_thread.start()

if __name__=="__main__":
    app = QtWidgets.QApplication(sys.argv)
    window = Backend()
    window.show()
    sys.exit(app.exec_())