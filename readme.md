Pour compiler :
	- client  :
		make client
	- serveur :
		make server

Pour exécuter :
	- client  (si le serveur est sur votre machine) :
		cd bin
		./client
	- client (si le serveur n'est pas sur votre machine) :
		./client adresse_ip_serveur
	- serveur :
		cd bin
		./server

-------------------------------------------------------------

<p>Vous allez jouer à un jeu de carte.</p>
<p>Lorsque vous êtes dans la parties (donc ni en attente de fin de tour, ni en attente de joueur), vous pourrez piocher à n'importe quel moment.</p>
<p>Vous pouvez avoir jusqu'à 10 cartes</p>
<p>Le tour est terminé lorsque tous les joueurs ont joué, alors, vous découvrirez le nombre de points que vous et les autres joueurs avez gagné</p>
<p>La valeur d'une carte est le nombre de points qu'elle va vous faire gagner.</p>
<p>Cependant, faites attention avant de piocher une carte, car le calcul se fait de la façon suivante : (valeur_carte - nb_de_carte_en_main)</p>
<p>Les différentes commandes comme piocher une carte, poser une carte, ..., vous serons indiquer dans le terminal</p>
