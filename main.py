from recursive_search import search_recursive
from stack_search import search_with_stack

if __name__ == "__main__":
    folder = input("Nhap duong dan thu muc can tim (vi du: C:\\Users\\Ten\\Desktop): ")
    target = input("Nhap ten file can tim (vi du: anh2.jpg): ")

    print("\n--- Tim bang de quy ---")
    if not search_recursive(folder, target):
        print("File khong ton tai!")

    print("\n--- Tim bang Stack (iterative) ---")
    if not search_with_stack(folder, target):
        print("File khong ton tai!")