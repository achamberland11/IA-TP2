# Plan de travail TP2 - IA et Génération Procédurale
## Contexte
Combiner les techinques de génération de comportements FSM, Steering et Pathfinding. L'objectif est d'implémenter un agent 
autonome capable de se déplacer de façon fluide dans un environnement 2D top-down avec obstacles et qui change de 
comportement selon la situation.

---

## Objectifs
- Comprendre et appliquer les concepts de FSM, de steering et de pathfinding.
- Examiner des algorithmes déterminant les comportements d'agents.
- Améliorer ses compétences en programmation C++.

---

## Consignes
### Carte du monde
Le monde doit être en 2D, vu de haut (top-down). La carte doit contenir des murs et former un labyrinthe. Vous devez 
utilser une topologie qui permettra de démontrer l'integlligence de l'agent. Par exemple, vous pouvez inclure des murs 
en U, des couloirs en S ou des salles avec des sorties étroites. Des obstacles supplémentaires (carrés ou circulasires)
et des murs doivent également apparaître.

### Agent
Un agent, représenté par un cercle ou un sprite, doit patrouiller le monde. Son circuit doit contenir une dizaine de points
de patrouille. Son champ de vision est déterminé par un cône (ajustable dans le code) visible dans le jeu. La "partie" 
se termine si l'agent attrape l'intrus.

### Intrus
Un intrus, représenté par un cercle ou un sprite, doit pouvoir se déplacer dans le monde. Lintrus est contrôlé par le 
joueur à l'aide des touches WASD (avance, recule, pivot anti-horaire/horaire). L'intrus se déplace plus rapidement que 
l'agent.

### Comportement attendu de l'agent
Le comportement de l'agent doit être géré par une FSM qui contient 3 états : patrouille, poursuite et retour.

#### Patrouille
L'agent suit une liste de points de patrouille. Il utilise le pathfinding pour aller ver le prochain point et le steering
pour suivre le chemin (seek / arrive / follow path). Si l'intrus entre dans le champ de vision → poursuite.
#### Poursuite
L'agent se dirige vers la position de l'intrus. Le mouvement est fluide grâce au steering (pursuit). Si l'intrus sort du
champ de vision → retour.
#### Retour
L'agent retourne au dernier point de patrouille en utilisant le pathfinding et le steering. Une fois arrivé → patrouille.

Le nom de l'état courtant de la FSM doit être affiché en tout temps dans le HUD du jeu. Dans tous les états, l'agent doit
éviter les bostacles et ne pas passer au travers des murs. Un peu de wander peut rendre le déplacement de l'agent plus 
crédible.

### Architecture en couche
Le projet doit utiliser une architecture en couches.

**FSM** : Gère le comportement global. Décide quoi faire.

**Pathfinding** : Calcule où passer. Produit une suite de points.

**Steering** : Gère le déplacement. Décide comment se déplacer. Assure un mouvement fluide (seek, arrive, follow path, 
pursuit, etc.)

### Framework
Vous pouvez utiliser les classes de la STL. Vous ne pouvez pas utiliser une librairie externe pour la FSM, le pathfinding
ou le Steering. Vous pouvez coder ces parties vous-même ou réutiliser le code fourni dans les exemples de Mat Buckland
(https://github.com/HEP85/game-ai). Vous pouvez également utiliser le code des Graphes du livre de Goodrich (Data Structures
and Algorithms in C++). Vous pouvez vous servir de l'IAG pour les algorithmes standards uniquement (A*, collisions, à
intersections, etc.), mais pas pour l'architecture de votre projet.

### Pathfinding
Le pathfinding doit utiliser A*. Vous êtes libre par rapport au choix de l'algorithme de création de graphe.

---

## Livrables:
### Code source
Fichier compressé (.zip) contenant les fichiers sources (.h et .cpp) de votre solution. Ne pas inclure les autres fichiers
de la solution de Visual Studio (ou de tout autre IDE utilisé pour le projet).

### Rapport technique
Dans un rapport écrit (.pdf) de 2 à 3 pages remis sur Moodle, vous devez décrire les structures de données et algorithmes
utilisés. Vous devez aussi justifier les choix techniques effectués. Finalement, vous devez préciser comment le travail 
a été réparti entre les membres de l’équipe. 

### Démonstration 
Vous devez démontrer dans une vidéo (enregistrement de l’écran + narration) que les fonctionnalités demandées (permettre
d’ajouter, d’exécuter et de visualiser les actions) fonctionnent. L’interface peut être simple mais doit être 
fonctionnelle. Vous pouvez remettre le vidéo sur Moodle ou fournir un lien vers un site d’hébergement externe.

---

## Structure et fonctionnement
### Core
#### Classe GObject
Classe de base de tout objet du jeu.

#### Classe GEntity : GObject
Classe de base de toute entité présente dans le monde du jeu. Possède une position, une rotation et un scale (à l'aide 
du component Transform).

#### Classe GComponent : GObject
Classe de base de tout composant d'un GEntity.

---

### Game
#### Classe GGame : GObject
Classe de base de la partie. Se charge d'initialiser le monde, de créer les entités requises, de lancer la boucle de jeu.
- Initialise SFLM
- Crée le monde
- Lance la boucle de jeu (Update(), Render())

#### Classe GWorld : GObject
Classe de base du monde. Contient les entités du niveau, contient les obstacles / murs, l'agent et l'intrus.

---

### Map
#### Classe GMap : GObject
Classe de base pour la map, gère la structure du niveau et les collisions.

#### Classe GTile : GObject
Classe de base pour un tile du niveau.

#### Classe GObstacle : GObject
Classe de base pour un obstacle du niveau.

---
### Controllers
#### Classe GController : GObject 
Gère les inputs pour diriger les actions d'un Character et son cycle de vie. A besoin d'une référence à un Character. 

##### Classe GPlayerController : GController
Gère les inputs du joueur. A besoin d'une référence à un GPlayerCharacter.

##### Classe GInstrusController : GPlayerController
Classe de l'intrus controllé par le joueur. A besoin d'une référence à un GIntrusCharacter.

##### Classe GAgentController : GController
Gère les actions de l'agent (IA) comme son FSM, le pathfinding et le steering. A besoin d'une référence à un 
GAgentCharacter. Le controller (GAgentController) orchestre les différentes actions de l'agent à l'aide de son FSM.

---

### Characters
#### Classe GCharacter : GEntity 
Permet de charger un personnage de tout type dans la partie. A besoin d'une position de départ (déjà comprise dans la 
classe GEntity). Les Characters sont toutes les Entity mobiles.

##### Classe GPlayerCharacter : GCharacter
Permet de charger un character controllé par l'utilisateur.

##### Classe GIntrusCharacter : GPlayerCharacter
Permet de charger l'intrus dans la partie.

##### Classe GAgentCharacter : GCharacter
Permet de charger un agent (IA) dans la partie. Contient les information sur le champ de vision, les points de patrouille,
l'état courant (Current State) de l'agent...

---

### Components
#### Classe GTransformComponent : GComponents
Classe contenant un vector 2 pour la position (2D), un vecteur 2 pour la rotation et un vecteur 2 pour le scale. Tout
GEntity contient un GTransformComponent.

#### Classe GColliderComponent : GComponents
Classe de base des colliders.

#### Classe GVisionComponent : GComponents
Classe de base du component pour le cone de vision. Vérifie si l'intrus (ou tout autre cible) est à portée, et si il 
est dans le champ de vision, vérifie ensuite si la ligne de vue est libre.

#### Classe GStateMachineComponent : GComponents
Classe de base du component FSM. Gère les différents states de l'agent et permet de changer d'état avec ChangeState() 
et Update l'état courant.

#### Classe GPathfindingComponent : GComponents
Classe de base du component pour le pathfinding. Gère le chemin à suivre par l'agent.

#### Classe GSteeringComponent : GComponents
Classe de base du component pour le steering. Gère les différents comportements de mouvement de l'agent (seek, arrive, 
pursuit, follow path, avoid obstacles*, etc.)

---

### Colliders
#### Classe GCircleCollider : GColliderComponent
Classe de base pour un collider circulaire.

#### Classe GBoxCollider : GColliderComponent
Classe de base pour un collider rectangulaire.

#### Classe GCollisionSystem : GObject
Classe de base pour le système de collisions. Gère les collisions entre les colliders.

---

### FSM
#### Classe GState : GObject
Classe abstraite de base pour tout les states de l'FSM. Contient les fonctions virtuelles pures Enter, Update, Exit.

#### Classe GPatrolState : GState
Classe de base pour le state patrouille.

#### Classe GChaseState : GState
Classe de base pour le state poursuite.

#### Classe GReturnState : GState
Classe de base pour le state retour (retour au dernier point de patrouille).


---

### Pathfinding

#### Classe GGraph : GObject
Classe de base d'un graphe. Stocke les nœuds et les connexions. Construit le réseau de navigation.

#### Classe GGraphNode : GObject
Classe de base d'un nœud du graphe.
- Position
- Index
- Adjacents
- Parent

#### Classe GAStarPathfinding : GObject
Classe de base du pathfinding A*.

---

### HUD
