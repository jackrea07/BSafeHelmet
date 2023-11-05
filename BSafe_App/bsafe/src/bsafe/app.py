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

        # Create a box for the navbar with increased padding at the bottom
        # navbar_box = toga.Box(style=Pack(direction=ROW, padding=(10, 0, 20, 0), background_color='rgba(0, 0, 0, 0.8)'))

        # Create a label for the title "BSafe Helmet" with updated style
        title_label = toga.Label(
            'BSafe Helmet',
            style=Pack(
                flex=1,  # Take up available space horizontally
                color='steelblue',
                font_size=20,
                alignment='center',  # Center the text horizontally
                padding_top=10,  # Add padding from the top
                background_color='rgba(0, 0, 0, 0.8)'  # Make the background color transparent
            )
        )
        # Create a button for the menu icon (three horizontal lines)
        menu_button = toga.Button('☰', on_press=self.open_menu, style=Pack(width=40, height=40, background_color='steelblue', color='white'))

        # # Add components to the navbar box
        # navbar_box.add(title_label)
        # navbar_box.add(menu_button)

        # # Add the navbar above the map and connect button
        # main_box.add(navbar_box)
        main_box.add(title_label)
        main_box.add(menu_button)

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
        map_widget = toga.WebView(url='file://' + os.path.abspath(map_filename), style=Pack(flex=1, padding=(10, 10, 10, 10)))


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

