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

        # Save the map as an HTML file
        map_filename = 'map.html'
        m.save(map_filename)

        # Load the map from the HTML file in a WebView widget with padding
        map_widget = toga.WebView(url='file://' + os.path.abspath(map_filename), style=Pack(flex=1, padding=(0, 0)))

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
        navbar_box = toga.Box(style=Pack(direction=ROW, padding=(0, 0, 20, 0), flex=1, height=100, alignment='center', background_color = 'black'))

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

def main():
    return BSafeApp()

if __name__ == '__main__':
    BSafeApp().main_loop()