import tkinter as tk
from tkinter import filedialog, messagebox
import subprocess

# 全域變數存儲檔案路徑
input_file = ""
output_file = ""

# 上傳檔案
def upload_file():
    global input_file
    input_file = filedialog.askopenfilename(title="Select File to Upload")
    if input_file:
        messagebox.showinfo("File Selected", f"Input File: {input_file}")

# 輸出檔案位置
def download_file():
    global output_file
    output_file = filedialog.asksaveasfilename(title="Select Download Location")
    if output_file:
        messagebox.showinfo("Location Selected", f"Output File: {output_file}")

# 壓縮檔案
def compress_file():
    if not input_file or not output_file:
        messagebox.showwarning("Missing Files", "Please select both input and output files!")
        return
    try:
        # 呼叫 C++ 可執行檔進行壓縮
        subprocess.run(["./HW8-B123245020", "-c", "-i", input_file, "-o", output_file], check=True)
        messagebox.showinfo("Success", "File compressed successfully!")
    except Exception as e:
        messagebox.showerror("Error", f"Compression failed: {str(e)}")

# 解壓縮檔案
def decompress_file():
    if not input_file or not output_file:
        messagebox.showwarning("Missing Files", "Please select both input and output files!")
        return
    try:
        # 呼叫 C++ 可執行檔進行解壓縮
        subprocess.run(["./HW8-B123245020", "-u", "-i", input_file, "-o", output_file], check=True)
        messagebox.showinfo("Success", "File decompressed successfully!")
    except Exception as e:
        messagebox.showerror("Error", f"Decompression failed: {str(e)}")

# 創建 GUI 主介面
def create_gui():
    root = tk.Tk()
    root.title("Huffman Compression Tool")
    root.geometry("400x300")

    # 上傳按鈕
    upload_button = tk.Button(root, text="Upload File", command=upload_file, width=20)
    upload_button.pack(pady=10)

    # 下載按鈕
    download_button = tk.Button(root, text="Download Location", command=download_file, width=20)
    download_button.pack(pady=10)

    # 壓縮按鈕
    compress_button = tk.Button(root, text="Compress File", command=compress_file, width=20, bg="lightblue")
    compress_button.pack(pady=10)

    # 解壓縮按鈕
    decompress_button = tk.Button(root, text="Decompress File", command=decompress_file, width=20, bg="lightgreen")
    decompress_button.pack(pady=10)

    root.mainloop()

# 啟動 GUI
if __name__ == "__main__":
    create_gui()
