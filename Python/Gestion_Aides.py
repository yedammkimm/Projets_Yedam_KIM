from abc import ABC, abstractmethod
import tkinter as tk 
from tkinter import messagebox


#Question1
#Aide   
class Aide(ABC):
    def __init__(self, factures):
        self.factures = factures
    
    @abstractmethod
    def calculer_aide(self):
        pass

#Logement
class Logement(ABC):
    @abstractmethod
    def calculer_aide(self):
        pass


#PrimeRenov
class PrimeRenov(Logement):
    def __init__(self, factures):
        self.factures = factures
    
    def calculer_aide(self):
        nb_criteres = 0
        montant_total = 0
        for facture in self.factures:
            if facture.satisfait_critere():
                nb_criteres += 1
            montant_total += facture.montant - facture.calculer_reduction()
        if nb_criteres >= 3:
            return montant_total
        raise Exception("Il faut au moins 3 factures satisfaisant un critère pour obtenir une aide.")

#PrimeRenovSerenite 
class PrimeRenovSerenite(PrimeRenov):
    def __init__(self, factures, taux_imposition):
        super().__init__(factures)
        self.taux_imposition = taux_imposition
    
    def calculer_aide(self):
        montant = super().calculer_aide()
        reduction_supplementaire = montant * (self.taux_imposition / 100)
        return montant + reduction_supplementaire

#Facture
class Facture:
    def __init__(self, montant, critere):
        self.montant = montant
        self.critere = critere
    
    def satisfait_critere(self):
        return self.critere.est_satisfait()
    
    def calculer_reduction(self):
        return self.critere.calculer_reduction(self.montant)

#Critere
class Critere(ABC):
    @abstractmethod
    def est_satisfait(self):
        pass
    
    @abstractmethod
    def calculer_reduction(self, montant):
        pass

#Efficacite
class Efficacite(Critere):
    def __init__(self, taux):
        self.taux = taux
    
    def est_satisfait(self):
        return self.taux > 0
    
    def calculer_reduction(self, montant):
        return montant * (self.taux / 100)

#TypeTravaux
class Typetravaux(Critere):
    def __init__(self, reduction):
        self.reduction = reduction
    
    def est_satisfait(self):
        return self.reduction > 0
    
    def calculer_reduction(self, montant):
        return self.reduction

#Beneficiaire
class Beneficiaire:
    def __init__(self, nom, prenom, revenu):
        self.nom = nom
        self.prenom = prenom
        self.revenu = revenu
        self.aides = []
    
    def ajouter_aide(self, aide):
        self.aides.append(aide)
    
    def calculer_total_aide(self):
        total = 0
        for aide in self.aides:
            total += aide.calculer_aide()
        return total


#Question 2

fac1 = Facture(550, Efficacite(10))
fac2 = Facture(333, Typetravaux(70))
fac3 = Facture(180, Efficacite(20))
fac4 = Facture(2000,Efficacite(40))

aide1 = PrimeRenov([fac1, fac2, fac3])
aide2 = PrimeRenovSerenite([fac1, fac2, fac3], 5)
#aide3 = PrimeRenov([fac4, fac4])
aide3 = PrimeRenov([fac4, fac4, fac3])
aide4 = PrimeRenovSerenite([fac1, fac4, fac2], 40)

beneficiaire = Beneficiaire("KIM", "Yedam", 20000)
beneficiaire1 = Beneficiaire("FILALI DARAI", "Aymane", 50000)
beneficiaire.ajouter_aide(aide1)
beneficiaire.ajouter_aide(aide2)
beneficiaire.ajouter_aide(aide3)
beneficiaire.ajouter_aide(aide4)
beneficiaire1.ajouter_aide(aide1)
beneficiaire1.ajouter_aide(aide2)
beneficiaire1.ajouter_aide(aide3)
beneficiaire1.ajouter_aide(aide4)


#print(beneficiaire.calculer_total_aide())
#print(beneficiaire1.calculer_total_aide())



#Question 3
root = tk.Tk()
root.title("Liste des aides")

listbox = tk.Listbox(root, height=20, width=50, selectmode=tk.MULTIPLE)
listbox1 = tk.Listbox(root, height=20, width=50, selectmode=tk.MULTIPLE)
listbox.grid(row=0, column=0)
listbox1.grid(row=0, column=1)
button = tk.Button(root, text="Montant", command=lambda: show_aide(beneficiaire, listbox))
button1 = tk.Button(root, text="Montant", command=lambda: show_aide(beneficiaire1, listbox1))
button.grid(row=1, column=0)
button1.grid(row=1, column=1)

#Question 4
def show_aide(beneficiaire, listbox):
    selected_indices = listbox.curselection()
    total = 0
    for i in selected_indices:
        total += beneficiaire.aides[i].calculer_aide()
    messagebox.showinfo("Montant total des aides", f"Le montant total des aides sélectionnées pour {beneficiaire.nom} {beneficiaire.prenom} est: {total} €")

for aide in beneficiaire.aides:
    listbox.insert(tk.END, f"Aide de {beneficiaire.nom} {beneficiaire.prenom}: {aide.calculer_aide()} €")

for aide in beneficiaire1.aides:
    listbox1.insert(tk.END, f"Aide de {beneficiaire1.nom} {beneficiaire1.prenom}: {aide.calculer_aide()} €")

root.mainloop()


#Question 5
    
#Ligne 38 
