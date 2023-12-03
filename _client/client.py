import webview
import requests
import tkinter as tk
import threading
import time

root = tk.Tk()

width = root.winfo_screenwidth()
height = root.winfo_screenheight()

MIN_WIDTH = 100
MIN_HEIGHT = 150


def fetch_and_display_html(current_window):
    
    url = "http://localhost:8080/weather?mode=html"
    try:
        response = requests.get(url)
        response.raise_for_status()
        html_content = response.text
        html_content = html_content.replace("Â", " ")
        current_window.load_html(html_content)
        
    except requests.exceptions.RequestException as e:
        print(f"Error: {e}")


def update(current_window):
    while True:
        fetch_and_display_html(current_window)
        time.sleep(5)
        print("cycle")

if __name__ == '__main__':
    current_window = webview.create_window("HTML Viewer", frameless=True, on_top=True, draggable=False, width=MIN_WIDTH, height=MIN_HEIGHT, x=width - MIN_WIDTH - 25, y=height - MIN_HEIGHT - 40)
    webview.start(update, current_window)
