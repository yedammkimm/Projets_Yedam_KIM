-- 1. Donner la ligne correspondant à la commune de nom 'Dijon'.
SELECT * from communes where nom = 'Dijon';
-- 2. Combien y a-t-il de communes en France ?
SELECT count(*) from communes;
-- 3. Combien de communes s’appellent 'Saint-Loup' ?
SELECT count(*) from communes where nom = 'Saint-Loup';
-- 4. Combien de communes contiennent la suite de lettres 'Saint' dans
-- leur nom, sans que le nom ne commence par cette même suite de
-- lettres ?
SELECT * from communes where nom = '%Saint%'
EXCEPT
SELECT * from communes where nom = '%Saint Saint%';

-- 5. Quels sont, sans doublons, les noms qui sont des noms de
-- communes, de départements ou de régions, dans l'ordre
-- lexicographique croissant ?
SELECT DISTINCT nom from communes 
UNION
SELECT DISTINCT nom from departements
UNION
SELECT DISTINCT nom from regions ASC;


-- 6. Quels sont les noms des communes qui portent le même nom que
-- leur département ?
SELECT c.nom from communes c
join departements d on d.nom = c.nom; 

-- 7. Quels sont, sans doublons, les noms des communes qui portent le
-- nom d'un département, sans que ce nom de commune soit celui d'une
-- commune qui porte le nom de son département ?

SELECT DISTINCT c.nom from communes c
join departements d on d.nom = c.nom
EXCEPT
SELECT DISTINCT c.nom from communes c 
join departements d on d.dep = c.dep;


-- 8. Dans combien de régions différentes existe-t-il une commune
-- s’appelant 'Sainte-Marie' ?

SELECT DISTINCT count (*) from regions r
join communes c on c.nom = 'Sainte-Marie' 
join departements d on d.dep = c.dep AND r.reg = d.reg;

-- 9. Quel est le taux (nombre flottant entre 0 et 1) de communes dont
-- le nom est constitué d'au moins trois mots séparés par des tirets ?
-- On pourra multiplier par 1.0 pour avoir une division flottante. On
-- pourra utiliser le fait qu'en SQLite le booléen `true` est
-- représenté par 1 et le booléen `false` par 0.

SELECT (count(*)*1.0) /(SELECT count(*) FROM communes) from communes where nom LIKE '%-%-%';

-- 10. Quelles sont les communes (tous les attributs) qui ne sont pas
-- des chefs-lieux (de département ou de région) ?

SELECT c.com, c.dep, c.nom from communes c
join departements d on d.cheflieu != c.com 
join regions r on r.cheflieu != c.com ;



-- 11. Quels sont les codes et noms des communes dont le chef-lieu
-- départemental et le chef-lieu régional ne sont pas les mêmes ?

SELECT c.nom, c.com from communes c 
join departements d on d.dep = c.dep 
join regions r on r.reg = d.reg and r.cheflieu != d.cheflieu; 

-- 12. Donner la table des communes avec comme colonnes : le nom de la
-- commune, le nom du département de cette commune, le nom du
-- chef-lieu du département, le nom de la région de cette commune, le
-- nom du chef-lieu de la région.

SELECT c.nom, d.nom, d.cheflieu, r.nom, r.cheflieu from communes c 
join departements d on c.dep = d.dep 
join regions r on d.reg = r.reg ;

-- 13. Donner, sans doublons, le nom des communes qui ne sont pas les
-- seules à avoir ce nom au sein d'une même région, dans l'ordre
-- lexicographique croissant.

-- 14. Donner les noms de chef-lieu de département avec le nom de ce
-- département et, pour ceux qui sont également chef-lieu de région,
-- le nom de cette région (et NULL) pour les autres.

-- 15. Donnez les noms des communes qui sont utilisés par au moins
-- deux communes, ainsi que le nombre de communes utilisant chacun de
-- ces noms. La table sera triée par ordre décroissant suivant le
-- nombre de communes, puis par ordre croissant des codes des
-- communes.

-- 16. Combien y a-t-il d'habitants en France (en 2016) ?

SELECT sum(pop24+ pop2564 + pop65 + naissances - deces) as population from demographie;

-- 17. Quelle est la proportion des moins de 25 ans en France, en
-- pourcentage entier.

SELECT (pop24*100/(pop24+ pop2564 + pop65 + naissances - deces)) as proportion from demographie;


-- 18. Donner, pour chaque commune, le code de la commune et le nombre
-- d'habitants qu'il y avait dans cette commune l'année précédente, en
-- supposant qu'il n'y a eu ni émigration, ni immigration.

SELECT com,(pop24+ pop2564 + pop65 + naissances - deces) as annee_precedente from demographie; 

-- 19. Quels sont, dans l'ordre lexicographique croissant, les codes
-- des communes avec strictement plus de naissances que la moyenne des
-- naissances par commune ?

SELECT com from demographie 
where naissances > ((SELECT sum(naissances) from demographie)/(SELECT count(*) from demographie)) 
ORDER by com ASC ;

-- 20. Quelles sont les communes sans aucun habitant ? La table aura
-- les mêmes colonnes que la table `communes`. On pourra utiliser
-- l'étoile préfixée par le nom ou l'alias de la table.

SELECT c.com, c.dep, c.nom from communes c
join demographie d on d.com = c.com and (d.pop24+ d.pop2564 + d.pop65 + d.naissances - d.deces) = 0

-- 21. Quelles sont les 19 communes les moins peuplées, parmi celles
-- qui comportent au moins un habitant renseigné ? On donnera le nom
-- de la commune, le nom de son département et sa population.

SELECT c.nom, de.nom,(d.pop24+ d.pop2564 + d.pop65 + d.naissances - d.deces) as population from demographie d, communes c, departements de
where population is not null and c.com = d.com and c.dep = de.dep 
order by population ASC limit 19 OFFSET 0;


-- 22. Donnez les communes (tous les attributs) ayant autant
-- d’habitants que de lettres (tirets inclus) dans leur nom, avec ce
-- nombre d'habitants.


-- 23. Quels sont les codes et noms des départements dans lesquels
-- toutes les communes ont vu au moins une naissance et un décès (si
-- cette valeur est renseignée). On considère que le département de
-- Mayotte vérifie la propriété et doit donc être présent.


-- 24. Donner la table des régions, de leur population, du nombre de
-- naissances et du nombre de décès. On souhaite avoir les mêmes
-- colonnes que la table `regions`, avec trois colonnes en plus : une
-- pour la population, une pour les naissances et une pour les décès.

-- 25. Écrire une requête renvoyant le nom de la région contenant le
-- département le plus peuplé ainsi que le nom de ce département.

-- 26. Donnez la table des communes ayant plus d’habitants que le
-- chef-lieu de leur département. La table aura pour colonnes : le nom
-- de la commune, sa population, le nom du département, le nom du
-- chef-lieu et la population de son chef-lieu.

-- 27. Donnez la table des régions en y ajoutant le pourcentage entier
-- de la population de la région habitant dans le chef-lieu de région.

-- 28. Combien y a-t-il de boulangeries en France ?

SELECT SUM(boulangeries) as boulangeries_en_france from equipements;

-- 29. Combien y a-t-il, en moyenne, de pharmacies par commune ?

SELECT AVG(pharmacies) from equipements;

-- 30. Dans combien de communes y-a-t-il strictement plus de
-- poissonneries que de crèches ?

SELECT count(*) from equipements 
where pharmacies > creches;

-- 31. Donner le nombre de lycées par département. On donnera le nom
-- et le nombre de lycées de chaque département, ordonnés par nombre
-- de lycées décroissant puis par nom de département croissant pour
-- l'ordre lexicographique.

SELECT sum(d.lycees) from demographie d
join communes c on c.com = d.com
group by c.dep ASC;

-- 32. Quel est le nom de la commune avec le plus faible nombre
-- d'habitants (connu) par boulangerie ? On donnera le nom de la
-- commune avec le nombre d'habitants par boulangerie (partie
-- entière).

-- 33. Quels sont les départements dans lesquels toutes les communes
-- sont dotées d'au moins une pharmacie ?

-- 34. Quel est le nom de la région ayant le plus de poissonneries par
-- habitant de plus de 65 ans ?

-- 35. Quelles sont les régions dont tous les départements ont au
--  moins un cinquième de leurs communes qui ont au moins une crèche
--  pour 30 naissances ?

-- 36. Donner la table des régions avec en plus, pour chaque région,
-- le nom du département contenant le plus de boulangeries, et la
-- proportion (en pourcentage entier) de boulangeries de la région qui
-- sont dans ce département.
