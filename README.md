# projet-commande-et-reseaux
Eduardo LORANDI, Pedro BASSO
## Fonctionement 

Prèmierement, les IP's sont toujours le *localhost*, alors se vous executez le VREP sur autre ordinateur, il faut changer le ip dans:
- `/robot/main.cpp`, ligne 148.


Pour complier, il faut executer les *Makefiles* dans les dossiers *server_client/* et *robot/*:

- `make` dans `robot/`
- `make` dans `server_client/`


Alors, lansez les programmes dans la ordre suivante:
- VREP: new scene -> choisir `robotis-cryon.ttt` et aussi appuyer "PLAY" dans la simulation
- `./control` dans `robot/`
- `./serverCommunicator`dans `server_client/`
- `./clientController` dans `server_client/`

Le *./clientController* ira demander pour choissir 4 positions differentes:
- Choisir 1 option
- Obserever le changement du robot sur VREP a cette position
- Observer, dans le `clientController` le temps de retard

C'est possible de verifier le fonctionement des outres partes en observant les prints sur `./control` et `./serverCommunicator`


## Some notes
- open VM:  C:/VBox/linuxmea, login: td1  pass: td1

- open VREP: at /home/VREP, type ./vrep.sh
- file-> open scene > .ttt file  and after, click on the "play" button to run the ROBOT_SERVER

ctrl shift O -> split terminal


Commande:

q[n], n= 6:

- q[0]  motor da base, girando no próprio eixo
- q[1]  motor da base, retroescavadeira  
- q[2]  motor do cotovelo, retroescavadeira 
- q[3]  maozinha, proprio eixo (tchauzinho)
- q[4]  maozinha retroesavadeira 
- q[5]  dedinho proprio eixo 
