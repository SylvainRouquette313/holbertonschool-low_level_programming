Cartes mémoire

Programmes analysés : stack_example.c, aliasing_example.c (obligatoires), heap_example.c (optionnel).

Méthode
Compilation avec les options du Makefile : -Wall -Wextra -Werror -pedantic -std=gnu89 -g.
Exécution de chaque programme normalement, puis avec valgrind --leak-check=full.
J'ai demandé à une IA des cartes mémoire étape par étape, puis j'ai vérifié chaque affirmation avec la sortie du programme, gdb et Valgrind. Les affirmations fausses sont listées à la fin de chaque partie.

Rappels :

Pile (stack) : variables locales et paramètres. Elles n'existent que pendant l'exécution de leur fonction. La pile grandit vers les adresses plus petites.
Tas (heap) : mémoire obtenue avec malloc. Elle existe jusqu'au free, quoi qu'il arrive aux variables pointeurs.
Zone statique : les chaînes littérales comme "Alice". Elles existent pendant tout le programme.
Les adresses ci-dessous viennent d'une exécution sans ASLR. Les tiennes seront différentes, mais les écarts entre les adresses restent les mêmes.
1. stack_example.c
Où se trouve chaque chose
Pile : dans walk_stack, les paramètres depth et max_depth et la variable locale marker. Dans dump_frame, les paramètres label et depth et les variables locales local_int, local_buf[16] et p_local.
Zone statique : les chaînes "enter", "exit" et les formats de printf. label lui-même est sur la pile, mais il pointe vers la zone statique.
Tas : notre code n'alloue rien. Valgrind affiche quand même 1 allocs, 1 frees, 4,096 bytes : c'est le tampon que la glibc crée pour stdout au premier printf. Il est libéré à la fin, donc pas de fuite.
Alias

Le seul alias est p_local = &local_int. Les deux noms désignent les mêmes 4 octets. Il ne sort jamais de dump_frame, donc il ne devient jamais pendant.

Étape par étape
main appelle walk_stack(0, 3). Un cadre est créé avec marker = 0.
walk_stack(0) appelle dump_frame("enter", 0). Un nouveau cadre est créé en dessous : local_int = 100, p_local pointe vers local_int, local_buf = "A". local_buf[2..15] ne sont jamais écrites, mais jamais lues non plus, donc ce n'est pas un bug.
dump_frame retourne. Son cadre est mort : ses variables n'existent plus, même si les octets sont encore en mémoire.
walk_stack(0) appelle walk_stack(1), qui appelle walk_stack(2), puis walk_stack(3). Chaque appel a ses propres depth et marker (0, 10, 20, 30). À la profondeur 3, depth < max_depth est faux, donc la récursion s'arrête. Au point le plus profond, 6 cadres sont vivants : main, 4 × walk_stack et 1 × dump_frame.
Remontée : chaque walk_stack(d) appelle dump_frame("exit", d), puis retourne.
main retourne. Tous les cadres ont disparu.

Faits mesurés :

marker descend de 48 octets (0x30) à chaque niveau : c'est la taille d'un cadre de walk_stack (objdump : sub $0x20,%rsp, plus l'adresse de retour et le %rbp sauvegardé). Un cadre de dump_frame fait 80 octets.
[exit] affiche les mêmes adresses que [enter] pour une même profondeur.
Là où l'IA s'est trompée

1. « L'appel exit retrouve les mêmes variables, donc leurs valeurs ont été conservées. » Faux. Même adresse ne veut pas dire même variable. Le cadre de enter est mort quand la fonction a retourné. Ensuite, walk_stack(d+1) a été construit sur les mêmes octets et les a écrasés (walk_stack(1) démarre au même %rbp que l'ancien dump_frame(…,0), vérifié dans gdb). L'appel exit obtient un nouveau cadre au même endroit parce que le pointeur de pile est revenu à la même valeur. Les valeurs sont identiques seulement parce que dump_frame les recalcule à partir de depth.

2. Mauvais décalage pour marker (erreur de mon assistant IA, Claude, pendant cette analyse). En lisant le désassemblage, il a placé marker à rbp-0x14. Cette case contient en réalité le paramètre depth (mov %edi,-0x14(%rbp), où %edi = 1er argument). Avec cette erreur, walk_stack(1) et dump_frame avaient des %rbp différents, ce qui est impossible puisque les deux sont appelés depuis le même endroit. gdb (p &marker) a montré le vrai décalage : rbp-0xc.

3. « Les variables sont placées dans l'ordre de déclaration. » Faux. local_buf est à une adresse plus haute que local_int, alors qu'il est déclaré après. C'est le compilateur qui décide de l'ordre.

4. « Le tas n'est pas utilisé. » Vrai seulement pour notre code : la glibc alloue le tampon de stdout (voir Valgrind plus haut).

2. aliasing_example.c
Problème de compilation

Avec GCC 12 ou plus récent, le Makefile ne compile pas ce programme : error: pointer 'b' used after 'free' [-Werror=use-after-free] aux lignes 40, 42, 44 et 45. GCC voit que b = a et détecte l'utilisation après free(a). Pour le lancer, j'ai ajouté -Wno-error=use-after-free (l'avertissement reste affiché).

Où se trouve chaque chose
Pile : a, b, n dans main ; n, i, arr dans make_numbers.
Tas : un tableau de 5 int (20 octets), alloué ligne 12, libéré ligne 38.
Propriété et alias
make_numbers alloue le tableau et donne la propriété à main en le renvoyant. arr disparaît avec la fonction ; le tableau reste.
a est le propriétaire. b = a copie seulement l'adresse, pas le tableau : b est un alias qui ne possède rien.
Après free(a), a et b contiennent toujours la même adresse, et les deux sont pendants. free ne peut pas modifier b, et ne met pas non plus a à NULL.
Étape par étape
a = make_numbers(5). Tas : [0, 11, 22, 33, 44].
b = a. Même adresse (0x…a2b0), donc a[2] et b[2] sont le même int : 22.
free(a) (ligne 38). Le bloc est rendu à la glibc. La glibc écrit tout de suite ses propres informations dans les 16 premiers octets (un pointeur de liste et une « clé » de sécurité). Vu dans gdb :
avant free : 00000000 0000000b 00000016 00000021 0000002c
après free : 5555555a 00000005 424284fc ab3c3ce1 0000002c
La ligne 40 affiche b (l'adresse). Rien n'est lu à travers b, donc Valgrind ne dit rien.
La ligne 42 lit b[2] : lecture invalide. Une exécution normale affiche un nombre aléatoire (une partie de la clé de la glibc), pas 22.
La ligne 44 écrit b[3] = 1234 : écriture invalide. Elle écrase la clé de la glibc. Pas de plantage, mais les données de l'allocateur sont corrompues.
La ligne 45 lit b[3] : encore une lecture invalide (elle affiche 1234).

Valgrind confirme exactement cela :

Invalid read of size 4 at line 42 : « 8 bytes inside a block of size 20 free'd ». 8 octets = indice 2 × 4 octets.
Invalid write of size 4 at line 44 : « 12 bytes inside » = indice 3.
Invalid read of size 4 at line 45 : même adresse.
Chaque erreur indique aussi où le bloc a été libéré (ligne 38) et alloué (ligne 12).
« All heap blocks were freed » : pas de fuite. Le bug est un use-after-free.

Correction : ne pas utiliser le tableau après free, mettre a et b à NULL, et ne libérer qu'une seule fois.

Là où l'IA s'est trompée

5. « Après free, b[2] contient toujours 22 car free n'efface pas la mémoire. » Sous Valgrind, le programme affiche bien 22, mais seulement parce que Valgrind utilise son propre allocateur. En exécution normale, la glibc a écrasé ces octets et le programme a affiché -1829332531. Une lecture après free donne une valeur indéfinie.

6. « b = a copie le tableau. » Non, seulement l'adresse. Un seul bloc, deux noms.

7. « Ce programme a une fuite mémoire. » Non. Valgrind indique 0 fuite. Les 3 erreurs sont des lectures et écritures invalides dans de la mémoire libérée.

3. heap_example.c (optionnel)
Où se trouve chaque chose
Pile : alice, bob dans main ; p, len, i dans person_new.
Tas : 4 blocs : 2 structures Person (16 octets chacune) et 2 noms ("Alice\0" = 6 octets, "Bob\0" = 4 octets). person_new copie le nom dans le tas.
Propriété : alice possède la structure, et la structure possède son name (par le champ name).
Étape par étape
alice = person_new("Alice", 30) : structure à 0x…a2b0, son nom à 0x…a2d0.
bob = person_new("Bob", 41) : structure à 0x…a2f0, son nom à 0x…a310.
free(bob->name); free(bob); : correct, d'abord le nom, puis la structure.
person_free_partial(alice) libère seulement la structure. Le seul pointeur vers "Alice\0" était dans cette structure : plus personne ne peut atteindre ces 6 octets. C'est la fuite.

Valgrind : 6 bytes in 1 blocks are definitely lost … person_new (heap_example.c:21) … main (heap_example.c:51). Les 6 octets sont "Alice\0".

Correction : dans person_free_partial, appeler free(p->name); avant free(p);.

Là où l'IA s'est trompée

8. « Le nom est signalé comme indirectly lost. » Non, comme definitely lost. « Indirectly lost » veut dire que le dernier pointeur se trouve dans un autre bloc perdu. Ici, la structure a été libérée, donc il ne reste aucun pointeur vers le nom nulle part.