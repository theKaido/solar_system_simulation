# Systeme solaire



________________________________________________________________________
Ce programme modélise un système solaire en reprenant comme base le fichier
samples/samples_demoHelper/00_deplacements_3d de la bibliothèque GL4Dummies
en utilisant la bibliothèque SDL (ttf , mixer ,image).

Ce système solaire compend huit planètes qui respectent 
leurs angles d'inclinaison et leur vitesse de rotation. 
Les distances et la taille du soleil ont été réduites 
pour tenir dans un format approprié.

De plus, ce modèle inclut une simulation d'un trou noir résultant de 
l'expansion du Soleil en fin de vie et de son effondrement gravitationnel, 
qui provoque un rétrécissement intense de sa taille.
Cette partie de la simulation permet de visualiser les effets gravitationnels 
d'un trou noir sur les planètes du système solaire simulé.
___________________________________________________________________________

## Pour Commencer

    Prerequis :
        lib GL4Dummies(https://gl4d.api8.fr/FR/)
        lib SDL2((ttf , mixer ,image).)

    Executer :
        Compiler le programme à l'aide de 'make' dans le terminal
        Executer le programme './systemesolaire'
        Laisser tourner le programme , utiliser les différents fonction présent
        dessus cité  ci-dessous.
        Pour compresser 'make dist'.


## Fonction

    Fonction présents :

        touche à appuyer :

            'a'      : active/désactive le trou noir.
            'e'      : active/désactive le mode éclair.
            'c'      : affiche un crédit mais vaut mieux pas l'utilisé car détruit les translation.
            'm'      : change la position de la caméra .
            '↑↓→←'   : déplace la caméra.
            'r'      : rénitialise la position de la caméra après déplacements.
            'w'      : active le mode rastérisation de polygones.
            'espace' : mets en pause l'animation.
            'q'      : quitte le programme.

## Sources

    Pour les textures de bases :

        "https://www.solarsystemscope.com/textures/" Pour les planètes
        "https://i.pinimg.com/originals/0e/32/be/0e32be04f23e0533c43581ccf672f8ad.jpg" Pour le trou noir
        "https://gms.gsfc.nasa.gov/vis/a030000/a030300/a030362/euvi_aia304_2012_carrington_print.jpg" Pour le soleil en fin de vie 

    Pour la musique :

        "https://www.youtube.com/watch?v=ndyyIhwojys&ab_channel=AudioLibrary%E2%80%94Musicforcontentcreators"

    Lien Git pour suivre l'évolution du projet :

        "https://code.up8.edu/DjoMTH/systemesolaire"


## Autheur

*Mathanaruban Jonny*
