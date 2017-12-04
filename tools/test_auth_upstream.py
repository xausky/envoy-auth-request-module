from BaseHTTPServer import HTTPServer
from BaseHTTPServer import BaseHTTPRequestHandler

class EnvoyTestHandler(BaseHTTPRequestHandler):

    def do_GET(self):
        self.send_response(200)
        self.send_header('Content-Type', 'application/json')
        self.end_headers()
        self.wfile.write('{"X-Auth-Status": "pass"}')
        print self.headers
        return
    def do_POST(self):
        self.send_response(403)
        self.send_header('Content-Type', 'application/json')
        self.end_headers()
        self.wfile.write('{"X-Auth-Status": "access denied"}')
        print self.headers
        return

if __name__ == '__main__':
    
    server = HTTPServer(('localhost', 9000), EnvoyTestHandler)
    print 'Starting test service server, use <Ctrl-C> to stop'
    server.serve_forever()