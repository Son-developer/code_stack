class Node:
    def __init__(self, name, is_file):
        self.name = name
        self.is_file = is_file
        self.children = []  # danh sách con (file/thư mục con)