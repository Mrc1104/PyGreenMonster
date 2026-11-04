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
import GUI.Colors as gmc
import Bindings.RemexClient as Remex

class GreenMonster:

    def __init__(self):
        os.environ["TCL_LIBRARY"] = "/lib64/tcl8.5/"
        self.win = tk.Tk()
        self.win.title("Green Monster")
        self.win.configure(background=gmc.green)
        self.adc_options_vars = [tk.StringVar(), tk.StringVar(), tk.StringVar(), tk.StringVar()]
        self.bm_test_setting = tk.StringVar()
        self.clean_setting = tk.StringVar()
        self.get_green_monster_style()
        # Example: self.cmsg   = CMsgServer(RemexClientLib,            CMsgHost             ,          CMsgPassword              )
        # Example: self.cmsg   = CMsgServer(RemexClientLib, bytes("adaq1.jlab.org", "utf-8"), bytes("MollerIntegrating", "utf-8"))
        self.cmsg = Remex.CMsgServer(Remex.RemexClientLib, None, None)
        self.create_widgets()

    def get_green_monster_style(self):
        style = ttk.Style()
        style.theme_create("green_monster", parent="alt", settings={
            "TNotebook": {"configure": {"background": gmc.green}},
            "TNotebook.Tab": {"configure": {"background": gmc.green}}})
        style.theme_use("green_monster")

    def quit(self):
        self.win.quit()
        self.win.destroy()
        exit()

    def educate_yourself(self, event):
      webbrowser.open_new(r"https://en.wikipedia.org/wiki/Green_Monster")

    def checkHRSflag(self):
      hrsinfile = open("/adaqfs/home/apar/devices/PyGreenMonster/includeHRS.flags",'r')
      tmp="False"
      for line in hrsinfile:
        print("Include HRSs = " + line)
        tmp=bool(distutils.util.strtobool(line.strip('\n')))
        print("Include HRSs = " + str(tmp))
        return tmp

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
        tab_titles = [#('BMW', bmw.BMW), 
                      ('ScanUtil', scan_util.ScanUtil)]
                      #('Expert', self.expert_tab)]
        for title, fn in tab_titles:
            tab = ttk.Frame(tab_control, width=800, height=600, style="My.TFrame")
            tab_control.add(tab, text=title)
            if "ScanUtil" in title:
              fn(tab,self, "ROC-isb")
            else:
              fn(tab)
        tab_control.grid(row=0, column=0, columnspan=2)
        fenway = tk.PhotoImage(file='GUI/Resources/gm.ppm')
        fenway_pahk = tk.Label(self.win, image=fenway, cursor="hand2", bg=gmc.green)
        fenway_pahk.image = fenway
        fenway_pahk.grid(row=1, column=0, padx=5, pady=10, sticky='SW')
        fenway_pahk.bind("<Button-1>", self.educate_yourself)
        tk.Button(self.win, text='QUIT', command=quit, background=gmc.green, width=20, height=4).grid(
            row=1, column=1, padx=15, pady=5, sticky='SE')

green_monster = GreenMonster()
green_monster.win.mainloop()
