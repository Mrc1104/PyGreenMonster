'''
Green Monster GUI Revamp
Code Commissioned 2019-01-04
Code by A.J. Zec
'''

import tkinter as tk
from tkinter import ttk
import distutils.util
import os
import webbrowser

import GUI.tabs.gm_scan as scan_util
import GUI.tabs.gm_bmw as beam_mod
import GUI.tabs.gm_ex  as example
import GUI.Colors as gmc
import Bindings.RemexClient as Remex

class GreenMonster:

    def __init__(self):
        os.environ["TCL_LIBRARY"] = "/lib64/tcl8.5/"
        self.win = tk.Tk()
        self.win.title("Green Monster")
        self.win.geometry("800x775")
        self.win.configure(background=gmc.green)
        self.adc_options_vars = [tk.StringVar(), tk.StringVar(), tk.StringVar(), tk.StringVar()]
        self.bm_test_setting = tk.StringVar()
        self.clean_setting = tk.StringVar()
        self.get_green_monster_style()
        # Example: self.cmsg   = CMsgServer(RemexClientLib,            CMsgHost             ,          CMsgPassword              )
        # Example: self.cmsg   = CMsgServer(RemexClientLib, bytes("adaq1.jlab.org", "utf-8"), bytes("MollerIntegrating", "utf-8"))
        self.cmsg = Remex.CMsgServer(Remex.RemexClientLib, None, None)
        self.create_footer()
        self.create_widgets()

    def get_green_monster_style(self):
        style = ttk.Style()
        style.theme_create("green_monster", parent="alt", settings={
            "TNotebook": {"configure": {"background": gmc.green}},
            "TNotebook.Tab": {"configure": {"background": gmc.green}},
            "TFrame": {"configure": {"background": gmc.green}},
            "TLabel": {"configure": {"background": gmc.green}}})
        style.theme_use("green_monster")

    def quit(self):
        self.win.destroy()

    def educate_yourself(self, event):
      webbrowser.open_new(r"https://en.wikipedia.org/wiki/Green_Monster")

    def expert_tab(self, expt_tab):
        tab_control = ttk.Notebook(expt_tab)
        tab_titles = []
        if self.checkHRSflag():
          tab_titles = [('TimeBoard', tmbd.Timeboard), 
                        ('VQWK ADCs', vqwk.VQWK), 
                        ]
        else:
          tab_titles = [('TimeBoard', tmbd.Timeboard), 
                        ('VQWK ADCs', vqwk.VQWK), 
                        ]
        for title, fn in tab_titles:
            sub_tab = ttk.Frame(tab_control, width=800, height=600, style="My.TFrame")
            tab_control.add(sub_tab, text=title)
            fn(sub_tab)
        tab_control.grid(row=0, column=0, columnspan=2)

    def create_widgets(self):
        gui_style = ttk.Style()
        gui_style.configure('My.TButton', foreground=gmc.green)
        gui_style.configure('My.TFrame', background=gmc.green)

        tab_control = ttk.Notebook(self.win)
        tab_control.pack(side="top",expand=True, fill="both")
        tab_titles = [#('ScanUtil', scan_util.ScanUtil),
                      ('bmw', beam_mod.BeamMod), 
                      ('EX' , example.Example)]
                      #('ScanUtil', scan_util.ScanUtil)]
                      #('Expert', self.expert_tab)]
        for title, fn in tab_titles:
            tab = ttk.Frame(tab_control, style="My.TFrame")
            tab_control.add(tab, text=title)
            if "ScanUtil" in title:
              fn(tab,self, "ROC-isb")
            else:
              fn(tab)
    def create_footer(self):
        footer = ttk.Frame(self.win)
        footer.pack(side="bottom", fill='x', padx=10,pady=0)
        self.fenway = tk.PhotoImage(file='GUI/Resources/gm.ppm')
        fenway_pahk = tk.Label(footer, image=self.fenway, cursor="hand2", bg=gmc.green)
        fenway_pahk.pack(side="left")
        fenway_pahk.bind("<Button-1>", self.educate_yourself)
        tk.Button(footer, text='QUIT', command=self.quit, background=gmc.green, width=20, height=4).pack(side="right")


green_monster = GreenMonster()
green_monster.win.mainloop()
