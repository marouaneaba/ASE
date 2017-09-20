# ASE-Fichiers

Gestionnaire de fichiers

  Authors
  -----------
  
  Hamann Denis & Delperdange Nicolas

  What is it?
  -----------

  Gestionnaire de fichiers façon Unix
  
  Files
  ------------  
  
  constantes.h
  dmpc.c
  drive.c
  drive.h
  Makefile
  mbr.c
  mbr.h
  Readme (this file)
  test_convert.c
  vm.c
  vol.c
  vol.h


  Installation
  ------------
  
  Commandes : 
  - Pour installer les différents utilitaires : make
  - Pour supprimer les fichiers : 		make clean
  - Pour supprimer les fichiers optionnels : 	make realclean
  

  Documentation
  -------------
  
  Ce projet se décompose en plusieurs étapes.
  La prémière étape étant de créer une couche logicielle d'accès au matériel.
  Celle-ci se traduit par le fichier exécutable dmps qui permet d'afficher un secteur, de tous
  les afficher ou de formater le disque.
  L'affichage est disponible en ascii ou en octal.
  
  
  La deuxième étape est la gestion des volumes.
  Celle-ci se traduit par l'exécutable vm qui par une petite interface utilisateur permet de gérer
  les différents volumes présents sur le disque virtuel (créer , lister et supprimer).


  

  

