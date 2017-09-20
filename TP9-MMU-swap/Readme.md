# ASE-MMU

Memory Management Unit

  Authors
  -----------
  
  Hamann Denis & Delperdange Nicolas

  What is it?
  -----------

  Gestionnaire de la mémoire façon MIPS et ARM avec systême de swap

  Executable oracle pour vérifier les réponses.
  Pour lancer : ./mmu_manager | tee /dev/stderr | ./oracle

  Défition de la taille de la matrice dans matrice.h (au dessus de 100, le temps d'execution commence à etre long)
  
  Files
  ------------  
  hardware.ini
  Makefile
  mmu_manager.c
  oracle.c
  swap.c
  user_process.c
  hw_config.h
  matrix.c
  Readme.md (this file)
  libhardware (folder)
  matrix.h
  mmu_manager.h
  swap.h

  Installation
  ------------
  
  Commandes : 
  - Pour installer les différents utilitaires : make
  - Pour supprimer les fichiers : 		make clean
  - Pour supprimer les fichiers optionnels : 	make realclean
  

  Documentation
  -------------
  
  Ce projet se décompose en plusieurs étapes. Pour ce rendu, nous avons
  développé la première étape.
  
  Celle-ci consiste en l'implémentation de la protection et de
  l'isolation de la mémoire.
  
  Nous avons donc développé dans cette étape un petit utilitaire qui
  simule l'exécution d'un programme en mode kernel et l'exécution d'un 
  programme en mode utilisateur.



  

  
