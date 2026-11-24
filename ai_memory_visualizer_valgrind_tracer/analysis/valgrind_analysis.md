Analyse Valgrind
Commande
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes -s ./<programme>

aliasing_example.c a été compilé en ajoutant -Wno-error=use-after-free : avec GCC 12 ou plus récent, les options du Makefile (-Wall -Werror) bloquent la compilation, car GCC détecte déjà le bug.

Résultats
Programme	Erreurs trouvées
heap_example	1 fuite mémoire (6 octets, definitely lost)
aliasing_example	2 lectures invalides + 1 écriture invalide (use-after-free)
stack_example	aucune

Valgrind n'a signalé aucune utilisation de mémoire non initialisée.

1. heap_example : fuite mémoire
6 bytes in 1 blocks are definitely lost
   at malloc
   by person_new (heap_example.c:21)
   by main (heap_example.c:51)
Type : fuite mémoire, definitely lost (fuite par perte de propriété).
Objet : le nom d'Alice dans le tas : "Alice" + '\0' = 6 octets. La ligne 21 est p->name = malloc(len + 1), appelée pour Alice depuis la ligne 51.
Cause :
Le seul pointeur vers le nom est alice->name, rangé à l'intérieur de la structure.
person_free_partial(alice) fait seulement free(p) : elle libère la structure, pas le nom.
Quand la structure est libérée, le pointeur vers le nom disparaît avec elle. Plus personne ne peut libérer ces 6 octets.
À noter : Valgrind indique où le bloc a été créé (ligne 21), pas où se trouve l'erreur. L'erreur est dans person_free_partial, qui devrait appeler free(p->name) avant free(p).
2. aliasing_example : use-after-free

Les trois erreurs concernent le même objet : le tableau de 5 int (20 octets) alloué dans make_numbers (ligne 12) et libéré par free(a) (ligne 38). b = a (ligne 34) a copié seulement l'adresse : b pointe vers le même tableau et devient un pointeur pendant (dangling) après free(a).

Invalid read of size 4    at main (aliasing_example.c:42)
 Address ... is 8 bytes inside a block of size 20 free'd
Invalid write of size 4   at main (aliasing_example.c:44)
 Address ... is 12 bytes inside a block of size 20 free'd
Invalid read of size 4    at main (aliasing_example.c:45)
 Address ... is 12 bytes inside a block of size 20 free'd
Ligne	Code	Erreur	Pourquoi
42	b[2]	lecture invalide (use-after-free)	8 octets = indice 2, lu après free
44	b[3] = 1234	écriture invalide (use-after-free)	12 octets = indice 3, écrit après free
45	b[3]	lecture invalide (use-after-free)	même adresse, lue après free

Comment lire le message :

« size 4 » : un int a été lu ou écrit.
« 8 bytes inside a block of size 20 » : la position est dans le tableau, ce n'est donc pas un débordement.
« free'd » : le tableau avait déjà été libéré. Le problème, c'est quand la mémoire est utilisée, pas où.

Deux choses que Valgrind ne signale pas :

La ligne 40 (printf de b avec %p) : le programme affiche l'adresse, il ne lit pas la mémoire, donc Valgrind ne voit aucun accès.
Aucune fuite : « All heap blocks were freed ». Le tableau a été libéré une fois.

L'écriture de la ligne 44 est la plus dangereuse : elle modifie de la mémoire qui appartient maintenant à l'allocateur.

3. Mémoire non initialisée

Rien n'est signalé. Valgrind ne se plaint que quand une valeur non initialisée est utilisée.

Dans stack_example, local_buf[2..15] ne sont jamais initialisées, mais jamais lues non plus.
Dans aliasing_example, les mauvaises lectures sont des lectures invalides (mémoire qui ne nous appartient plus), pas de la mémoire non initialisée (mémoire qui nous appartient mais n'a jamais été écrite).
4. Là où l'IA s'est trompée

Affirmation de l'IA : « La lecture invalide de la ligne 42 veut dire que le programme lit après la fin du tableau (débordement de tampon). »

Pourquoi c'est faux : Valgrind dit « 8 bytes inside a block of size 20 free'd ». L'indice 2 est valide pour un tableau de 5 int. Un débordement s'afficherait comme « 0 bytes after a block of size 20 alloc'd ». Ici le tableau a été libéré : c'est un use-after-free, un problème de durée de vie, pas de taille.

Affirmation de l'IA : « La fuite est à la ligne 21, donc le bug est dans person_new. »

Pourquoi c'est faux : pour une fuite, Valgrind donne la ligne de l'allocation. person_new est correcte. La mémoire est perdue dans person_free_partial, qui oublie free(p->name).