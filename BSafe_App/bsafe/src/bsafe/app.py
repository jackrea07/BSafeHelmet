"""
Mobile app to pair with the BSafe Helmet
"""
import toga
from toga.style import Pack
from toga.style.pack import COLUMN, ROW


class BSafe(toga.App):

    def startup(self):
        main_box = toga.Box(style=Pack(direction=COLUMN, padding=20, alignment="center"))

        # Create a label to display GPS location
        gps_label = toga.Label('GPS Location:', style=Pack(padding=(0, 10), alignment="center"))
        gps_location = toga.Label('Waiting for location...', style=Pack(padding=(0, 10), alignment="center"))

        # Create a label to display Bluetooth connection status
        bluetooth_label = toga.Label('Bluetooth Status:', style=Pack(padding=(0, 10), alignment="center"))
        bluetooth_status = toga.Label('Not connected', style=Pack(padding=(0, 10), alignment="center"))

        # Create a square box for GPS location (you can customize size and appearance)
        square_box = toga.Box(style=Pack(direction=ROW, alignment="center"))
        square_box.add(gps_location)

        # Center the labels and square box horizontally
        main_box.add(gps_label)
        main_box.add(square_box)
        main_box.add(bluetooth_label)
        main_box.add(bluetooth_status)

        # Align the components to the left side of the main box
        # main_box.alignment = LEFT

        # Set the main box as the main content of the main window
        self.main_window = toga.MainWindow(title=self.formal_name)
        self.main_window.content = main_box
        self.main_window.show()


def main():
    return BSafe()
