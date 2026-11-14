import tkinter as tk
from tkinter import ttk
import GUI.utils as u
import GUI.Colors as gmc
from GUI.tabs.gm_tab import BaseTab
import Bindings.RemexClient as Remex
import Bindings.BMW as bmw

class LabeledEntry:
    def __init__(self, parent, label, entry_val, rw="normal"):
        self._frame = tk.Frame(parent, bg = gmc.pewter)
        self._frame.pack(anchor='w', pady=(5,5), fill='x', expand=True)

        self._label = tk.Label(self._frame, text=label, font=("Arial", 10), bg = gmc.pewter)
        self._label.pack(side='left', padx=(0,5), expand=True)

        self._rb = tk.Entry(self._frame, width=5, font=("Arial", 10), justify="center", textvariable=entry_val, state="readonly")
        self._rb.pack(side="right", fill='x', expand=True)

        self._entry = tk.Entry(self._frame, width=10, font=("Arial", 10), justify="center", textvariable=entry_val, state=rw)
        self._entry.pack(side="right", fill='x', expand=True)
    def GetEntry(self):
        return self.entry.get()
    def LockEntry(self):
        self.entry.config(state="readonly")
    def UnlockEntry(self):
        self.entry.config(state="normal")

class CoilVars:
    def __init__(self, parent, coil, waveforms):
        self.coil     = coil
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

    def __init__(self, parent, coil: bmw.BMWCoils, width=20, row=0, col=0):
        self.frame = tk.Frame(parent, bg = gmc.pewter)
        self.frame.grid(row=row, column=col, padx=5, pady=5, sticky="nsew")


        title_frame = tk.Frame(self.frame, bg = gmc.steel)
        title_frame.pack(fill='x')
        self.label = tk.Label(title_frame, text=f"ID {coil.GetId()}: {coil.GetName()}", bg=gmc.steel)
        self.label.pack(side = 'top', anchor='w',padx = (10,0), fill='x')

        self.freq_var = tk.StringVar(self.frame, value="10")
        self.ampl_var = tk.StringVar(self.frame, value="0.25")
        self.cycl_var = tk.StringVar(self.frame, value="100")
        self.wave_var = tk.StringVar(self.frame, value=bmw.WaveformType.Sine.name)
        self.stat_var = tk.StringVar(self.frame, value=bmw.CoilStatus(bmw.GetCoilStatus(coil)).name)

        entry_frame = tk.Frame(self.frame, bg = gmc.pewter)
        entry_frame.pack(fill='x')
        self.freq_entry = LabeledEntry(entry_frame, "Freq [Hz]", self.freq_var)
        self.ampl_entry = LabeledEntry(entry_frame, "Ampl [A] ", self.ampl_var)
        self.cycl_entry = LabeledEntry(entry_frame, "Cycles   ", self.cycl_var)
        self.stat_entry = LabeledEntry(entry_frame, "Status   ", self.stat_var, "readonly")

        opt = tk.OptionMenu(entry_frame, self.wave_var, *[w.name for w in bmw.WaveformType], command=lambda x:print(f"Selected {x}"))
        opt.pack(side="left", padx=5, pady=5, fill = 'x', expand=True)
        opt.config(width=10, font=("Arial", 10), anchor='w')
        opt['menu'].config(font=("Arial", 10))

    
        apply_frame = tk.Frame(self.frame, bg=gmc.pewter)
        apply_frame.pack(fill='x')
        tk.Button(apply_frame, text="Apply", width=10, font=("Arial", 10), command=lambda: print("Apply Button Pressed")).pack(side = "left", fill='x', padx=(5,5), pady=(2,2), expand=True)
        trigger_frame = tk.Frame(self.frame, bg=gmc.pewter)
        trigger_frame.pack(fill='x', expand=True)
        self.enable_btn = u.BinaryButton(trigger_frame, "Trigger Disable ", "Trigger Enable", width = 10,font=("Arial", 10))
        self.enable_btn.pack(side='left', fill='x', padx=(5,5), pady=(1,0), expand=True)

    def GetFreq(self):
        return self.freq_entry.GetEntry()
    def GetAmpl(self):
        return self.ampl_entry.GetEntry()
    def GetCycl(self):
        return self.cycl_entry.GetEntry()


class BeamMod(BaseTab):
  def __init__(self, tab):
     super().__init__(tab)
     #print(f"BeamMod Startup = {bmw.BeamModStartup()}")
     self.CoilManager = bmw.BeamModCoils()
     for coil in self.CoilManager:
          print(f"coil name = {coil.GetName()}")
    ## Member vars
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
     for index, coil in enumerate(self.CoilManager):
         row = index // MAX_COLUMNS
         col = index % MAX_COLUMNS
         coils_grid.grid_columnconfigure(col, weight=1)
         coils_grid.grid_rowconfigure(row, weight=1)

         field = CoilFields(coils_grid, coil, 20, row, col)
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

     #self.wave_opt = tk.StringVar(self)
     #self.menu_opt = tk.StringVar(self)
     #self.wave_opt.set(self.waveforms[0]);
     #self.menu_opt.set(self.coil_names[0]);

     #tk.OptionMenu(sing_coil_tab, self.menu_opt, self.coil_names[0], *self.coil_names[1:], command=lambda x:print(f"Selected {x}")).pack(side="left", padx=5, pady=5)
     #tk.Button(sing_coil_tab, text="Load WaveForm", command=lambda : print("Load Sinwave Button Pressed")).pack(side="left", padx=5, pady=5)
     #tk.Button(sing_coil_tab, text="Enable Trigger", command=lambda : print("Enter Trigger State Button Pressed")).pack(side="left", padx=5, pady=5)
     #tk.Button(sing_coil_tab, text="Stop", command=lambda : print(f"Stop Button Pressed, {self.coil_fields[0].field.get_monitors()['freq'].set(1)}")).pack(side="left", padx=5, pady=5)

     cycle_notebook.add(sup_cyc_tab, text="Super Cycle")
     cycle_notebook.add(sing_coil_tab, text="Single Coil")

  def CleanUp(self):
     print("beammod::Destory called")
     self.CoilManager.Close()
        
        
     
     
