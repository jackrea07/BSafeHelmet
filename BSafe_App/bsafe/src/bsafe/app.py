# import toga
# from toga.style import Pack
# from toga.style.pack import COLUMN
# import folium
# import os

# class BSafe(toga.App):
#     def startup(self):
#         main_box = toga.Box(style=Pack(direction=COLUMN, padding=20, alignment="left"))

#         # Create a label to display GPS location
#         gps_label = toga.Label('GPS Location:', style=Pack(padding=(0, 10), alignment="left"))

#         # Create a square box for the map
#         square_box = toga.Box(style=Pack(width=200, height=200, alignment="left", padding=10))

#         def display_map(widget):
#             # Replace the following coordinates with the actual latitude and longitude
#             latitude = 37.7749
#             longitude = -122.4194

#             # Create a Folium map centered at the specified coordinates
#             m = folium.Map(location=[latitude, longitude], zoom_start=15)

#             # Add a marker at the specified coordinates
#             folium.Marker([latitude, longitude], tooltip='Location').add_to(m)

#             # Save the map as an HTML file
#             map_filename = 'map.html'
#             m.save(map_filename)

#             # Load the map from the HTML file in a WebView widget
#             map_widget = toga.WebView(url='file://' + os.path.abspath(map_filename), style=Pack(flex=1))
#             square_box.add(map_widget)

#         # Create a "Display Map" button
#         display_button = toga.Button('Display Map', on_press=display_map, style=Pack(padding=10))

#         main_box.add(gps_label)
#         main_box.add(square_box)
#         main_box.add(display_button)

#         # Set the main box as the main content of the main window
#         self.main_window = toga.MainWindow(title=self.formal_name)
#         self.main_window.content = main_box
#         self.main_window.show()

# def main():
#     return BSafe()

import toga
from toga.style import Pack
from toga.style.pack import COLUMN, ROW

import folium
import os

import time
import threading

class BSafeApp(toga.App):
    def startup(self):
        self.threads = []  # Initialize the threads list

        # Create a box for organizing components vertically and center them
        main_box = toga.Box(style=Pack(direction=COLUMN, padding=(140, 40, 40, 40), background_color='rgba(0, 0, 0, 0.8)', alignment='center'))
        # Replace the following coordinates with the actual latitude and longitude
        latitude = 29.643633
        longitude = -82.354927

        # Create a Folium map centered at the specified coordinates
        m = folium.Map(location=[latitude, longitude], zoom_start=15)

        # Add a marker at the specified coordinates
        folium.Marker([latitude, longitude], tooltip='Location').add_to(m)

        # Save the map as an HTML file
        map_filename = 'map.html'
        m.save(map_filename)

        # Load the map from the HTML file in a WebView widget
        map_widget = toga.WebView(url='file://' + os.path.abspath(map_filename), style=Pack(flex=1))

        # Create an image of a motorcycle helmet and center it
        # helmet_image = toga.ImageView('Helmet.png', style=Pack(width=200, height=200, padding=20))

        # Create a button to connect to the helmet
        connect_button = toga.Button('Connect to your Helmet', on_press=self.connect_to_helmet,
                                     style=Pack(padding=20, background_color='steelblue', color='white'))

        # Add components to the main box
        main_box.add(map_widget)
        # main_box.add(helmet_image)
        main_box.add(connect_button)

        # Create a main window and set its content to the main box
        self.main_window = toga.MainWindow(title='BSafe Helmet App', size=(400, 600))
        self.main_window.content = main_box

        # Show the main window
        self.main_window.show()

    def connect_to_helmet(self, widget):
        # Create a new window for the connected helmet screen
        connected_helmet_window = toga.Window(title='Connected Helmet Screen', size=(400, 300))
        connected_helmet_window.app = self

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

