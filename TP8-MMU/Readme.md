# ASE-MMU

Memory Management Unit

  Authors
  -----------
  
  Hamann Denis & Delperdange Nicolas

  What is it?
  -----------

  Gestionnaire de la mémoire façon MIPS et ARM
  
  Files
  ------------  
  
  libhardware (dossier)
  hardware.ini
  hw_config.h
  mi_syscall.h
  mi_kernel.h
  mi_kernel.c
  mi_user.h
  mi_user.c
  Readme.md (this file)
  Makefile


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



  

  
