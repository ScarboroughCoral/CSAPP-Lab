/*
 * mm-naive.c - The fastest, least memory-efficient malloc package.
 * 
 * In this naive approach, a block is allocated by simply incrementing
 * the brk pointer.  A block is pure payload. There are no headers or
 * footers.  Blocks are never coalesced or reused. Realloc is
 * implemented directly using mm_malloc and mm_free.
 *
 * NOTE TO STUDENTS: Replace this header comment with your own header
 * comment that gives a high level description of your solution.
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>

#include "mm.h"
#include "memlib.h"

/*********************************************************
 * NOTE TO STUDENTS: Before you do anything else, please
 * provide your team information in the following struct.
 ********************************************************/
team_t team = {
    /* Team name */
    "coral",
    /* First member's full name */
    "Scarborough Coral",
    /* First member's email address */
    "3249977074@qq.com",
    /* Second member's full name (leave blank if none) */
    "",
    /* Second member's email address (leave blank if none) */
    ""
};

/* single word (4) or double word (8) alignment */
#define ALIGNMENT 8

/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~0x7)


#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))


/* Basic constants and macors */
#define WSIZE 4 /* Word and header/footer size(bytes)*/
#define DSIZE 8 /* Double word size */
#define CHUNKSIZE (1<<12) 

#define MAX(x,y) ((x)>(y)?(x):(y))

/* Pack a size and allocated bit into a word */
#define PACK(size,alloc) ((size)|(alloc)) 

/* Read and write a word at address p */
#define GET(p) (*(unsigned int *)(p)) 
#define PUT(p,val) (*(unsigned int *)(p) = (val)) 

/* Read the size and allocated fields from address p */
#define GET_SIZE(p) (GET(p)&~0x7) 
#define GET_ALLOC(p) (GET(p)&0x1) 

/* Given block ptr bp,compute address of its header and footer */
#define HDRP(bp) ((char *)(bp)-WSIZE) 
#define FTRP(bp) ((char *)(bp)+GET_SIZE(HDRP(bp))-DSIZE) 

/* Given block ptr bp,compute address of next and previous blocks */
#define NEXT_BLKP(bp) ((char *)(bp) + GET_SIZE(((char *)(bp) - WSIZE))) 
#define PREV_BLKP(bp) ((char *)(bp) - GET_SIZE(((char *)(bp) - DSIZE))) 

static char *heap_listp;
/* 
 * mm_init - initialize the malloc package.
 */
int mm_init(void)
{
    if ((heap_listp = mem_sbrk(4*WSIZE))==(void*)-1)
    {
        return -1;
    }
    PUT(heap_listp,0);/* alignment padding*/
    PUT(heap_listp + (1*WSIZE),PACK(DSIZE,1));/* prologue header */
    PUT(heap_listp + (2*WSIZE),PACK(DSIZE,1));/* prologue footer */
    PUT(heap_listp + (3*WSIZE),PACK(0,1));/* epilogue header */
    heap_listp+=(2*WSIZE);/*heap_listp points to prologue first word of content(footer,no content)*/
    
    /* Extend the empty heap with a free block of CHUNKSIZE bytes */
    if (extend_heap(CHUNKSIZE/WSIZE) == NULL)
    {
        return -1;
    }
    
    return 0;
}

/* 
 * mm_malloc - Allocate a block by incrementing the brk pointer.
 *     Always allocate a block whose size is a multiple of the alignment.
 */
void *mm_malloc(size_t size)
{
    int newsize = ALIGN(size + SIZE_T_SIZE);
    void *p = mem_sbrk(newsize);
    if (p == (void *)-1)
	return NULL;
    else {
        *(size_t *)p = size;
        return (void *)((char *)p + SIZE_T_SIZE);
    }
}

/*
 * mm_free - Freeing a block does nothing.
 */
void mm_free(void *ptr)
{
}

/*
 * mm_realloc - Implemented simply in terms of mm_malloc and mm_free
 */
void *mm_realloc(void *ptr, size_t size)
{
    void *oldptr = ptr;
    void *newptr;
    size_t copySize;
    
    newptr = mm_malloc(size);
    if (newptr == NULL)
      return NULL;
    copySize = *(size_t *)((char *)oldptr - SIZE_T_SIZE);
    if (size < copySize)
      copySize = size;
    memcpy(newptr, oldptr, copySize);
    mm_free(oldptr);
    return newptr;
}



void mm_checker(){
    // 1.Is every block in the free list marked as free?
    // 2.Are there any contiguous free blocks that somehow escaped coalescing?
    // 3.Is every free block actually in the free list?
    // 4.Do the pointers in the free list point to valid free blocks?
    // 5.Do any allocated blocks overlap?
    // 6.Do the pointers in a heap block point to valid heap addresses?
}

static void *extend_heap(size_t words){
    char *bp;
    size_t size;

    /* Allocate an even number of words to maintain alignment */
    size = (words % 2)?(words+1)*WSIZE:words*WSIZE;
    if ((long)(bp=mem_sbrk(size))==-1)
    {
        return NULL;
    }
    /* Initialize free block header/footer and the pilogue header*/
    PUT(HDRP(bp),PACK(size,0));
    PUT(FTRP(bp),PACK(size,0));
}









