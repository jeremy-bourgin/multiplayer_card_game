-------------------------------------------------------------

<p>Le but de ce projet était de découvrir les aspects de la programmation multithreadé avec des ressources partagées en utilisant le langage C.</p>
<p>Pour cela, nous avons voulu faire un prototype de jeu de cartes multijoueur. Le principe du jeu est le suivant :</p>
- un minimum de 1 carte en main, et un maximum de 10
- on peut piocher une carte lorsqu'on le souhaite (les autres joueurs sont alors avertis)
- pour jouer, il suffit de poser une carte que l'on a en main
- le tour est terminé lorsque tous les joueurs ont posé une carte
- chaque joueur gagne des points correspondant à la valeur de la carte qu'ils ont jouée
- un minimum de 3 joueurs connectés pour jouer
- on peut quitter à tout moment

Les ressources partagées sont les suivantes :
- la pile de cartes où les joueurs peuvent piocher à n'importe quel moment
- la table de jeu où les joueurs posent une carte à chaque tour
- les points cumulés par chacun des joueurs

-------------------------------------------------------------

<p>Les différentes commandes comme piocher une carte, poser une carte, ..., vous serons indiquer dans le terminal</p>

-------------------------------------------------------------

<p>Pour compiler :</p>
- client  : make client
- serveur : make server

-------------------------------------------------------------

<p>Pour exécuter le client :</p>
- cd bin
- ./client ou ./client adresse_ip_serveur

-------------------------------------------------------------

<p>Pour exécuter le serveur :</p>
- cd bin
- ./server
