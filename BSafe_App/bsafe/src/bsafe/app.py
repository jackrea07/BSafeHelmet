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
        self.threads = []  # Initialize the threads list

        # Create a box for organizing components vertically and center them
        main_box = toga.Box(style=Pack(direction=COLUMN, padding=(20, 40, 40, 40), background_color='rgba(0, 0, 0, 0.8)', alignment='center'))

        # Create a box for the navigation bar components (title and menu button)
        nav_bar_box = toga.Box(style=Pack(direction=ROW, padding=(10, 10, 10, 10)))

        # Create a label for the title "BSafe Helmet" without background color
        title_label = toga.Label('BSafe Helmet', style=Pack(flex=1, font_size=20))

        # Create a button for the menu icon (three horizontal lines) without background color
        menu_button = toga.Button('☰', on_press=self.open_menu, style=Pack(width=40, height=40))

        # Add components to the navigation bar box
        nav_bar_box.add(title_label)
        nav_bar_box.add(menu_button)

        # Add the navigation bar above the map and connect button with padding
        main_box.add(nav_bar_box)
        main_box.add(toga.Box(style=Pack(height=20)))  # Add padding between navigation bar and map

        # Replace the following coordinates with the actual latitude and longitude
        latitude = 29.643633
        longitude = -82.354927

        # Create a Folium map centered at the specified coordinates with padding
        m = folium.Map(location=[latitude, longitude], zoom_start=15, control_scale=True, zoom_control=False)

        # Add a marker at the specified coordinates
        folium.Marker([latitude, longitude], tooltip='Location').add_to(m)


        ######## OLD CODE UNSTABLE ###### vvvvvvvv
        # # Save the map as an HTML file
        # map_filename = 'map.html'
        # m.save(map_filename)

        # # Load the map from the HTML file in a WebView widget with padding
        # map_widget = toga.WebView(url='file://' + os.path.abspath(map_filename), style=Pack(flex=1, padding=(0, 10)))

        # Save the map as a base64-encoded string
        map_base64 = folium.Map(location=[latitude, longitude], zoom_start=15, control_scale=True, zoom_control=False)._repr_html_()

        # Load the map from the base64-encoded string in a WebView widget with padding
        map_widget = toga.WebView(url=f'data:text/html;base64,{base64.b64encode(map_base64.encode()).decode()}', style=Pack(flex=1, padding=(0, 0)))



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

    def open_menu(self, widget):
        # Implement menu functionality here
        pass

    def connect_to_helmet(self, widget):
        # Create a new window for the connected helmet screen
        connected_helmet_window = toga.Window(title='Connected Helmet Screen', size=(400, 600))
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

