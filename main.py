from kivymd.app import MDApp
from kivy.lang import Builder
from kivymd.toast import toast
from usbserial4a import serial4a
from usb4a import usb
from kivy.core.window import Window
from kivy.clock import Clock
from threading import Thread
import time

kv ='''
MDFloatLayout:
    size_hint: (1,1)
    md_bg_color: 1,1,1,1
    MDFloatLayout:
        size_hint:(1,.3)
        pos_hint: {"center_x": 0.5, "center_y":.85}
        MDLabel:
            id: time_display
            text: "[b]0.000[/b]"
            markup: True
            font_size: "45dp"
            halign: "center"
            pos_hint: {"center_x": .5, "center_y": .5}
    MDFloatLayout:
        size_hint: (1,.3)
        pos_hint: {"center_x": 0.5, "center_y":.65}
        Button:
            id: start_button
            size_hint: (.3,.3)
            
            pos_hint: {"center_x": .3, "center_y":.5}

            text: "[b]Start[/b]"
            markup: True
            font_size: "20dp"
            on_release: app.start_timer()
            background_color:1,1,1,0
            canvas.before:
                Color:
                    rgba: 0,0,1,1
                RoundedRectangle:
                    size: self.size
                    pos:self.pos
                    radius: [10]
        Button:
            padding : 10
            size_hint: (.3,0.3)
            
            pos_hint: {"center_x": .7, "center_y":.5}

            text: "[b]Reset[/b]"
            markup: True
            font_size: "20dp"
            color: rgba(0,0,0,255)
            background_color:1,1,1,0
            on_release: app.reset_timer()
            canvas.before:
                Color:
                    rgba: 204/255,204/255,204/255,1
                RoundedRectangle:
                    size: self.size
                    pos:self.pos
                    radius: [10]

'''



class MainApp(MDApp):
    def build(self):
        self.starttime = 0.0
        self.elapsed_time = 0.0
        self.running = False
        Clock.schedule_interval(self.update_timer, 0.01)
        return Builder.load_string(kv)
    def on_start(self):
        if device_list:
            toast(device_list[0].getDeviceName())
    def reset_timer(self):
        self.running =False
        self.starttime = 0.0
        self.elapsed_time = 0.0
        self.root.ids.time_display.text = "[b]0.000[/b]"   
    def update_timer(self, dt):
        if self.running:
            self.elapsed_time = time.time() - self.starttime
            self.root.ids.time_display.text = f"[b]{self.elapsed_time:.3f}[/b]"
    def start_timer(self):
        if not self.running:
            self.running =True
            self.starttime = time.time() - self.elapsed_time
            self.root.ids.start_button.canvas.before.children[0].rgba = (1, 0, 0, 1) 
            self.root.ids.start_button.text = "[b]Stop[/b]" 
            
        elif self.running:
            self.running =False
            self.root.ids.start_button.canvas.before.children[0].rgba = (0, 0, 1, 1)
            self.root.ids.start_button.text = "[b]Start[/b]" 
if _name_ == "_main_":
    device_list = usb.get_usb_device_list()
    if device_list:
    	serial_port = serial4a.get_serial_port(device_list[0].getDeviceName(), 9600)
    MainApp().run()
