🧭 Pathfinding en C (BFS)

📌 Description

Ce projet est une implémentation d’un algorithme de pathfinding en C utilisant le parcours en largeur (Breadth-First Search – BFS) afin de déterminer le plus court chemin entre deux nœuds dans un graphe non pondéré.

Le programme :

- Lit un fichier de configuration
- Construit dynamiquement un graphe en mémoire
- Identifie le nœud de départ et le nœud d’arrivée
- Calcule le plus court chemin
- Affiche le résultat dans le terminal

--------------------------------------------------------------------------------------------

🧠 Algorithme utilisé

Le projet utilise l’algorithme Breadth-First Search (BFS) :

- Exploration niveau par niveau
- Garantie du plus court chemin dans un graphe non pondéré
- Utilisation d’une file (queue)
- Reconstruction du chemin via un tableau parent[]

--------------------------------------------------------------------------------------------

📂 Format du fichier d’entrée

Le programme attend un fichier structuré avec les sections suivantes :

///////////////////////////////////////////////

#nodes

A

B

C

D


#start

A


#end

D


#links

A-B

B-C

C-D

///////////////////////////////////////////////


🔹 Sections

#nodes → liste des nœuds

#start → nœud de départ

#end → nœud d’arrivée

#links → connexions bidirectionnelles sous la forme A-B

--------------------------------------------------------------------------------------------

⚙️ Compilation

gcc -Wall -Wextra -Werror main.c library.c -o pathfinding


(Adapter selon l’organisation de vos fichiers.)

--------------------------------------------------------------------------------------------

▶️ Exécution

./pathfinding fichier.txt

--------------------------------------------------------------------------------------------

📤 Exemple de sortie

pathfinding:

A B C D


Si aucun chemin n’est trouvé :

No path

--------------------------------------------------------------------------------------------


🏗️ Architecture du projet


🔹 Construction du graphe

- Allocation dynamique des nœuds
- Stockage des connexions via listes d’adjacence
- Gestion mémoire complète (malloc, realloc, free)


🔹 Fonctions principales

init_node() → Lecture et création du graphe

add_node() → Ajout dynamique d’un nœud

add_links_from_buffer() → Création des connexions

shortest_path() → Calcul du plus court chemin

bfs_while() → Étape principale du BFS

free_nodes() → Libération mémoire

--------------------------------------------------------------------------------------------

🧹 Gestion mémoire

Le projet :

- Alloue dynamiquement les nœuds
- Alloue dynamiquement les tableaux de liens
- Libère proprement toute la mémoire à la fin
- Compatible avec valgrind.

--------------------------------------------------------------------------------------------  

🚀 Points techniques

- Manipulation avancée de pointeurs triples (Node ***)
- Reallocation dynamique progressive
- Parsing manuel de fichier
- Reconstruction de chemin via tableau parent[]
- Gestion d’erreurs personnalisée

--------------------------------------------------------------------------------------------

📚 Concepts abordés

- Structures en C
- Listes d’adjacence
- Allocation dynamique
- Lecture de fichier
- Algorithme BFS
- Gestion d’erreurs
- Manipulation de chaînes (trim, strcmp, sscanf)

--------------------------------------------------------------------------------------------

👨‍💻 Auteur

Projet réalisé dans le cadre d’un apprentissage du langage C et des algorithmes de graphes.
