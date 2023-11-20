import base64

import toga
from toga.style import Pack
from toga.style.pack import COLUMN, ROW

import folium
import os

import time
import threading

class BSafeApp(toga.App):
    def startup(self):

        # List to store threads
        self.threads = []

        # Create a box for organizing components vertically and center them
        main_box = toga.Box(style=Pack(direction=COLUMN, padding=(0, 0, 0, 0)))

        # Replace the following coordinates with the actual latitude and longitude
        latitude = 29.643633
        longitude = -82.354927

        # Create a Folium map centered at the specified coordinates with padding
        m = folium.Map(location=[latitude, longitude], zoom_start=15, control_scale=True, zoom_control=False)

        # Add a marker at the specified coordinates
        folium.Marker([latitude, longitude], tooltip='Location').add_to(m)

        # Save the map as a base64-encoded string
        map_base64 = m._repr_html_()

        # Load the map from the base64-encoded string in a WebView widget with padding
        map_widget = toga.WebView(url=f'data:text/html;base64,{base64.b64encode(map_base64.encode()).decode()}', style=Pack(flex=2, padding=(0, 0)))

        # Create buttons for the navigation bar with icons
        bluetooth_icon_path = 'bsafe/src/bsafe/icons/bluetooth_icon'  # Replace with the actual path
        map_icon_path = 'src/bsafe/icons/map_icon.png'  # Replace with the actual path
        user_icon_path = 'src/bsafe/icons/user_icon.png'  # Replace with the actual path

        button1 = toga.Button('B1', on_press=self.connect_to_helmet, style=Pack(flex=1, padding=(15, 15), height=70, background_color='steelblue'))
        button1.icon = toga.Icon(bluetooth_icon_path, 'Bluetooth')

        button2 = toga.Button('B2', on_press=self.show_map, style=Pack(flex=1, padding=(15, 15), height=70, background_color='steelblue'))
        #button2.icon = toga.Icon(map_icon_path, 'Map')

        button3 = toga.Button('B3', on_press=self.show_user_profile, style=Pack(flex=1, padding=(15, 15), height=70, background_color='steelblue'))
        #button3.icon = toga.Icon(user_icon_path, 'User Profile')

        # Create a box for the navigation bar
        navbar_box = toga.Box(style=Pack(direction=ROW, padding=(0, 0, 0, 0), flex=1, height=100, alignment='center', background_color='black'))

        # Add buttons to the navigation bar
        navbar_box.add(button1)
        navbar_box.add(button2)
        navbar_box.add(button3)

        # Add components to the main box
        main_box.add(map_widget)
        main_box.add(navbar_box)

        # Create a main window and set its content to the main box
        self.main_window = toga.MainWindow(title='BSafe Helmet App', size=(1000, 1800))
        self.main_window.content = main_box

        # Show the main window
        self.main_window.show()

    def connect_to_helmet(self, widget):
        # Create a new window for the connected helmet screen
        connected_helmet_window = toga.Window(title='Connected Helmet Screen', size=(1000, 1800))
        connected_helmet_window.app = self

        # Set the background color of the window
        connected_helmet_window.background_color = 'lightblue'

        # Create a label for displaying the loading text
        loading_label = toga.Label('Searching for bluetooth signals', style=Pack(padding=20))

        # Add the loading label to the connected helmet window
        connected_helmet_window.content = loading_label

        # Start a thread to update the loading text with cycling ellipses
        def update_loading_text():
            ellipses = 1
            running = True
            while running:
                time.sleep(0.5)
                ellipses = (ellipses + 1) % 4
                loading_text = 'Searching for bluetooth signals' + '.' * ellipses

                # Update the loading label
                loading_label.text = loading_text

        # Start the thread as daemon
        thread = threading.Thread(target=update_loading_text, daemon=True)
        thread.start()

        # Add the thread to the app's threads list
        self.threads.append(thread)

        # Show the connected helmet window
        connected_helmet_window.show()
        pass

    def show_map(self, widget):
        # Implement button 2 functionality here
        pass

    def show_user_profile(self, widget):
        # Implement button 3 functionality here
        pass

def main():
    return BSafeApp()

if __name__ == '__main__':
    BSafeApp().main_loop()