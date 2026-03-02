# 🔍 Pathfinding en C — BFS sur graphe

Un programme en C qui lit un graphe depuis un fichier texte, affiche ses informations et trouve le chemin le plus court entre deux nœuds via un algorithme **BFS (Breadth-First Search)**.

---

## 📋 Présentation

Ce projet implémente un **moteur de graphes** en C. Il parse un fichier de configuration décrivant des nœuds et des liens, construit le graphe en mémoire, puis recherche le chemin le plus court entre un nœud de départ et un nœud d'arrivée.

---

## ✨ Fonctionnalités

- 📂 **Parsing de fichier** : lecture d'un fichier `.txt` structuré en sections
- 🔗 **Construction du graphe** : ajout dynamique de nœuds et de liens bidirectionnels
- 🚦 **Nœuds de départ / arrivée** : marquage automatique depuis le fichier
- 🔍 **BFS** : recherche du chemin le plus court entre deux nœuds
- 🏝️ **Nœuds isolés** : détection des nœuds sans aucun lien
- 🧹 **Gestion mémoire** : libération complète des ressources allouées

---

## 🗂️ Structure du projet

```
projet/
├── main.c          # Point d'entrée, affichage des résultats
├── library.c       # Toute la logique (parsing, graphe, BFS)
├── library.h       # Structures, enum d'erreurs, prototypes
├── file.txt        # Exemple de fichier de graphe
└── Makefile        # Compilation du projet
```

---

## 📄 Format du fichier d'entrée

Le fichier `.txt` est divisé en sections identifiées par un mot-clé précédé de `#`.

```
#nodes
3
4
2
5
#start
9
6
#end
7
#links
3-4
4-2
2-9
9-6
6-7
```

| Section   | Description                                          |
|-----------|------------------------------------------------------|
| `#nodes`  | Liste des nœuds du graphe                            |
| `#start`  | Premier nœud listé = nœud de départ                 |
| `#end`    | Premier nœud listé = nœud d'arrivée                 |
| `#links`  | Liens entre nœuds au format `A-B` (bidirectionnel)  |

---

## 🏗️ Architecture du code

### Structures (`library.h`)

```c
typedef struct n {
    char name[64];
    struct n **links;
    int nb_links;
    int is_start;
    int is_end;
} Node;
```

### Codes d'erreur

| Code | Valeur | Description              |
|------|--------|--------------------------|
| `FILE_NOT_FOUND`   | 1 | Fichier introuvable      |
| `NO_START_NODE`    | 2 | Section `#start` absente |
| `NO_END_NODE`      | 3 | Section `#end` absente   |
| `BAD_FILE_FORMAT`  | 4 | Format de fichier invalide |

### Fonctions principales (`library.c`)

| Fonction              | Rôle                                              |
|-----------------------|---------------------------------------------------|
| `init_node()`         | Parse le fichier et construit le graphe           |
| `shortest_path()`     | Lance le BFS et affiche le chemin trouvé          |
| `bfs_while()`         | Itération interne du BFS                          |
| `get_unconnected_node()` | Retourne les nœuds sans liens                 |
| `add_node()`          | Ajoute un nœud s'il n'existe pas encore           |
| `add_links_from_buffer()` | Crée un lien bidirectionnel entre deux nœuds |
| `free_nodes()`        | Libère toute la mémoire allouée                   |

---

## ⚙️ Compilation

### Prérequis

- GCC
- Make

### Avec le Makefile

```bash
make        # Compile le projet -> exécutable "main"
make clean  # Supprime l'exécutable
make re     # Recompile depuis zéro
```

### Manuellement

```bash
gcc -Wall -g -lm main.c library.c -o main
```

---

## 🚀 Utilisation

```bash
./main
```

> Le fichier d'entrée est défini dans `main.c` via la variable `filename`. Modifiez-la pour pointer vers votre propre fichier.

### Exemple de sortie

Avec le fichier `file.txt` fourni :

```
nodes: 7
links: 5
start: 9
end: 7
unconnected nodes :
3

pathfinding:
9 6 7
```

---

## 🛠️ Technologies utilisées

| Élément       | Détail                          |
|---------------|---------------------------------|
| Langage       | C (C99)                         |
| Compilation   | GCC avec `-Wall -g -lm`         |
| Algorithme    | BFS (Breadth-First Search)      |
| Mémoire       | Allocation dynamique (`malloc`, `realloc`) |

---

## 👨‍💻 Auteur

Projet réalisé dans le cadre d'un cours algorithmique / structures de données en C.

Achille Coves.
