import tkinter as tk
from tkinter import simpledialog
from tkinter import messagebox
from tkinter import ttk
import random

def on_stock_select(event):
    random_number = random.randint(1, 100)
    stock_display.config(text=f"Stock Value: {random_number}")

def on_user_select(event):
    num_users = random.randint(1, 10)
    random_users = [f"User{random.randint(1, 1000)}" for _ in range(num_users)]
    chat_display.insert(tk.END, f"Random users generated: {', '.join(random_users)}\n")

def send_message():
    message = message_entry.get().strip()
    if message: 
        chat_display.config(state='normal') 
        chat_display.insert(tk.END, f"You: {message}\n")
        chat_display.config(state='disabled')  
        message_entry.delete(0, tk.END)


def show_login_window():
    login_result = {"status": 0} 
    login_window = tk.Toplevel(root)
    login_window.title("Login")

    tk.Label(login_window, text="Username:").pack(pady=2)
    username_entry = tk.Entry(login_window)
    username_entry.pack(pady=2)

    tk.Label(login_window, text="Password:").pack(pady=2)
    password_entry = tk.Entry(login_window, show="*")
    password_entry.pack(pady=2)

    def try_login():
        username = username_entry.get()
        password = password_entry.get()
        if username == "user" and password == "pass":
            messagebox.showinfo("Login", "Login Successful")
            login_result["status"] = 1
            login_window.destroy()
        else:
            messagebox.showerror("Login", "Login Failed")
            login_result["status"] = 0

    def create_account():
        messagebox.showinfo("Account", "Create Account")
        login_result["status"] = 2
        login_window.destroy()

    tk.Button(login_window, text="Login", command=try_login).pack(pady=5)
    tk.Button(login_window, text="Create Account", command=create_account).pack(pady=5)

    login_window.grab_set()  
    root.wait_window(login_window)  

    return login_result["status"]


root = tk.Tk()
root.title("Stock and Chat Application")
root.withdraw() 

# login_status = show_login_window()
login_status = 1
if login_status in [1, 2]:
    root.deiconify()  
    print("Success")
    stock_frame = tk.Frame(root)
    stock_frame.grid(row=0, column=0, sticky="nsew") 

    stock_label = tk.Label(stock_frame, text="Select Stock:")
    stock_label.pack(pady=10)

    stock_dropdown = ttk.Combobox(stock_frame, values=["Stock1", "Stock2", "Stock3"])
    stock_dropdown.pack()
    stock_dropdown.bind("<<ComboboxSelected>>", on_stock_select)

    stock_display = tk.Label(stock_frame, text="Stock Value: ")
    stock_display.pack(pady=10)

    # Chat section
    chat_frame = tk.Frame(root)
    chat_frame.grid(row=0, column=1, sticky="nsew") 

    user_label = tk.Label(chat_frame, text="Select User:")
    user_label.pack(pady=10)

    user_dropdown = ttk.Combobox(chat_frame, values=["User1", "User2", "User3"])
    user_dropdown.pack()
    user_dropdown.bind("<<ComboboxSelected>>", on_user_select)

    chat_display = tk.Text(chat_frame, state='disabled')
    chat_display.pack(pady=10)
    chat_display.config(state='normal') 
    for i in range(1, 50):
        chat_display.insert(tk.END, f"User{i}: Hello! This is a test message.\n")
    chat_display.config(state='disabled')  


    message_entry = tk.Entry(chat_frame)
    message_entry.pack(pady=10)

    send_button = tk.Button(chat_frame, text="Send", command=send_message)
    send_button.pack()

else:
    root.destroy() 

root.mainloop()
