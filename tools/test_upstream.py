from BaseHTTPServer import HTTPServer
from BaseHTTPServer import BaseHTTPRequestHandler

class EnvoyTestHandler(BaseHTTPRequestHandler):

    def do_GET(self):
        message = '\n'.join([
          'CLIENT VALUES:',
          'client_address=%s (%s)' % (self.client_address, self.address_string()),
          'command=%s' % self.command,
          'path=%s' % self.path,
          'request_version=%s' % self.request_version,
          '',
          'SERVER VALUES:',
          'server_version=%s' % self.server_version,
          'sys_version=%s' % self.sys_version,
          'protocol_version=%s' % self.protocol_version,
          '',
        ])
        self.send_response(200)
        self.end_headers()
        self.wfile.write(message)
        print self.headers
        return

if __name__ == '__main__':
    
    server = HTTPServer(('localhost', 8000), EnvoyTestHandler)
    print 'Starting server, use <Ctrl-C> to stop'
    server.serve_forever()