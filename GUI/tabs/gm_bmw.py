import tkinter as tk
from tkinter import ttk
import GUI.utils as u
import GUI.Colors as gmc
import Bindings.RemexClient as Remex

class LabeledEntry:
    def __init__(self, parent, label, default_entry="",tk_var=None, rw="normal"):
        self.tk_var = tk_var if tk_var != None else tk.StringVar(value=default_entry)
        self.tk_var.set(default_entry)
        self.frame = tk.Frame(parent, bg = gmc.pewter)
        self.frame.pack(anchor='w', pady=(5,0), fill='x', expand=True)

        self.label = tk.Label(self.frame, text=label, bg = gmc.pewter)
        self.label.pack(side='left', padx=(0,5), expand=True)

        self.entry = tk.Entry(self.frame, width=10, justify="center", textvariable=self.tk_var, state=rw)
        self.entry.pack(side="left", fill='x', expand=True)
    def GetEntry(self):
        return self.entry.get()

class CoilName:
    def __init__(self, id, name):
        self.name = name
        self.id = id

class CoilVars:
    def __init__(self, parent, coil, waveforms):
        self.coil     = coil
        self.freq_var = tk.StringVar(parent, value="")
        self.ampl_var = tk.StringVar(parent, value="")
        self.cycl_var = tk.StringVar(parent, value="")
        self.wave_var = tk.StringVar(parent, value="")
        self.stat_var = tk.StringVar(parent, value="")
        self.waveforms = waveforms
    def get_data(self):
        return {
            "freq" : self.freq_var.get(),
            "ampl" : self.ampl_var.get(),
            "cycl" : self.cycl_var.get(),
            "wave" : self.wave_var.get(),
            "stat" : self.stat_var.get()
        }
    def get_fields(self):
        return {
            "freq" : self.freq_var,
            "ampl" : self.ampl_var,
            "cycl" : self.cycl_var,
            "wave" : self.wave_var,
            "stat" : self.stat_var
        }

        

class CoilFields:
    def __init__(self, parent, coil_var : CoilVars, width=20, row=0, col=0):
        self.frame = tk.Frame(parent, bg = gmc.pewter)
        self.frame.grid(row=row, column=col, padx=5, pady=5, sticky="nsew")

        self.label = tk.Label(self.frame, text=f"(id {coil_var.coil.id}) {coil_var.coil.name}", bg=gmc.steel)
        self.label.pack(anchor='w', fill='x')

        fields = coil_var.get_fields();
        wave_types = coil_var.waveforms
        self.freq_entry = LabeledEntry(self.frame, "Freq [Hz]", 10       , fields['freq'])
        self.ampl_entry = LabeledEntry(self.frame, "Ampl [A] ", 0.25     , fields['ampl'])
        self.cycl_entry = LabeledEntry(self.frame, "Cycles   ", 100      , fields['cycl'])
        self.stat_entry = LabeledEntry(self.frame, "Status   ", "Unknown", fields['stat'], "readonly")
        wave = fields['wave']
        wave.set(wave_types[0])
        tk.OptionMenu(self.frame, wave, wave_types[0], *wave_types[1:], command=lambda x:print(f"Selected {x}")).pack(side="left", padx=5, pady=5, fill = 'x', expand='true')
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
    self.coil_list = [ CoilName(0, 'MHF1C01H'), CoilName(1, 'MHF1C02H'), CoilName(2, 'MHF1C03V'), CoilName(3,'MHF1C08H'), CoilName(4,'MHF1C08V'), CoilName(5,'MHF1C10H'), CoilName(6,'MHF1C10V'), CoilName(7,'SL 20 Vernier')]
    self.coil_names= [coil.name for coil in self.coil_list]
    self.coil_ids  = [coil.id for coil in self.coil_list]
    self.coil_fields= [] # will be filled later
    self.coil_values= [] # will be filled later
    self.waveforms = ['Sine', 'Ramp']

    tk.Label(tab, text="Coils", bg=gmc.green).pack(anchor="nw", pady=(5,0))

    ## Coil Frame
    coils_frame = tk.Frame(tab, width=600, height=400, bg=gmc.green)
    coils_frame.pack(padx=5, pady=5, fill="both", expand=True)
    coils_grid  = tk.Frame(coils_frame, bg=gmc.green)
    coils_grid.pack(fill="both", expand=True)
    
    ## Coil Fields
    MAX_COLUMNS = 4
    for index, coil in enumerate(self.coil_list):
        row = index // MAX_COLUMNS
        col = index % MAX_COLUMNS
        coils_grid.grid_columnconfigure(col, weight=1)
        coils_grid.grid_rowconfigure(row, weight=1)

        var = CoilVars(coils_grid, coil, self.waveforms)
        field = CoilFields(coils_grid, var, 20, row, col)
        self.coil_values.append(var)
        self.coil_fields.append(field)

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
    tk.Button(sing_coil_tab, text="Load WaveForm", command=lambda : print("Load Sinwave Button Pressed")).pack(side="left", padx=5, pady=5)
    tk.Button(sing_coil_tab, text="Enable Trigger", command=lambda : print("Enter Trigger State Button Pressed")).pack(side="left", padx=5, pady=5)
    tk.Button(sing_coil_tab, text="Stop", command=lambda : print(f"Stop Button Pressed, {self.coil_fields[0].field.get_monitors()['freq'].set(1)}")).pack(side="left", padx=5, pady=5)

    cycle_notebook.add(sup_cyc_tab, text="Super Cycle")
    cycle_notebook.add(sing_coil_tab, text="Single Coil")
