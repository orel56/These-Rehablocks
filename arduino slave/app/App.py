import tkinter as tk 
import list as li


class App(tk.Tk) :
    def __init__(self):
        super().__init__()
        self.label=[]
        self.current_state= "init"
        self.protocol("WM_DELETE_WINDOW",self.close) # Allows to close the window with the red cross
        self.title("Programmation de nouveaux blocs pour Réhablocks") 
        self.app_height= 200
        self.app_width= 550
        self.geometry(str(self.app_width)+"x"+str(self.app_height)) # Set the initial window's geometry 
        self.resizable(False,False)
        self.init_label()
        self.init_button()
        self.init_list()
    def close(self):
        self.quit()

    def init_label(self) :
        self.label.append(tk.Label(text="Selection du type de bloc"))
        self.label[0].place(x=150,y=0)
        self.label.append(tk.Label(text="Liste des devices disponibles : "))
        self.label[1].place(x=300,y=45)
    def init_list(self) :
        self.list=li.list()


    def init_button(self):
        self.next_button=tk.Button(self,text="next",state="disabled", activebackground="#c8ad7f",activeforeground="#fff",bg="#8b6c42",fg="#fff",highlightbackground="#8b6c42",relief="flat")
        self.next_button.place(x=150,y=45)
