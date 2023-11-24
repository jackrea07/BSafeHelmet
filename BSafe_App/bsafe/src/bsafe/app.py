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

        # Create a Folium map centered at the specified coordinates with padding
        latitude = 29.643633
        longitude = -82.354927
        m = folium.Map(location=[latitude, longitude], zoom_start=14, control_scale=True, zoom_control=False)
        folium.Marker([latitude, longitude], tooltip='Location').add_to(m)

        # Save the map as a base64-encoded string
        map_base64 = m._repr_html_()

        # Load the map from the base64-encoded string in a WebView widget with padding
        map_widget = toga.WebView(
            url=f'data:text/html;base64,{base64.b64encode(map_base64.encode()).decode()}',
            style=Pack(flex=4, padding=(0, 0))
        )

        # Create buttons for the navigation bar with icons
        bluetooth_icon_path = 'icons/bluetooth_icon.png'  # Replace with the actual path
        map_icon_path = 'icons/map_icon.png'  # Replace with the actual path
        user_icon_path = 'icons/user_icon.png'  # Replace with the actual path

        button1 = toga.Button(
            'B1', on_press=self.connect_to_helmet,
            style=Pack(flex=1, padding=(15, 15), height=70, background_color='steelblue')
        )
        button1.icon = toga.Icon(bluetooth_icon_path, 'Bluetooth')

        button2 = toga.Button(
            'B2', on_press=self.show_map,
            style=Pack(flex=1, padding=(15, 15), height=70, background_color='steelblue')
        )
        # button2.icon = toga.Icon(map_icon_path, 'Map')

        button3 = toga.Button(
            'B3', on_press=self.show_user_profile,
            style=Pack(flex=1, padding=(15, 15), height=70, background_color='steelblue')
        )
        # button3.icon = toga.Icon(user_icon_path, 'User Profile')

        # Create a box for the navigation bar
        navbar_box = toga.Box(
            style=Pack(direction=ROW, padding=(0, 0, 0, 0), flex=1, height=100, alignment='bottom', background_color='black')
        )

        # Add buttons to the navigation bar
        navbar_box.add(button1)
        navbar_box.add(button2)
        navbar_box.add(button3)

        # Create a box for the map
        map_container = toga.Box(style=Pack(flex=4, height=485, width=900))

        # Add components to the map container
        map_container.add(map_widget)

        # Create a box for the navbar
        navbar_container = toga.Box(style=Pack(flex=1, alignment='bottom'))

        # Add components to the navbar container
        navbar_container.add(navbar_box)

        # Create a main window and set its content to the two containers
        self.main_window = toga.MainWindow(title='BSafe Helmet App', size=(1000, 1800))
        self.main_window.content = toga.Box(
            style=Pack(direction=COLUMN, padding=(0, 0)),
            children=[map_container, navbar_container]
        )

        # Show the main window
        self.main_window.show()

    def connect_to_helmet(self, widget):
        # Create a label for displaying the loading text
        loading_label = toga.Label('Searching for bluetooth signals', style=Pack(padding=20))

        # Add the loading label to the main box (replace the existing content)
        self.main_box.remove(self.map_widget)  # Remove the map widget
        #self.main_box.remove(self.navbar_box)  # Remove the navigation bar
        self.main_box.add(loading_label)  # Add the loading label

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

        # Simulate a delay to represent the time it takes to connect to the helmet
        time.sleep(5)

        # Once connected, update the main window with the new information
        self.main_box.remove(loading_label)  # Remove the loading label
        self.main_box.add(self.map_widget)  # Add back the map widget
        self.main_box.add(self.navbar_box)  # Add back the navigation bar

        # You can update the map or perform any other actions as needed

        # Refresh the main window
        self.main_window.refresh()

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
