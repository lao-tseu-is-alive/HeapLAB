/*
##      "Heaps of Fun with glibc malloc"
By :            Rayden Chia
Post date :     July 6, 2016
Post url :      https://raydenchia.com/heaps-of-fun-with-glibc-malloc/
File : malloc_syscall_demo.c
## how-to-compile:
     gcc -w -o malloc_syscall_demo malloc_syscall_demo.c
*/
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>



void print_map()
{
	char sz_exec_cmd[64];
	sprintf(&sz_exec_cmd, "cat /proc/%d/maps\0", getpid());
	printf("Output of /proc/%d/maps:\n", getpid());
	system(sz_exec_cmd);
	printf("\n");
	return;
}

int main()
{
	    printf("Process started with pid %d\n", getpid());

        // Get current brk ptr with sbrk(0)
        printf("Before calling brk, end of heap is at %p\n", sbrk(0));

        // Increment brk ptr by 1 page size (4K)
        brk(sbrk(0) + 4096); // Equiv to sbrk(4096);
        printf("After +4096, end of heap is at %p\n", sbrk(0));

        sbrk(-4096); // Equiv to brk(sbrk(0)-4096);
        printf("After -4096, end of heap is at %p\n\n", sbrk(0));
        print_map();

        void *p1 = malloc(120*1024);
        printf("Invoked malloc(120*1024).\n");
        print_map();

        free(p1);
        void *p2 = malloc(140*1024);
        printf("Invoked malloc(140*1024).\n");
        print_map();
        free(p2);

        return 0;
}