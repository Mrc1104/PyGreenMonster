import tkinter as tk
from tkinter import ttk
import GUI.utils as u
import GUI.Colors as gmc
import Bindings.RemexClient as Remex

class LabeledEntry:
    def __init__(self, parent, label, default_entry="",rw="normal", tk_var=None):
        self.tk_var = tk_var if tk_var != None else tk.StringVar(value=default_entry)
        self.frame = tk.Frame(parent, bg = gmc.pewter)
        self.frame.pack(anchor='w', pady=(5,0), fill='x', expand=True)

        self.label = tk.Label(self.frame, text=label, bg = gmc.pewter)
        self.label.pack(side='left', padx=(0,5), expand=True)

        self.entry = tk.Entry(self.frame, width=10, justify="center", textvariable=self.tk_var, state=rw)
        self.entry.pack(side="left", fill='x', expand=True)
    def GetEntry(self):
        return self.entry.get()

class CoilFields:
    def __init__(self, parent, label_text, width=20, row=0, col=0):
        self.frame = tk.Frame(parent, bg = gmc.pewter)
        self.frame.grid(row=row, column=col, padx=5, pady=5, sticky="nsew")

        self.label = tk.Label(self.frame, text=label_text, bg=gmc.steel)
        self.label.pack(anchor='w', fill='x')

        self.freq_entry = LabeledEntry(self.frame, "Freq [Hz]", 10)
        self.ampl_entry = LabeledEntry(self.frame, "Ampl [A] ", 0.25)
        self.cycl_entry = LabeledEntry(self.frame, "Cycles   ", 100)
        self.stat_entry = LabeledEntry(self.frame, "Status   ", "Unknown", "readonly")
    def GetFreq(self):
        return self.freq_entry.GetEntry()
    def GetAmpl(self):
        return self.ampl_entry.GetEntry()
    def GetCycl(self):
        return self.cycl_entry.GetEntry()

class BMW(tk.Frame):
  def __init__(self, tab):
    super().__init__(tab)
    ## Member vars
    self.coil_names = ['MHF1C01H', 'MHF1C02H', 'MHF1C03V', 'MHF1C08H', 'MHF1C08V', 'MHF1C10H', 'MHF1C10V', 'SL 20 Vernier']
    self.coil_fields= [] # will be filled later
    self.waveforms = ['Sine', 'Ramp']

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
    self.apply_button = tk.Button(coils_frame, text="Update Coil Settings", command = lambda: print("Apply Button Pressed"))
    self.apply_button.pack(anchor='e', padx=10)
    print(f"field entry = {self.coil_fields[0].GetAmpl()}")

    ## SuperCycle Frame
    cycle_frame = tk.Frame(tab, width=600, height=100, bg=gmc.green)
    cycle_frame.pack(padx=5, pady=5, fill="both", expand=True)
    cycle_notebook = ttk.Notebook(cycle_frame)
    cycle_notebook.pack(expand=True, fill="both")
    sup_cyc_tab = tk.Frame(cycle_notebook, bg="lightblue")
    sup_cyc_var = tk.StringVar(value="None")
    for tool in ["Resizing", "Rotating"]:
        tk.Radiobutton(
            sup_cyc_tab,
            text=tool,
            variable=sup_cyc_var,
            value=tool,
            bg="lightblue",
            ).pack(anchor="w", padx=20, pady=5)

    sing_coil_tab = tk.Frame(cycle_notebook, bg="lightgreen")

    self.wave_opt = tk.StringVar(self)
    self.menu_opt = tk.StringVar(self)
    self.wave_opt.set(self.waveforms[0]);
    self.menu_opt.set(self.coil_names[0]);

    tk.OptionMenu(sing_coil_tab, self.menu_opt, self.coil_names[0], *self.coil_names[1:], command=lambda x:print(f"Selected {x}")).pack(side="left", padx=5, pady=5)
    tk.OptionMenu(sing_coil_tab, self.wave_opt, self.waveforms[0], *self.waveforms[1:], command=lambda x:print(f"Selected {x}")).pack(side="left", padx=5, pady=5)

    tk.Button(sing_coil_tab, text="Load WaveForm", command=lambda : print("Load Sinwave Button Pressed")).pack(side="left", padx=5, pady=5)
    tk.Button(sing_coil_tab, text="Enable Trigger", command=lambda : print("Enter Trigger State Button Pressed")).pack(side="left", padx=5, pady=5)
    tk.Button(sing_coil_tab, text="Stop", command=lambda : print("Stop Button Pressed")).pack(side="left", padx=5, pady=5)

    cycle_notebook.add(sup_cyc_tab, text="Super Cycle")
    cycle_notebook.add(sing_coil_tab, text="Single Coil")
