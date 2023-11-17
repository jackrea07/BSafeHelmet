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
        map_widget = toga.WebView(url=f'data:text/html;base64,{base64.b64encode(map_base64.encode()).decode()}', style=Pack(flex=1, padding=(0, 0)))

        # Create buttons for the navigation bar
        button1 = toga.Button('B1', on_press=self.button1_action, style=Pack(flex=1, 
                                                                             padding=(10, 10), 
                                                                             height=100, 
                                                                             background_color='steelblue', 
                                                                             #border_width=1
                                                                             ))
        button2 = toga.Button('B2', on_press=self.button2_action, style=Pack(flex=1, 
                                                                             padding=(10, 10), 
                                                                             height=100, 
                                                                             background_color='steelblue', 
                                                                             #border_width=1
                                                                             ))
        
        # Set images for the buttons
        # button1.image = 'path/to/button1_icon.png'
        # button2.image = 'path/to/button2_icon.png'

        # Create a box for the navigation bar
        navbar_box = toga.Box(style=Pack(direction=ROW, padding=(0, 0, 0, 0), flex=1, height=100, alignment='center', background_color = 'black'))

        # Add buttons to the navigation bar
        navbar_box.add(button1)
        navbar_box.add(button2)

        # Add components to the main box
        main_box.add(map_widget)
        main_box.add(navbar_box)

        # Create a main window and set its content to the main box
        self.main_window = toga.MainWindow(title='BSafe Helmet App', size=(400, 600))
        self.main_window.content = main_box

        # Show the main window
        self.main_window.show()

    def button1_action(self, widget):
        # Implement button 1 functionality here
        pass

    def button2_action(self, widget):
        # Implement button 2 functionality here
        pass

    def connect_to_helmet(self, widget):
        # Create a new window for the connected helmet screen
        connected_helmet_window = toga.Window(title='Connected Helmet Screen', size=(400, 600))
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

def main():
    return BSafeApp()

if __name__ == '__main__':
    BSafeApp().main_loop()