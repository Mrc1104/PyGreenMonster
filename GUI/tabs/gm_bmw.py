import tkinter as tk
from tkinter import ttk
import GUI.utils as u
import GUI.Colors as gmc
import RemexClient.RemexClient as Remex

class LabeledEntry:
    def __init__(self, parent, label, default_entry=""):
        self.frame = tk.Frame(parent, bg = gmc.pewter)
        self.frame.pack(anchor='w', pady=(5,0), fill='x', expand=True)

        self.label = tk.Label(self.frame, text=label, bg = gmc.pewter)
        self.label.pack(side='left', padx=(0,5), expand=True)

        self.entry = tk.Entry(self.frame, width=10, justify="center")
        self.entry.insert(0, default_entry)
        self.entry.pack(side="left", fill='x', expand=True)


class CoilFields:
    def __init__(self, parent, label_text, width=20, row=0, col=0):
        self.frame = tk.Frame(parent, bg = gmc.pewter)
        self.frame.grid(row=row, column=col, padx=5, pady=5, sticky="nsew")

        self.label = tk.Label(self.frame, text=label_text, bg=gmc.steel)
        self.label.pack(anchor='w', fill='x')

        self.freq_entry = LabeledEntry(self.frame, "Freq [Hz]", 10)
        self.ampl_entry = LabeledEntry(self.frame, "Ampl [A] ", 0.25)
        self.cycl_entry = LabeledEntry(self.frame, "Cycles   ", 100)

class BMW(tk.Frame):
  def __init__(self, tab):
    ## Member vars
    self.coil_names = ['MHF1C01H', 'MHF1C02H', 'MHF1C03V', 'MHF1C08H', 'MHF1C08V', 'MHF1C10H', 'MHF1C10V', 'SL 20 Vernier']
    self.coil_fields= [] # will be filled later

    tk.Label(tab, text="Coils", bg=gmc.green).pack(anchor="nw", pady=(5,0))

    ## Coil Frame
    coils_frame = tk.Frame(tab, width=600, height=400, bg=gmc.green)
    coils_frame.pack(padx=5, pady=5, fill="both", expand=True)
    coils_grid  = tk.Frame(coils_frame, bg=gmc.green)
    coils_grid.pack(fill="both", expand=True)
    
    ## Coil Fields
    MAX_COLUMNS = 4
    for index, coil in enumerate(self.coil_names):
        row = index // MAX_COLUMNS
        col = index % MAX_COLUMNS
        coils_grid.grid_columnconfigure(col, weight=1)
        coils_grid.grid_rowconfigure(row, weight=1)
        field = CoilFields(coils_grid, coil, 20, row, col)
        self.coil_fields.append(field)
    self.apply_button = tk.Button(tab, text="Update Coil Settings")
    self.apply_button.pack()
