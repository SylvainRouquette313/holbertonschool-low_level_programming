Memory Maps

Programs analyzed: stack_example.c, aliasing_example.c (mandatory), heap_example.c (optional).

How I worked
Compiled with the Makefile flags: -Wall -Wextra -Werror -pedantic -std=gnu89 -g.
Ran each program normally, then with valgrind --leak-check=full.
Asked an AI for step-by-step memory maps, then checked every claim with the program output, gdb and Valgrind. Wrong claims are listed at the end of each section.

Reminders:

Stack: local variables and parameters. They exist only while their function is running. The stack grows toward lower addresses.
Heap: memory from malloc. It exists until free, no matter what happens to the pointer variables.
Static storage: string literals like "Alice". They exist for the whole program.
Addresses below come from one run with ASLR off. Yours will differ, but the distances between addresses stay the same.
1. stack_example.c
What lives where
Stack: in walk_stack, the parameters depth and max_depth and the local marker. In dump_frame, the parameters label and depth and the locals local_int, local_buf[16] and p_local.
Static storage: the strings "enter", "exit" and the printf formats. label itself is on the stack, but it points to static storage.
Heap: our code allocates nothing. Valgrind still shows 1 allocs, 1 frees, 4,096 bytes: this is the buffer glibc creates for stdout at the first printf. It is freed at exit, so there is no leak.
Aliasing

The only alias is p_local = &local_int. Both names refer to the same 4 bytes. It never leaves dump_frame, so it never dangles.

Step by step
main calls walk_stack(0, 3). A frame is created with marker = 0.
walk_stack(0) calls dump_frame("enter", 0). A new frame is created below: local_int = 100, p_local points to local_int, local_buf = "A". local_buf[2..15] are never written, but never read either, so this is not a bug.
dump_frame returns. Its frame is dead: its variables no longer exist, even if the bytes are still in memory.
walk_stack(0) calls walk_stack(1), which calls walk_stack(2), then walk_stack(3). Each call has its own depth and marker (0, 10, 20, 30). At depth 3, depth < max_depth is false, so the recursion stops. At the deepest point there are 6 live frames: main, 4 × walk_stack and 1 × dump_frame.
Unwinding: each walk_stack(d) calls dump_frame("exit", d), then returns.
main returns. All frames are gone.

Measured facts:

marker moves down by 48 bytes (0x30) at each level: this is the size of one walk_stack frame (objdump: sub $0x20,%rsp, plus the return address and the saved %rbp). A dump_frame frame is 80 bytes.
[exit] prints the same addresses as [enter] for the same depth.
Where the AI was wrong

1. "The exit call finds the same variables, so their values were preserved." Wrong. The same address does not mean the same variable. The enter frame died when it returned. Then walk_stack(d+1) was built on the same bytes and overwrote them (walk_stack(1) starts at the same %rbp as the old dump_frame(…,0), checked in gdb). The exit call gets a new frame at the same place because the stack pointer is back to the same value. The values match only because dump_frame recomputes them from depth.

2. Wrong offset for marker (this was my AI assistant, Claude, during this analysis). Reading the disassembly, it placed marker at rbp-0x14. That slot actually holds the depth parameter (mov %edi,-0x14(%rbp), where %edi = 1st argument). With that mistake, walk_stack(1) and dump_frame had different %rbp values, which is impossible because both are called from the same place. gdb (p &marker) showed the real offset: rbp-0xc.

3. "Variables are placed in declaration order." Wrong. local_buf is at a higher address than local_int even though it is declared after it. The compiler decides the order.

4. "No heap is used." Only true for our code: glibc allocates the stdout buffer (see Valgrind above).

2. aliasing_example.c
Build problem

With GCC 12 or later, the Makefile does not build this program: error: pointer 'b' used after 'free' [-Werror=use-after-free] on lines 40, 42, 44 and 45. GCC sees that b = a and detects the use after free(a). To run it, I added -Wno-error=use-after-free (the warning is still shown).

What lives where
Stack: a, b, n in main; n, i, arr in make_numbers.
Heap: one array of 5 int (20 bytes), allocated at line 12, freed at line 38.
Ownership and aliasing
make_numbers allocates the array and gives ownership to main by returning it. arr dies with the function; the array stays.
a is the owner. b = a copies only the address, not the array: b is an alias that does not own anything.
After free(a), a and b both still hold the same address, and both are dangling. free cannot change b, and it does not set a to NULL either.
Step by step
a = make_numbers(5). Heap: [0, 11, 22, 33, 44].
b = a. Same address (0x…a2b0), so a[2] and b[2] are the same int: 22.
free(a) (line 38). The block goes back to glibc. glibc immediately writes its own bookkeeping into the first 16 bytes (a list pointer and a security "key"). Seen in gdb:
before free: 00000000 0000000b 00000016 00000021 0000002c
after free:  5555555a 00000005 424284fc ab3c3ce1 0000002c
Line 40 prints b (the address). Nothing is read through b, so Valgrind says nothing.
Line 42 reads b[2]: invalid read. A normal run prints a random number (part of glibc's key), not 22.
Line 44 writes b[3] = 1234: invalid write. It overwrites glibc's key. There is no crash, but the allocator's data is corrupted.
Line 45 reads b[3]: invalid read again (it shows 1234).

Valgrind confirms exactly this:

Invalid read of size 4 at line 42: "8 bytes inside a block of size 20 free'd". 8 bytes = index 2 × 4 bytes.
Invalid write of size 4 at line 44: "12 bytes inside" = index 3.
Invalid read of size 4 at line 45: same address.
Each error also says where the block was freed (line 38) and allocated (line 12).
"All heap blocks were freed": no leak. The bug is a use-after-free.

Fix: do not use the array after free, set a and b to NULL, and free only once.

Where the AI was wrong

5. "After free, b[2] still contains 22 because free does not erase memory." Under Valgrind it does print 22, but only because Valgrind uses its own allocator. In a normal run, glibc overwrote the bytes and the program printed -1829332531. A read after free gives an undefined value.

6. "b = a copies the array." No, only the address. A single block, two names.

7. "This program leaks memory." No. Valgrind reports 0 leaks. The 3 errors are invalid reads/writes in freed memory.

3. heap_example.c (optional)
What lives where
Stack: alice, bob in main; p, len, i in person_new.
Heap: 4 blocks: 2 Person structs (16 bytes each) and 2 names ("Alice\0" = 6 bytes, "Bob\0" = 4 bytes). person_new copies the name to the heap.
Ownership: alice owns the struct, and the struct owns its name (through the name field).
Step by step
alice = person_new("Alice", 30): struct at 0x…a2b0, its name at 0x…a2d0.
bob = person_new("Bob", 41): struct at 0x…a2f0, its name at 0x…a310.
free(bob->name); free(bob);: correct, name first, then the struct.
person_free_partial(alice) frees only the struct. The only pointer to "Alice\0" was inside that struct, so now nobody can reach those 6 bytes. This is the leak.

Valgrind: 6 bytes in 1 blocks are definitely lost … person_new (heap_example.c:21) … main (heap_example.c:51). The 6 bytes are "Alice\0".

Fix: in person_free_partial, call free(p->name); before free(p);.

Where the AI was wrong

8. "The name is reported as indirectly lost." No, as definitely lost. "Indirectly lost" means the last pointer is inside another leaked block. Here the struct was freed, so no pointer to the name remains anywhere.