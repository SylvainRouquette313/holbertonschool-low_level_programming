Rapport de crash — crash_example.c
1. Description du crash

Compilation avec les options du Makefile (-Wall -Wextra -Werror -pedantic -std=gnu89 -g) : le programme compile sans aucun avertissement.

Exécution :

$ ./crash_example
crash_example: deterministic NULL dereference (segmentation fault)
  requesting n=0
Segmentation fault (core dumped)
$ echo $?
139

Code de sortie 139 = 128 + 11 → le processus a été tué par le signal 11 (SIGSEGV).

Localisation avec gdb :

Program received signal SIGSEGV, Segmentation fault.
0x...528a in main () at crash_example.c:32
32          nums[0] = 42;
(gdb) p nums
$1 = (int *) 0x0
(gdb) p n
$2 = 0
(gdb) x/i $pc
=> movl   $0x2a,(%rax)        ; écrire 42 à l'adresse contenue dans rax (= 0)

Confirmation avec Valgrind :

Invalid write of size 4
   at main (crash_example.c:32)
 Address 0x0 is not stack'd, malloc'd or (recently) free'd
Process terminating with default action of signal 11 (SIGSEGV)
 Access not within mapped region at address 0x0

Accès invalide : une écriture de 4 octets (un int) à l'adresse 0x0, ligne 32 (nums[0] = 42;).

2. Cause racine

Le bug n'est pas le segfault. Le bug est que main utilise le résultat de allocate_numbers sans vérifier s'il vaut NULL.

Chaîne complète, de la cause au crash :

Ligne 25 : n = 0.
Ligne 30 : main appelle allocate_numbers(0).
Lignes 9-10 : dans allocate_numbers, la condition n <= 0 est vraie, donc la fonction fait return NULL;. malloc n'est jamais appelé (Valgrind : total heap usage: 1 allocs = seulement le tampon de stdout).
Ligne 30 : nums reçoit NULL.
Ligne 32 : nums[0] = 42; est équivalent à *(nums + 0) = 42;, donc à *NULL = 42;. Le processeur essaie d'écrire à l'adresse 0.
Matériel / noyau : l'adresse 0 n'appartient à aucune zone mappée du processus. Le processeur déclenche une faute de page, le noyau ne trouve aucune zone valide et envoie SIGSEGV.
L'action par défaut de SIGSEGV termine le processus (code 139).

La fonction allocate_numbers fait son travail correctement : elle signale l'erreur en renvoyant NULL. C'est l'appelant (main) qui ignore ce signal.

3. Pourquoi l'accès mémoire est invalide

Catégorie de comportement indéfini : déréférencement d'un pointeur nul (NULL pointer dereference). En C, appliquer * (ou []) à un pointeur nul est un comportement indéfini (C89 §6.3.3.2 ; CERT C EXP34-C : Do not dereference null pointers).

Pile, tas, ou les deux ?

La variable nums est sur la pile (variable locale de main). Elle est parfaitement valide : son cadre est vivant, sa valeur est NULL.
La mémoire visée n'est ni sur la pile, ni dans le tas. Aucun bloc n'a été alloué. L'adresse 0 n'appartient à aucune zone du programme. Valgrind le dit exactement : « not stack'd, malloc'd or (recently) free'd ».
Ce n'est donc pas un problème de durée de vie (pas de use-after-free) ni de taille (pas de débordement) : le pointeur ne désigne aucun objet.

Pourquoi le crash est déterministe (et pas aléatoire) :

n vaut toujours 0, donc allocate_numbers renvoie toujours NULL.
Sous Linux, la première page de mémoire (autour de l'adresse 0) n'est jamais mappée pour un processus utilisateur, justement pour que tout déréférencement de NULL provoque une faute immédiate.
Résultat : à chaque exécution, même ligne, même adresse, même signal. Le crash se déduit du code, sans avoir besoin de relancer le programme.

Remarque : le programme meurt avant free(nums) (ligne 36). Le tampon de stdout n'est jamais libéré, d'où still reachable: 4,096 bytes dans Valgrind. Ce n'est pas une fuite de notre code, c'est la conséquence du crash.

4. Critique des explications de l'IA

J'ai demandé à une IA les causes possibles et les corrections possibles, puis j'ai vérifié chaque point avec le code, gdb et Valgrind.

Correct :

« C'est un déréférencement de pointeur NULL à la ligne 32. » → Confirmé par gdb (nums = 0x0) et Valgrind (Address 0x0).
« Il faut vérifier la valeur de retour avant de l'utiliser. » → C'est la vraie correction.

Incorrect :

« malloc a échoué par manque de mémoire. » Faux : malloc n'est jamais appelé. La fonction s'arrête avant, à la ligne 10, à cause de n <= 0.
« malloc(0) renvoie NULL. » Faux pour deux raisons : malloc n'est pas appelé ici ; et sur glibc, malloc(0) renvoie en général un petit pointeur non nul. Le résultat de malloc(0) dépend de l'implémentation, donc on ne peut pas s'appuyer dessus.
« Le crash a lieu dans free(nums), car on libère NULL. » Faux : le programme meurt à la ligne 32, avant free. Et free(NULL) est autorisé par la norme C : il ne fait rien.
« Rien n'a été affiché, donc le crash a lieu avant les printf. » Faux. Quand la sortie va dans un fichier ou un pipe, stdout est mis en tampon : les deux premiers printf sont dans le tampon, mais le processus est tué avant de le vider. Dans un terminal, les lignes s'affichent. gdb montre que le crash est bien ligne 32.

Spéculatif / inutile :

« Il y a peut-être une corruption du tas ou un débordement de tableau. » Spéculation sans preuve : aucun bloc n'est alloué, et Valgrind ne signale qu'une seule erreur, l'écriture à l'adresse 0.
« Il suffit de mettre n = 5. » Ça fait disparaître le crash, mais ne corrige pas le bug : n'importe quelle valeur n <= 0 (ou un vrai échec de malloc) refait planter le programme. Ça cache le problème.
« Initialiser nums à NULL. » C'est déjà fait (ligne 24) et ça ne change rien : la valeur est écrasée par le retour de la fonction, qui est NULL.
5. (Optionnel) Correction proposée

Correction suggérée, non appliquée au code (la consigne demande de ne pas modifier le programme).

Vérifier le retour de allocate_numbers avant toute utilisation :

c
nums = allocate_numbers(n);
if (!nums)
{
    fprintf(stderr, "allocate_numbers failed (n=%d)\n", n);
    return 1;
}

nums[0] = 42;

Cette correction traite les deux cas où la fonction renvoie NULL : n <= 0, et un vrai échec de malloc.