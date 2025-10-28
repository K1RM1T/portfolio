import numpy as np
import matplotlib.pyplot as plt
import tkinter as tk
from tkinter import messagebox


def plot_function():
    user_input = entry_func.get().strip().lower().replace('^', '**')

    if not user_input.startswith('y='):
        messagebox.showerror("Ошибка", "Введите функцию в формате y=...")
        return

    try:
        x_start = float(entry_x_from.get())
        x_end = float(entry_x_to.get())

        if x_start >= x_end:
            raise ValueError("Начальное значение x должно быть меньше конечного.")

        expr = user_input[2:]
        open_calculator_window(expr)
        x = np.linspace(x_start, x_end, 1000)

        y = eval(expr, {"__builtins__": {}}, {"x": x, "np": np})
        y = np.full_like(x, y) if isinstance(y, (int, float)) else y

        valid = np.isfinite(y)
        x_valid = x[valid]
        y_valid = y[valid]

        if len(x_valid) == 0:
            raise ValueError("Нет допустимых значений для отображения графика.")

        plt.figure(figsize=(8, 6))
        plt.plot(x_valid, y_valid, label=user_input, color='blue')
        plt.axhline(0, color='black', linewidth=0.5)
        plt.axvline(0, color='black', linewidth=0.5)
        plt.xlabel("x")
        plt.ylabel("y")
        plt.title("График функции")
        plt.grid(True)
        plt.legend()
        plt.show()

    except ValueError as ve:
        messagebox.showerror("Ошибка диапазона", str(ve))
    except Exception as e:
        messagebox.showerror("Ошибка", f"Не удалось построить график:\n{e}")


def open_calculator_window(expr):
    def calculate_y():
        try:
            x_val = float(entry_x_val.get())
            y_val = eval(expr, {"__builtins__": {}}, {"x": x_val, "np": np})
            result_label.config(text=f"y = {y_val}")
        except Exception as e:
            result_label.config(text=f"Ошибка: {e}")

    calc_win = tk.Toplevel(root)
    calc_win.title("Вычисление значения y")

    tk.Label(calc_win, text="Введите значение x:").pack(pady=5)
    entry_x_val = tk.Entry(calc_win, width=20)
    entry_x_val.pack(pady=5)

    tk.Button(calc_win, text="Рассчитать", command=calculate_y).pack(pady=5)
    result_label = tk.Label(calc_win, text="")
    result_label.pack(pady=5)


root = tk.Tk()
root.title("График функции")

tk.Label(root, text="Введите функцию вида y=").pack(pady=5)
entry_func = tk.Entry(root, width=40)
entry_func.pack(pady=5)

frame_range = tk.Frame(root)
frame_range.pack(pady=5)

tk.Label(frame_range, text="x от:").pack(side=tk.LEFT)
entry_x_from = tk.Entry(frame_range, width=10)
entry_x_from.insert(0, "-10")
entry_x_from.pack(side=tk.LEFT, padx=5)

tk.Label(frame_range, text="до:").pack(side=tk.LEFT)
entry_x_to = tk.Entry(frame_range, width=10)
entry_x_to.insert(0, "10")
entry_x_to.pack(side=tk.LEFT, padx=5)

btn = tk.Button(root, text="Построить график", command=plot_function)
btn.pack(pady=10)

root.mainloop()