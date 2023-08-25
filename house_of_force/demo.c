/*
# An initial heap malloc discovering experiment inspired by the above post
##      "Heaps of Fun with glibc malloc"
By :            Rayden Chia
Post date :     July 6, 2016
Post url :      https://raydenchia.com/heaps-of-fun-with-glibc-malloc/
File : demo.c
## how-to-compile:
     gcc -w -g -o demo demo.c
## then to debug with gdb:
    gdb demo
    and break main or b main
    then run or just r followed by next or just n
    vmmap
    vis_heap_chunk or vis to see heap
    print p9 or p p9 to see addr of var p9
*/
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

int main(int argc, char* argv[]) {

        // Get current brk ptr with sbrk(0)
        printf("🌟Before calling brk,        end of heap is at %p\n", sbrk(0));
        void *p9 = malloc(9);
        printf("🌟After *p9 = malloc(9),     end of heap is at %p\n\n", sbrk(0));

        void *p0 = malloc(0);
        printf("🌟After *p0 = malloc(0),     end of heap is at %p\n\n", sbrk(0));

        void *p1 = malloc(1);
        printf("🌟After *p1 = malloc(1),     end of heap is at %p\n\n", sbrk(0));

        void *p24 = malloc(24);
        printf("🌟After *p24 = malloc(24),   end of heap is at %p\n\n", sbrk(0));

}