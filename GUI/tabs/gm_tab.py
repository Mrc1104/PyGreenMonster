from abc import ABC, abstractmethod
import tkinter as tk

class BaseTab(tk.Frame, ABC):
    def __init__(self, parent, **kwargs):
        super().__init__(parent, kwargs)
        self._cleaned_up = False

    @abstractmethod
    def CleanUp(self) -> None:
        "Derived classes need to overload this method"
        pass
    def destroy(self):
        "Override of the tk.destroy method"
        if not self._cleaned_up:
            self._cleaned_up = True
            self.CleanUp()
        "Pass the call to children frames"
        super().destroy()
