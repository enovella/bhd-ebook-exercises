import string,cgi
from os import curdir, sep
from BaseHTTPServer import BaseHTTPRequestHandler, HTTPServer
 
class WWWHandler(BaseHTTPRequestHandler):
 
	def do_GET(self):
		try:
			f = open(curdir + sep + "webex_poc1.html")

			self.send_response(200)
			self.send_header('Content-type', 'text/html')
			self.end_headers()
			self.wfile.write(f.read())
			f.close()
      
			return
      
		except IOError:
			self.send_error(404,'File Not Found: %s' % self.path)

def main():
	try:
		server = HTTPServer(('', 80), WWWHandler)
		print 'server started'
		server.serve_forever()
	except KeyboardInterrupt:
		print 'shutting down server'
		server.socket.close()

if __name__ == '__main__':
	main()