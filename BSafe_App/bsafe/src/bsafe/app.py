import toga
from toga.style import Pack
from toga.style.pack import COLUMN
import folium
import os

class BSafe(toga.App):
    def startup(self):
        main_box = toga.Box(style=Pack(direction=COLUMN, padding=20, alignment="left"))

        # Create a label to display GPS location
        gps_label = toga.Label('GPS Location:', style=Pack(padding=(0, 10), alignment="left"))

        # Create a square box for the map
        square_box = toga.Box(style=Pack(width=200, height=200, alignment="left", padding=10))

        def display_map(widget):
            # Replace the following coordinates with the actual latitude and longitude
            latitude = 37.7749
            longitude = -122.4194

            # Create a Folium map centered at the specified coordinates
            m = folium.Map(location=[latitude, longitude], zoom_start=15)

            # Add a marker at the specified coordinates
            folium.Marker([latitude, longitude], tooltip='Location').add_to(m)

            # Save the map as an HTML file
            map_filename = 'map.html'
            m.save(map_filename)

            # Load the map from the HTML file in a WebView widget
            map_widget = toga.WebView(url='file://' + os.path.abspath(map_filename), style=Pack(flex=1))
            square_box.add(map_widget)

        # Create a "Display Map" button
        display_button = toga.Button('Display Map', on_press=display_map, style=Pack(padding=10))

        main_box.add(gps_label)
        main_box.add(square_box)
        main_box.add(display_button)

        # Set the main box as the main content of the main window
        self.main_window = toga.MainWindow(title=self.formal_name)
        self.main_window.content = main_box
        self.main_window.show()

def main():
    return BSafe()
