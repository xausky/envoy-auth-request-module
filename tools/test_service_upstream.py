from BaseHTTPServer import HTTPServer
from BaseHTTPServer import BaseHTTPRequestHandler

class EnvoyTestHandler(BaseHTTPRequestHandler):

    def do_GET(self):
        self.send_response(200)
        self.end_headers()
        self.wfile.write("I'm service upstream get method.")
        print self.headers
        return
    def do_POST(self):
        self.send_response(200)
        self.end_headers()
        self.wfile.write("I'm service upstream post method.")
        print self.headers
        return
if __name__ == '__main__':
    
    server = HTTPServer(('localhost', 8000), EnvoyTestHandler)
    print 'Starting test service server, use <Ctrl-C> to stop'
    server.serve_forever()