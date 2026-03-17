# backend/server.py
import ctypes
from http.server import BaseHTTPRequestHandler, HTTPServer

# Az önce derlediğimiz C kütüphanesini Python'a yüklüyoruz
lib = ctypes.CDLL('./ministream.so')
lib.deney_json.restype = ctypes.c_char_p

class Handler(BaseHTTPRequestHandler):
    def do_GET(self):
        if self.path == '/benchmark':
            # C fonksiyonunu çağır!
            sonuc = lib.deney_json()
            
            self.send_response(200)
            self.send_header('Content-Type', 'application/json')
            self.send_header('Access-Control-Allow-Origin', '*')
            self.end_headers()
            self.wfile.write(sonuc)

    def log_message(self, format, *args):
        pass # Terminal çıktısını sustur

print("Python Backend calisiyor... Port: 8765")
HTTPServer(('localhost', 8765), Handler).serve_forever()
