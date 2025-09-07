import ttkbootstrap as tb
from ttkbootstrap.constants import *
from tkinter import messagebox, filedialog
import subprocess
import os

class CacheSimulatorGUI:
    def __init__(self, root):
        self.root = root
        self.root.title("Cache Simulator")
        self.root.geometry("800x600")
        # self.root.iconbitmap('icon.ico')  # Optional: Add your own icon

        # Set the executable name
        self.executable = "./cache_simulator.exe" if os.name == 'nt' else "./cache_simulator"

        # Create main frame with padding
        self.main_frame = tb.Frame(root, padding=10)
        self.main_frame.pack(fill=BOTH, expand=True)

        # Create GUI elements
        self.create_inputs()
        self.create_output_area()
        self.create_buttons()

    def create_inputs(self):
        # Input frame with border and padding
        input_frame = tb.LabelFrame(
            self.main_frame, text="Cache Configuration", padding=10
        )
        input_frame.pack(fill=X, pady=10)

        # Define labels and entries
        labels = [
            "Instruction Cache Size (bytes):",
            "Data Cache Size (bytes):",
            "Cache Line Size (bytes):",
            "Cache Access Time (cycles):",
            "Access Sequence File:",
        ]
        default_values = ["4096", "4096", "12", "5", ""]
        self.entries = {}

        for idx, (label_text, default) in enumerate(zip(labels, default_values)):
            label = tb.Label(input_frame, text=label_text, font=("Segoe UI", 10))
            label.grid(row=idx, column=0, sticky=W, pady=5)

            if "File" in label_text:
                entry = tb.Entry(input_frame, width=40, font=("Segoe UI", 10))
                entry.grid(row=idx, column=1, padx=5, pady=5, sticky=(W, E))
                browse_button = tb.Button(
                    input_frame,
                    text="Browse",
                    bootstyle=INFO,
                    command=self.browse_file,
                    width=10,
                )
                browse_button.grid(row=idx, column=2, padx=5, pady=5)
            else:
                entry = tb.Entry(input_frame, width=20, font=("Segoe UI", 10))
                entry.grid(row=idx, column=1, padx=5, pady=5, sticky=W)
                entry.insert(0, default)

            self.entries[label_text] = entry

        input_frame.columnconfigure(1, weight=1)

    def create_output_area(self):
        # Output frame with border and padding
        output_frame = tb.LabelFrame(
            self.main_frame, text="Simulation Output", padding=10
        )
        output_frame.pack(fill=BOTH, expand=True, pady=10)

        # Create text widget and scrollbar
        self.output_text = tb.Text(
            output_frame, wrap=WORD, font=("Consolas", 10), state='disabled'
        )
        scrollbar = tb.Scrollbar(
            output_frame, orient="vertical", command=self.output_text.yview
        )
        self.output_text.configure(yscrollcommand=scrollbar.set)

        # Grid them
        self.output_text.pack(side=LEFT, fill=BOTH, expand=True)
        scrollbar.pack(side=RIGHT, fill=Y)

    def create_buttons(self):
        # Buttons frame with padding
        button_frame = tb.Frame(self.main_frame, padding=10)
        button_frame.pack(fill=X, pady=10)

        # Run button
        run_button = tb.Button(
            button_frame,
            text="Run Simulation",
            bootstyle=SUCCESS,
            command=self.run_simulation,
            width=15,
        )
        run_button.pack(side=LEFT, padx=5)

        # Clear button
        clear_button = tb.Button(
            button_frame,
            text="Clear",
            bootstyle=DANGER,
            command=self.clear_output,
            width=10,
        )
        clear_button.pack(side=LEFT, padx=5)

    def browse_file(self):
        filename = filedialog.askopenfilename(
            initialdir=".",
            title="Select Access Sequence File",
            filetypes=(("Text files", "*.txt"), ("All files", "*.*")),
        )
        if filename:
            # Convert to relative path if possible
            try:
                filename = os.path.relpath(filename)
            except ValueError:
                # Keep absolute path if relative path is not possible
                pass
            self.entries["Access Sequence File:"].delete(0, END)
            self.entries["Access Sequence File:"].insert(0, filename)

    def clear_output(self):
        self.output_text.config(state='normal')
        self.output_text.delete(1.0, END)
        self.output_text.config(state='disabled')

    def run_simulation(self):
        try:
            # Validate inputs
            icache_size = int(self.entries["Instruction Cache Size (bytes):"].get())
            dcache_size = int(self.entries["Data Cache Size (bytes):"].get())
            line_size = int(self.entries["Cache Line Size (bytes):"].get())
            access_time = int(self.entries["Cache Access Time (cycles):"].get())
            input_file = self.entries["Access Sequence File:"].get()

            if not os.path.exists(input_file):
                messagebox.showerror("Error", "Access sequence file not found!")
                return

            # Clear previous output
            self.clear_output()

            # Create the input string with exact formatting
            input_string = f"{icache_size}\n{dcache_size}\n{line_size}\n{access_time}\n{input_file}\n"

            # Run the executable
            process = subprocess.Popen(
                self.executable,
                stdin=subprocess.PIPE,
                stdout=subprocess.PIPE,
                stderr=subprocess.PIPE,
                text=True,
                bufsize=1,
            )

            # Send input and get output
            stdout, stderr = process.communicate(input_string)

            # Display output
            self.output_text.config(state='normal')
            if stdout:
                self.output_text.insert(END, stdout)
            if stderr:
                self.output_text.insert(END, "\nErrors:\n" + stderr)
            self.output_text.config(state='disabled')

            # Scroll to the beginning
            self.output_text.see("1.0")

        except ValueError:
            messagebox.showerror(
                "Error", "Please enter valid numeric values for cache parameters!"
            )
        except Exception as e:
            messagebox.showerror("Error", f"An error occurred: {str(e)}")

def main():
    style = tb.Style("cyborg")  # Choose a modern theme, e.g., "cyborg", "minty", "superhero"
    root = tb.Window(themename="cyborg")
    app = CacheSimulatorGUI(root)
    root.mainloop()

if __name__ == "__main__":
    main()
