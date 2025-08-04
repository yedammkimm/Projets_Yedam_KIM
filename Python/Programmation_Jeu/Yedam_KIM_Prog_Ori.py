import random as rd
import tkinter as tk
from tkinter import messagebox
from abc import ABC, abstractmethod

Heal_distance = 5
Ranged_distance = 20
Melee_distance = 10

class Character(ABC):
    def __init__(self, name):
        self.name = name
        self.HP = rd.randint(50, 100)
        self.position = rd.randint(0, 100)
        self.alive = True

    @abstractmethod
    def action(self):
        pass

    def allinfo(self):
        if ((isinstance(self, Attacker)) and (isinstance(self, Healer))):
            print("It's a Paladin")
            print("The Paladin's name:", self.name)
            print("The Paladin's HP:", self.HP)
            print("The Attacker's Position:", self.position)
            print("The Paladin's attack points:", self.attack_points)
            print("The Healer's heal points:", self.heal_points)   
            if self.alive: 
                print("The Paladin is alive")
            else:
                print("The Paladin is dead")
        else:
            if (isinstance(self, Attacker)):
                print("It's an Attacker")
                print("The Attacker's name:", self.name)
                print("The Attacker's HP:", self.HP)
                print("The Attacker's Position:", self.position)
                print("The Attacker's attack points:", self.attack_points)
                if self.ranged: 
                    print("The Attacker is ranged")
                else:
                    print("The Attacker is Melee")
                if self.alive: 
                    print("The Attacker is alive")
                else:
                    print("The Attacker is dead")
                
            elif (isinstance(self, Healer)):
                print("It's a Healer")
                print("The Healer's name:", self.name)
                print("The Healer's HP:", self.HP)
                print("The Attacker's Position:", self.position)
                print("The Healer's heal points:", self.heal_points)
                if self.alive: 
                    print("The Healer is alive")
                else:
                    print("The Healer is dead")

    def allinfo_window(self):
        stri = ""
        if isinstance(self, Attacker) and isinstance(self, Healer):
            stri += (
                "It's a Paladin\n"
                f"The Paladin's name: {self.name}\n"
                f"The Paladin's HP: {self.HP}\n"
                f"The Paladin's Position: {self.position}\n"
                f"The Paladin's attack points: {self.attack_points}\n"
                f"The Paladin's heal points: {self.heal_points}\n"
            )
            stri += "The Paladin is alive" if self.alive else "The Paladin is dead"
        elif isinstance(self, Attacker):
            stri += (
                "It's an Attacker\n"
                f"The Attacker's name: {self.name}\n"
                f"The Attacker's HP: {self.HP}\n"
                f"The Attacker's Position: {self.position}\n"
                f"The Attacker's attack points: {self.attack_points}\n"
            )
            stri += "The Attacker is ranged\n" if self.ranged else "The Attacker is melee\n"
            stri += "The Attacker is alive" if self.alive else "The Attacker is dead"
        elif isinstance(self, Healer):
            stri += (
                "It's a Healer\n"
                f"The Healer's name: {self.name}\n"
                f"The Healer's HP: {self.HP}\n"
                f"The Healer's Position: {self.position}\n"
                f"The Healer's heal points: {self.heal_points}\n"
            )
            stri += "The Healer is alive" if self.alive else "The Healer is dead"

        return stri

    def Ability(self, target):
        if abs(self.position - target.position) > 30: 
            print("The Character can not use his ability from his position")
        else:
            if ((isinstance(self, Attacker)) and (isinstance(self, Healer))):
                self.__healorattack__(target)
            elif (isinstance(self, Attacker)):
                self.__attack__(target)
            elif (isinstance(self, Healer)):
                self.__heal__(target)

class Attacker(Character): 
    def __init__(self, name, attack_points, attack_type):
        Character.__init__(self, name)
        if attack_points == "":
            self.attack_points = rd.randint(20, 50)
        else: 
            self.attack_points = attack_points
            
        if attack_type == "":
            print(self.name, "Attacker should be Ranged or Melee")
            self.ranged = (input() == "Ranged")
        else: 
            self.ranged = (attack_type == "Ranged")

    def action(self):
        pass
    
    def __attack__(self, target):
        self.allinfo()
        print()
        target.allinfo()
        print()
        if abs(self.position - target.position) > Ranged_distance and self.ranged:
            target.HP = target.HP - 2 * self.attack_points
            print("Critical Damage!!!")
        elif abs(self.position - target.position) < Melee_distance and not self.ranged:
            target.HP = target.HP - 2 * self.attack_points
            print("Critical Damage!!!")
        else: 
            print("Normal Damage ")
            target.HP = target.HP - self.attack_points
        if rd.randint(0, 1) == 1:
            print("Self Damage")
            self.HP = self.HP - self.attack_points
            if self.HP < 0:
                self.alive = False
        if target.HP < 0: 
            target.alive = False
        print("After Attack")
        print()
        self.allinfo()
        print()
        target.allinfo()

class Support(Character): 
    def __init__(self, name, heal_points):
        Character.__init__(self, name)
        if heal_points == "":
            self.heal_points = rd.randint(20, 50)
        else: 
            self.heal_points = heal_points

    def action(self):
        pass
        
    def __heal__(self, target):
        if target.alive == False: 
            print("The Ally is dead: Can not be healed")
            target = input("Choose Another Ally to heal: ")
        if abs(self.position - target.position) < Heal_distance:
            print("The Ally is too close: Can not be healed")
            target = input("Choose Another Ally to heal: ")
        else:
            self.allinfo()
            print()
            target.allinfo()
            target.HP = target.HP + self.heal_points
            print()
            print("After Heal")
            print()
            self.allinfo()
            print()
            target.allinfo()

class Warrior(Attacker):
    def __init__(self, name, attack_points, attack_type):
        Attacker.__init__(self, name, attack_points, attack_type)
        
    def __attack__(self, target):
        Attacker.__attack__(self, target)

class Healer(Support):
    def __init__(self, name, heal_points):
        Support.__init__(self, name, heal_points)
        
    def __heal__(self, target):
        Support.__heal__(self, target)

class Paladin(Warrior, Healer):
    def __init__(self, name, attack_points, heal_points, attack_type):
        Warrior.__init__(self, name, attack_points, attack_type)
        Healer.__init__(self, name, heal_points)
        
    def __healorattack__(self, target):
        if (input("What do you want the paladin to do ? ") == 'Attack'): 
            Warrior.__attack__(self, target)
        else: 
            Healer.__heal__(self, target)

# Question 4
B1 = Attacker("Aymane", "", "Ranged")
B2 = Healer("Nicolas", "")
B3 = Paladin("Quentin", "", "", "Melee")

R1 = Attacker("Victor", "", "Melee")
R2 = Healer("Maxime", "")
R3 = Paladin("Yedam", "", "", "Ranged")

# Question 5 and 6
class AttackingSameTargetException(Exception):
    pass

class Team:
    def __init__(self, name, members=[]):
        self.name = name
        self.members = members

    def add_member(self, member):
        self.members.append(member)

    def remove_member(self, member):
        if member in self.members:
            self.members.remove(member)
        else:
            print(member.name, "is not a member of", self.name)

    def display_members(self):
        print("Members of Team", self.name)
        for member in self.members:
            print("-", member.name)
            member.allinfo()

    def num_members(self):
        return len(self.members)

    def team_attack(self, target_team):
        self.display_members()
        print()
        target_team.display_members()
        print()
        attack_count = {}

Team1 = Team("Blue", [B1, B2])
Team2 = Team("Red", [R1, R3])
Team1.add_member(B3)
Team2.add_member(R2)

# Question 7 to 11
class Application:
    def __init__(self, battlename, team1, team2):
        self.battlename = battlename
        self.team1 = team1
        self.team2 = team2
        self.selected_attacker = None
        self.selected_target = None
        self.action_in_progress = None

    def update_selected_attacker(self, event=None):
        selected_indices = self.listbox_Team1.curselection()
        if selected_indices:
            self.selected_attacker = self.team1.members[selected_indices[0]]
            self.label1.config(text=self.selected_attacker.allinfo_window())
        else:
            self.selected_attacker = None

    def update_selected_target(self, event=None):
        selected_indices = self.listbox_Team2.curselection()
        if selected_indices:
            self.selected_target = self.team2.members[selected_indices[0]]
            self.label2.config(text=self.selected_target.allinfo_window())
            if self.action_in_progress == 'attack' and self.selected_attacker and self.selected_target:
                self.perform_attack()
        else:
            self.selected_target = None

    def perform_attack(self):
        if self.selected_attacker and self.selected_target:
            if isinstance(self.selected_attacker, Attacker):
                self.selected_attacker.__attack__(self.selected_target)
            self.update_selected_attacker()
            self.update_selected_target()
            self.clear_action()
        else:
            print("Select an attacker and a target to perform an attack.")

    def perform_heal(self):
        if self.selected_attacker and self.selected_target:
            if isinstance(self.selected_attacker, Healer):
                self.selected_attacker.__heal__(self.selected_target)
            self.update_selected_attacker()
            self.update_selected_target()
            self.clear_action()
        else:
            print("Select a healer and a target to perform healing.")

    def clear_action(self):
        self.selected_attacker = None
        self.selected_target = None
        self.action_in_progress = None
        self.listbox_Team1.selection_clear(0, tk.END)
        self.listbox_Team2.selection_clear(0, tk.END)
        self.label1.config(text="")
        self.label2.config(text="")

    def handle_attack_button(self):
        self.action_in_progress = 'attack'
        self.listbox_Team1.bind("<<ListboxSelect>>", self.update_selected_attacker)
        self.listbox_Team2.bind("<<ListboxSelect>>", self.update_selected_target)

    def handle_heal_button(self):
        self.action_in_progress = 'heal'
        self.listbox_Team1.bind("<<ListboxSelect>>", self.update_selected_attacker)
        self.listbox_Team2.bind("<<ListboxSelect>>", self.update_selected_target)

    def showcharacterstat(self):
        window = tk.Tk()
        window.title(self.battlename)
        window.geometry("640x400+100+100")
        window.resizable(True, True)

        self.listbox_Team1 = tk.Listbox(window, selectmode='single', height=0)
        for x in self.team1.members:
            self.listbox_Team1.insert(tk.END, x.name)
        self.listbox_Team1.grid(row=0, column=0)

        self.listbox_Team2 = tk.Listbox(window, selectmode='single', height=0)
        for x in self.team2.members:
            self.listbox_Team2.insert(tk.END, x.name)
        self.listbox_Team2.grid(row=0, column=2)

        self.button_attack = tk.Button(window, text="Attack", command=self.handle_attack_button)
        self.button_heal = tk.Button(window, text="Heal", command=self.handle_heal_button)
        self.button_attack.grid(row=1, column=0)
        self.button_heal.grid(row=1, column=2)

        self.label1 = tk.Label(window, text="", width=30, height=10, fg="black", relief="flat")
        self.label2 = tk.Label(window, text="", width=30, height=10, fg="black", relief="flat")
        self.label1.grid(row=2, column=0)
        self.label2.grid(row=2, column=2)

        window.mainloop()



#In order to start, you have to select someone on the first listbox and click attack first, The ability works based on the stats showed in the label. Stats should show first in order to use ability.
A = Application("Team1 vs Team2", Team1, Team2)
A.showcharacterstat()
