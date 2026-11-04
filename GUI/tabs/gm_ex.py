import tkinter as tk
from tkinter import ttk
import GUI.utils as u
import GUI.Colors as gmc
import RemexClient.RemexClient as Remex

class Example(tk.Frame):
  def __init__(self, tab):
    ## Tools frame
    tools_frame = tk.Frame(tab, width=200, height=400, bg="skyblue")
    tools_frame.pack(padx=5, pady=5, side=tk.LEFT, fill=tk.Y)
    tk.Label(
            tools_frame,
            text="Original Image",
            bg="skyblue",
            ).pack(padx=5, pady=5)
    tk.Label(tools_frame).pack(padx=5, pady=5)

# Tools and Filters tabs
    notebook = ttk.Notebook(tools_frame)
    notebook.pack(expand=True, fill="both")

    tools_tab = tk.Frame(notebook, bg="lightblue")
    tools_var = tk.StringVar(value="None")
    for tool in ["Resizing", "Rotating"]:
        tk.Radiobutton(
            tools_tab,
            text=tool,
            variable=tools_var,
            value=tool,
            bg="lightblue",
            ).pack(anchor="w", padx=20, pady=5)

    filters_tab = tk.Frame(notebook, bg="lightgreen")
    filters_var = tk.StringVar(value="None")
    for filter in ["Blurring", "Sharpening"]:
        tk.Radiobutton(
            filters_tab,
            text=filter,
            variable=filters_var,
            value=filter,
            bg="lightgreen",
            ).pack(anchor="w", padx=20, pady=5)

    notebook.add(tools_tab, text="Tools")
    notebook.add(filters_tab, text="Filters")

# Image frame
    image_frame = tk.Frame(tab, width=400, height=400, bg="grey")
    image_frame.pack(padx=5, pady=5, side=tk.RIGHT)
    tk.Label(
            image_frame,
            text="Edited Image",
            bg="grey",
            fg="white",
            ).pack(padx=5, pady=5)
    tk.Label(image_frame).pack(padx=5, pady=5)
    #self.bm_frame = tk.LabelFrame(tab, text='Beam Modulation', background=gmc.green)
    #self.script_frame = tk.LabelFrame(self.bm_frame, text='Beam Modulation Script', background=gmc.green)
    #self.script_frame.grid(row=0, column=0, pady=10, sticky='W')
    #self.test_frame = tk.LabelFrame(self.bm_frame, text='Beam Modulation - TEST', background=gmc.green)
    #self.test_frame.grid(row=1, column=0, pady=10, sticky='W')

    #self.button_titles = ['MHF1C01H', 'MHF1C02H', 'MHF1C03V', 'MHF1C08H',
    #                 'MHF1C08V', 'MHF1C10H', 'MHF1C10V', 'SL Zone 20']

    #self.ks_bm_l = tk.Label(self.script_frame, text='Kill Switch is OFF', background=gmc.green)
    #self.status_bm_l = tk.Label(self.script_frame, text='******** Hello', background=gmc.green)
    #self.script_bm_l = tk.Label(self.script_frame, text='Beam Modulation script is ****', background=gmc.green)
    #self.test_e = tk.Entry(self.test_frame, width=12)
    #self.bm_test_setting = tk.StringVar(); self.bm_test_index = tk.IntVar()
    #self.script_frame_layout()
    ##self.test_frame_layout()
    #self.bm_frame.pack(padx=20, pady=20, anchor='w')
    #self.check_status_button()

  def script_frame_layout(self):
    self.ks_bm_l.grid(row=0, column=0, padx=10, pady=10, sticky='W')
    tk.Button(self.script_frame, text='Check Status', background=gmc.green, command=self.check_status_button).grid(
        row=0, column=1, padx=10, pady=10, sticky='W')
    self.status_bm_l.grid(row=2, column=2, padx=10, pady=10, sticky='W')
    self.bm_button = tk.Button(self.script_frame, text='Start Beam Modulation', background=gmc.green, command=self.change_status)
    self.bm_button.grid(row=1, column=0, padx=10, pady=10, sticky='W')
    self.script_bm_l.grid(row=1, column=1, padx=10, pady=10, sticky='W')
  def check_status_button(self):
    pass
    #self.check_status()
    #self.check_active_flag()
  def change_status(self):
    pass

  def test_frame_layout(self):
    tk.Button(self.test_frame, text='Enable BMW Test', background=gmc.green).grid(
        row=0, column=0, columnspan=2, padx=10, pady=10)
    tk.Button(self.test_frame, text='Toggle Kill Switch', background=gmc.green).grid(
        row=0, column=2, columnspan=2, padx=10, pady=10)
    
    self.bm_test_index.set(0)
    self.bm_test_setting.set(self.button_titles[self.bm_test_index.get()])
