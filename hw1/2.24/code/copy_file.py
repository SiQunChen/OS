import os
import sys

def copy_file(source_file, target_file):
    try:
        # 檢查源檔案是否存在
        if not os.path.exists(source_file):
            raise FileNotFoundError(f"Source file '{source_file}' does not exist.")
        
        # 打開源檔案以讀取
        with open(source_file, 'rb') as src:
            # 打開目標檔案以寫入
            with open(target_file, 'wb') as dest:
                # 從源檔案讀取內容並寫入目標檔案
                dest.write(src.read())
        
        print("File copied successfully.")
    except Exception as e:
        print(f"Error: {e}")

def main():
    # 提示用戶輸入原始檔案和目標檔案的名稱
    source_file = input("Enter the name of the source file: ")
    target_file = input("Enter the name of the target file: ")

    # 呼叫copy_file函式來複製檔案
    copy_file(source_file, target_file)

if __name__ == "__main__":
    main()
