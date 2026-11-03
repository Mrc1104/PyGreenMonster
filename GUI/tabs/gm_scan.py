'''
Green Monster GUI Revamp
Containing ScanUtil Tab
Code Commissioned 2019-01-16
Code by A.J. Zec
'''

import tkinter as tk
from tkinter import ttk
import GUI.utils as u
import os
import subprocess
import RemexClient.RemexClient as Remex
import GUI.Colors as gmc

SCAN_GET_DATA	=1001
SCAN_SET_DATA	=1002
SCAN_GET_STATUS	=1003
GM_SCN_SET	=6003
SCN_INT_CLN	=1
SCN_INT_NOT	=0

class ScanUtil(tk.Frame):
  def __init__(self, tab, gm, rocname):
    self.remex = Remex.RemexClient(Remex.RemexClientLib, bytes(rocname, "utf-8"), gm.cmsg);
    print(f"Connecting to Remex Client on {rocname}")
    self.util_frame = tk.LabelFrame(tab, text='SCAN UTILITY', bg=gmc.green)
    self.GM = gm
    self.options = ['CLEAN', 'NOT CLEAN', 'GRAB EPICS/NOT CLEAN']
    self.clean_setting = tk.StringVar()
    self.clean_setting.set(self.options[0])
    self.inj_frame = tk.LabelFrame(self.util_frame, text='Inj', bg=gmc.green)
    self.inj_labels = [tk.Label(self.inj_frame, text='Set Point 1', bg=gmc.green),
                       tk.Label(self.inj_frame, text='Set Point 2', bg=gmc.green),
                       tk.Label(self.inj_frame, text='Set Point 3', bg=gmc.green),
                       tk.Label(self.inj_frame, text='Set Point 4', bg=gmc.green)]
    self.inj_entries = [tk.Entry(self.inj_frame),
                        tk.Entry(self.inj_frame),
                        tk.Entry(self.inj_frame),
                        tk.Entry(self.inj_frame)]
    self.epics_frame = tk.LabelFrame(self.util_frame, text='EPICS', bg=gmc.green)
    self.epics_labels = [tk.Label(self.epics_frame, text='Epics Variable 1', bg=gmc.green),
                         tk.Label(self.epics_frame, text='Epics Variable 2', bg=gmc.green),
                         tk.Label(self.epics_frame, text='Epics Variable 3', bg=gmc.green),
                         tk.Label(self.epics_frame, text='Epics Variable 4', bg=gmc.green)]
    self.epics_entries = [tk.Entry(self.epics_frame),
                          tk.Entry(self.epics_frame),
                          tk.Entry(self.epics_frame),
                          tk.Entry(self.epics_frame)]
    self.timer_frame = tk.LabelFrame(self.util_frame, text='Timer (seconds)', bg=gmc.green)
    self.timer_labels = [tk.Label(self.timer_frame, text='0', fg='#ffffff', bg='#3C5353', padx=10, pady=10)]
    self.timer_entries = [tk.Entry(self.timer_frame)]
    for i, op in enumerate(self.options):
      tk.Radiobutton(self.util_frame, text=op, variable=self.clean_setting,
          value=op, bg=gmc.green, command=self.set_status).grid(row=0, column=i, padx=10, pady=10, sticky='W')
    self.fill_inj_frame()
    self.inj_frame.grid(row=1, column=0, columnspan=2, padx=10, pady=10, sticky='W')
    self.fill_epics_frame()
    self.epics_frame.grid(row=1, column=2, columnspan=1, padx=10, pady=10, sticky='W')
    self.fill_timer_frame()
    self.timer_frame.grid(row=3, column=0, columnspan=5, padx=10, pady=10, sticky='W')
    tk.Button(self.util_frame, text='Check Values', bg=gmc.green, command=self.check_values).grid(row=2, column=0, padx=10, pady=10)
    tk.Button(self.util_frame, text='Set Values', bg=gmc.green, command=self.set_values).grid(row=2, column=1, padx=10, pady=10)
    tk.Button(self.util_frame, text='Grab EPICS', bg=gmc.green, command=self.check_EPICS).grid(row=2, column=2, padx=10, pady=10)
    tk.Button(self.timer_frame, text='Start', bg=gmc.green, command=lambda: self.start_timer()).grid(row=0, column=2, padx=5, pady=5)
    self.pause_btn = tk.Button(self.timer_frame, text='Pause', bg=gmc.green, command=lambda: self.pause_timer())
    self.pause_btn.grid(row=0, column=3, padx=5, pady=5)
    tk.Button(self.timer_frame, text='Clear', bg=gmc.green, command=lambda: self.clear_timer()).grid(row=0, column=4, padx=5, pady=5)
    self.util_frame.pack(padx=20, pady=20, anchor='w')
    self.check_values()
    self.startTimer = False
    self.timer_loop()

  def fill_inj_frame(self):
    for r in range(0, 4):
      self.inj_labels[r].grid(row=r, column=0, padx=15, pady=10, sticky='E')
      u.set_text(self.inj_entries[r], '0').grid(row=r, column=1, padx=10, pady=10, sticky='W')

  def fill_epics_frame(self):
    for r in range(0, 4):
      self.epics_labels[r].grid(row=r, column=0, padx=15, pady=10, sticky='E')
      u.set_text(self.epics_entries[r], 'None').grid(row=r, column=1, padx=10, pady=10, sticky='W')

  def fill_timer_frame(self):
    self.timer_labels[0].grid(row=0, column=0, padx=15, pady=10, sticky='E')
    u.set_text(self.timer_entries[0], '0').grid(row=0, column=1, padx=10, pady=10, sticky='W')

  def start_timer(self):
    self.startTimer = True
    self.timer_labels[0].config(text=self.timer_entries[0].get())
    self.timer_frame.config(bg=gmc.green)
    self.timer_labels[0].config(bg='#3C5353')

  def timer_loop(self):
    # Make a timing thing: 
    if self.startTimer is True:
      try:
        localTime = int(float(self.timer_labels[0].cget("text")))
      except ValueError:
        localTime = 0
      if localTime<=0:
        localTime = 0
        self.startTimer = False
        self.timer_frame.config(bg='#9E1A1A')
        self.timer_labels[0].config(bg='#3E1515')
      else:
        localTime = localTime - 1
      self.timer_labels[0].config(text=str(localTime))
      #self.timer_entries[0].delete(0, tk.END)
      #self.timer_entries[0].insert(0, str(localTime))
    self.GM.win.after(1000,self.timer_loop) # Recursion loop here - splits off a new instance of this function and finishes the one currently running (be careful)
    #else:
    #  gm.win.after(1000,self.timer_loop(gm),gm) # Recursion loop here - splits off a new instance of this function and finishes the one currently running (be careful)

  def resume_timer(self):
    self.startTimer = True
    self.timer_frame.config(bg=gmc.green)
    self.timer_labels[0].config(bg='#3C5353')
    self.pause_btn.configure(text='Pause', bg=gmc.green, command=lambda: self.pause_timer())
    
  def pause_timer(self):
    self.startTimer = False
    self.timer_frame.config(bg=gmc.green)
    self.timer_labels[0].config(bg=gmc.timer_yellow)
    self.pause_btn.configure(text='Resume', bg=gmc.timer_yellow, command=lambda: self.resume_timer())
    

  def clear_timer(self):
    self.startTimer = False
    self.timer_labels[0].config(text='0')
    #self.timer_entries[0].delete(0, tk.END)
    #self.timer_entries[0].insert(0, '0')
    self.timer_frame.config(bg=gmc.green)
    self.timer_labels[0].config(bg=gmc.timer_red)

  # scandata indexing starts at 0
  def get_scandata(self, setpoint : int):
    command = "getSCANData({})".format(setpoint+1)
    Received = self.remex.send(bytes(command, "utf-8"), Remex.RemexEnum.INT)
    return Received.success, Received.values.int

  def get_cleandata(self):
    command = "getSCANClean()"
    Received = self.remex.send(bytes(command, "utf-8"), Remex.RemexEnum.INT)
    return Received.success, Received.values.int

  def set_cleandata(self, clean : bool):
    command = "setSCANClean({})".format(int(clean))
    Received = self.remex.send(bytes(command, "utf-8"), Remex.RemexEnum.VOID)
    return Received.success

  def set_scandata(self, setpoint : int, value : int):
    command = "setSCANData({},{})".format(setpoint+1, value)
    Received = self.remex.send(bytes(command, "utf-8"), Remex.RemexEnum.VOID)
    return Received.success

  def check_status(self):
    pass

  def check_values(self):
    for scandata in range(0, 4):
        status, value = self.get_scandata(scandata) 
        if(status == True):
            self.inj_entries[scandata].delete(0, tk.END)
            self.inj_entries[scandata].insert(0, str(value))
            self.inj_labels[scandata].config(bg=gmc.green)
        else:
            self.inj_labels[scandata].config(bg=gmc.red)

  def check_EPICS(self):
    i = 0
    while i < 4:
    #  if inj_
      cmds = ['caget', '-t', '-w 1', self.epics_entries[i].get()]
      value = "NULL"
      if "None" not in self.epics_entries[i].get():
        value = subprocess.Popen(cmds, stdout=subprocess.PIPE).stdout.read().strip().decode('ascii') # Needs to be decoded... be careful 
        if value != "NULL" and "Invalid" not in value:
          #self.inj_entries[i] = str(value)
          value = int(round(float(value)))
          self.inj_entries[i].delete(0, tk.END)
          self.inj_entries[i].insert(0, str(value))
      i += 1
    self.check_status()

  def set_status(self):
    if self.clean_setting.get() == 'CLEAN':
      status = 1
    else:
      status = 0

    if self.clean_setting.get() == 'GRAB EPICS/NOT CLEAN':
      i = 0
      while i < 4:
      #  if inj_
        cmds = ['caget', '-t', '-w 1', self.epics_entries[i].get()]
        value = "NULL"
        if "None" not in self.epics_entries[i].get():
          value = subprocess.Popen(cmds, stdout=subprocess.PIPE).stdout.read().strip().decode('ascii') # Needs to be decoded... be careful 
          if value != "NULL" and "Invalid" not in value:
            #self.inj_entries[i] = str(value)
            self.inj_entries[i].delete(0, tk.END)
            self.inj_entries[i].insert(0, str(value))
        i += 1

    self.set_values()

    success = self.set_cleandata(status)
    print("Setting SCN status: " + str(status))

    self.check_values()

  def set_values(self):
    for scandata in range(0, 4):
        value = int(round(float(self.inj_entries[scandata].get())))
        if(self.set_scandata(scandata, value) == True):
            self.inj_labels[scandata].config(bg=gmc.green)
        else:
            self.inj_labels[scandata].config(bg=gmc.red)


