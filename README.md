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
Classe de base de tout entité présente dans le monde du jeu. Possède une position (à l'aide du component Transform).

#### Classe GComponenet : GObject
Classe de base de tout composant d'un GEntity.

---

### Game
#### Classe GGame : GObject
Classe de base de la partie. Se charge d'initialiser le monde, de créer les entités requises, de lancer la boucle de jeu.

---
### Controllers
#### Classe GController : GEntity
Gère les inputs pour diriger les actions d'un Character et son cycle de vie. À besoin d'une référence à un Character. 

##### Classe GPlayerController : GController
Gère les inputs du joueur (WASD pour les déplacements).

##### Classe GInstrusController : GPlayerController
Classe de l'intrus controllé par le joueur.

##### Classe GAgentController : GController
Gère les actions de l'agent (IA) comme son FSM, le pathfinding et le steering.

---

### Characters
#### Classe GCharacter : GEntity 
Permet de charger un personnage de tout type dans la partie. A besoin d'une position de départ (déjat comprise dans la 
classe Object), de points de vie, d'une vitesse.

##### Classe GPlayerCharacter : GCharacter
Permet de charger un joueur dans la partie.

##### Classe GAgent : GCharacter
Permet de charger un agent (IA) dans la partie.

---

### Components
#### Classe GTransformComponent : GComponents
Classe contenant un vector 2 pour la position (2D), tout GEntity contient un GTransform.

#### Classe GColliderComponent : GComponents
Classe de base des colliders.

#### Classe GVisionComponent : GComponents
Classe de base des composants cone de vision.

#### Classe 

---

### 2D Colliders
#### Classe GCircleCollider : GColliderComponent
Classe de collider circulaire.

#### Classe GBoxCollider : GColliderComponent
Classe de collider rectangulaire.
