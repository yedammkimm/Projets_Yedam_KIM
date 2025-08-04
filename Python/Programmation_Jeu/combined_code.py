
import random as rd
import tkinter as tk
from tkinter import messagebox
from abc import ABC, abstractmethod

Heal_distance = 5
Ranged_distance = 20
Melee_distance = 10

# Question 1
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
        info = f"{self.__class__.__name__} {self.name} has {self.HP} health points at position {self.position}."
        if isinstance(self, Attacker):
            info += f" Attack points: {self.attack_points}."
            if self.ranged:
                info += " The Attacker is ranged."
            else:
                info += " The Attacker is melee."
        if isinstance(self, Healer):
            info += f" Heal points: {self.heal_points}."
        if isinstance(self, Paladin):
            info += f" Attack points: {self.attack_points}, Heal points: {self.heal_points}."
        if self.alive:
            info += " The character is alive."
        else:
            info += " The character is dead."
        return info

class Attacker(Character):
    def __init__(self, name, attack_points, ranged=True):
        super().__init__(name)
        self.attack_points = attack_points
        self.ranged = ranged

    def action(self):
        pass

    def fight(self, opponent):
        distance = abs(self.position - opponent.position)
        if (self.ranged and distance <= Ranged_distance) or (not self.ranged and distance <= Melee_distance):
            damage = self.attack_points
            print(f"{self.name} attacks {opponent.name} and inflicts {damage} damage points.")
            opponent.HP -= damage
            if opponent.HP <= 0:
                opponent.alive = False
                print(f"{opponent.name} is dead.")
        else:
            print(f"{self.name} is too far to attack {opponent.name}.")

class Healer(Character):
    def __init__(self, name, heal_points):
        super().__init__(name)
        self.heal_points = heal_points

    def action(self):
        pass

    def heal(self, ally):
        distance = abs(self.position - ally.position)
        if distance <= Heal_distance:
            print(f"{self.name} heals {ally.name} and restores {self.heal_points} health points.")
            ally.HP += self.heal_points
            self.HP -= 1  # Healer loses health when healing
        else:
            print(f"{self.name} is too far to heal {ally.name}.")

class Paladin(Attacker, Healer):
    def __init__(self, name, attack_points, heal_points):
        Attacker.__init__(self, name, attack_points, ranged=False)
        Healer.__init__(self, name, heal_points)

    def action(self):
        pass

# Question 5
class CharacterError(Exception):
    pass

class Game:
    def __init__(self):
        self.characters = []

    def add_character(self, character):
        self.characters.append(character)

    def create_character(self, character_type, name, health_points, attack_points=0, heal_points=0):
        try:
            if character_type == "Warrior":
                character = Attacker(name, attack_points, ranged=False)
            elif character_type == "Healer":
                character = Healer(name, heal_points)
            elif character_type == "Paladin":
                character = Paladin(name, attack_points, heal_points)
            else:
                raise CharacterError("Unknown character type")
            self.add_character(character)
        except CharacterError as e:
            print(f"Error: {e}")

    def display_characters(self):
        for character in self.characters:
            print(character.allinfo())

# Question 7
class Application:
    def __init__(self, title, team1, team2):
        self.window = tk.Tk()
        self.window.title(title)
        self.team1 = team1
        self.team2 = team2
        self.action_in_progress = None
        self.selected_attacker = None
        self.selected_target = None

        self.setup_ui()

    def setup_ui(self):
        self.listbox_team1 = tk.Listbox(self.window)
        self.listbox_team2 = tk.Listbox(self.window)
        self.button_attack = tk.Button(self.window, text="Attack", command=self.handle_attack)
        self.button_heal = tk.Button(self.window, text="Heal", command=self.handle_heal)
        self.text_team1 = tk.Text(self.window, height=10, width=30)
        self.text_team2 = tk.Text(self.window, height=10, width=30)

        self.listbox_team1.grid(row=0, column=0)
        self.listbox_team2.grid(row=0, column=2)
        self.button_attack.grid(row=1, column=0)
        self.button_heal.grid(row=1, column=2)
        self.text_team1.grid(row=2, column=0)
        self.text_team2.grid(row=2, column=2)

        for character in self.team1:
            self.listbox_team1.insert(tk.END, character.name)
        for character in self.team2:
            self.listbox_team2.insert(tk.END, character.name)

    def handle_attack(self):
        self.action_in_progress = "attack"
        self.listbox_team1.bind("<<ListboxSelect>>", self.select_attacker)
        self.listbox_team2.bind("<<ListboxSelect>>", self.select_target)

    def handle_heal(self):
        self.action_in_progress = "heal"
        self.listbox_team1.bind("<<ListboxSelect>>", self.select_attacker)
        self.listbox_team2.unbind("<<ListboxSelect>>")

    def select_attacker(self, event):
        index = self.listbox_team1.curselection()[0]
        self.selected_attacker = self.team1[index]
        self.text_team1.delete("1.0", tk.END)
        self.text_team1.insert(tk.END, self.selected_attacker.allinfo())

    def select_target(self, event):
        if self.action_in_progress == "attack":
            index = self.listbox_team2.curselection()[0]
            self.selected_target = self.team2[index]
            self.text_team2.delete("1.0", tk.END)
            self.text_team2.insert(tk.END, self.selected_target.allinfo())
            self.perform_action()

    def perform_action(self):
        if self.action_in_progress == "attack":
            try:
                self.selected_attacker.fight(self.selected_target)
                self.update_display()
            except Exception as e:
                messagebox.showinfo("Error", str(e))
        elif self.action_in_progress == "heal":
            try:
                self.selected_attacker.heal(self.selected_target)
                self.update_display()
            except Exception as e:
                messagebox.showinfo("Error", str(e))

    def update_display(self):
        self.text_team1.delete("1.0", tk.END)
        self.text_team1.insert(tk.END, self.selected_attacker.allinfo())
        self.text_team2.delete("1.0", tk.END)
        self.text_team2.insert(tk.END, self.selected_target.allinfo())

    def run(self):
        self.window.mainloop()

# Create example teams
team1 = [Attacker("Warrior1", 100, 20), Healer("Healer1", 80, 15)]
team2 = [Attacker("Warrior2", 120, 25), Paladin("Paladin1", 110, 18, 12)]

app = Application("Battle Game", team1, team2)
app.run()
