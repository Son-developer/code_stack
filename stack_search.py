import os

def search_with_stack(folder_path, target):
    stack = [folder_path]  # stack chứa các đường dẫn thư mục cần duyệt

    while stack:
        current_folder = stack.pop()

        for name in os.listdir(current_folder):
            full_path = os.path.join(current_folder, name)

            if os.path.isfile(full_path) and name == target:
                print(f"Tim thay file: {full_path}")
                return True

            if os.path.isdir(full_path):
                stack.append(full_path)  # đẩy thư mục con vào stack

    return False