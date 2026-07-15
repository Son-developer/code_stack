import os

def search_recursive(folder_path, target):
    # Duyệt qua từng thứ trong thư mục
    for name in os.listdir(folder_path):
        full_path = os.path.join(folder_path, name)

        if os.path.isfile(full_path) and name == target:
            print(f"Tim thay file: {full_path}")
            return True

        if os.path.isdir(full_path):
            # Gọi đệ quy vào thư mục con
            if search_recursive(full_path, target):
                return True

    return False