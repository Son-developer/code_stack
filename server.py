import http.server
import socketserver
import json
import os
from urllib.parse import urlparse, parse_qs

PORT = 8000

class SearchAPIHandler(http.server.SimpleHTTPRequestHandler):
    def do_GET(self):
        parsed_path = urlparse(self.path)
        
        # Nếu web gọi tới API tìm kiếm
        if parsed_path.path == '/api/search':
            query_components = parse_qs(parsed_path.query)
            dir_path = query_components.get('dir', [''])[0]
            file_name = query_components.get('file', [''])[0]
            
            results = []
            status = "success"
            message = ""

            # Kiểm tra xem đường dẫn thư mục có thật không
            if os.path.exists(dir_path):
                try:
                    # Quét tìm file (không phân biệt hoa thường)
                    for root, dirs, files in os.walk(dir_path):
                        for file in files:
                            if file_name.lower() in file.lower():
                                full_path = os.path.join(root, file)
                                results.append(full_path)
                except Exception as e:
                    status = "error"
                    message = f"Không thể quét thư mục này: {str(e)}"
            else:
                status = "error"
                message = "Đường dẫn thư mục không tồn tại trên máy tính!"

            # Trả dữ liệu JSON về cho trình duyệt
            self.send_response(200)
            self.send_header('Content-Type', 'application/json')
            self.end_headers()
            response = json.dumps({"status": status, "results": results, "message": message})
            self.wfile.write(response.encode('utf-8'))
        else:
            # Nếu truy cập bình thường, trả về file HTML/CSS tĩnh
            super().do_GET()

# Khởi chạy Server
with socketserver.TCPServer(("", PORT), SearchAPIHandler) as httpd:
    print(f"🚀 Server chạy ngon lành tại: http://localhost:{PORT}")
    try:
        httpd.serve_forever()
    except KeyboardInterrupt:
        print("\nĐã đóng Server.")