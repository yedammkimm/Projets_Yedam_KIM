# Gestion de Comptes Bancaires

## Compilation
Exécutez la commande suivante pour compiler les programmes :
```
make
```

## Exécution
1. **Démarrer le serveur TCP** :
```
./serv_tcp
```

2. **Démarrer le client TCP** :
```
./clie_tcp
```

3. **Démarrer le serveur UDP** :
```
./serv_udp
```

4. **Démarrer le client UDP** :
```
./clie_udp
```

## Commandes disponibles :
- **AJOUT** : Ajouter de l'argent à un compte  
  Format : `AJOUT <id_client> <id_compte> <password> <somme>`

- **RETRAIT** : Retirer de l'argent d'un compte  
  Format : `RETRAIT <id_client> <id_compte> <password> <somme>`

- **SOLDE** : Consulter le solde du compte  
  Format : `SOLDE <id_client> <id_compte> <password>`

- **OPERATIONS** : Consulter les 10 dernières opérations effectuées sur le compte  
  Format : `OPERATIONS <id_client> <id_compte> <password>`

## Nettoyer le projet
Pour supprimer les fichiers compilés :
```
make clean
```

## Remarque
- Le serveur TCP écoute sur le port **8080** et le serveur UDP sur le port **8081**.
- Le client doit être lancé après le serveur.
- Assurez-vous que les identifiants de compte et mots de passe sont corrects.
- Le serveur prend en charge plusieurs clients simultanément grâce aux **threads**.
