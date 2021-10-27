// my_malloc.c
// Example from Kernighan and Ritchie, 8.7 [1988]

#include <stddef.h>
#include <stdlib.h>
/*
 * The Align field is never used, it just forces each Header to be aligned on a worst-case boundary
 */

typedef long Align;		                  // alignment for to long boundary

union header { // block header
	struct {
			union header *ptr;		         //next block if on free list
			unsigned size;			                // size of this block
	} s;
	Align x;						                       //force alignment of blocks
};

typedef union header Header;

static Header base;                        // empty list to get started
static Header *freep = NULL;         // start of free list

/* my_malloc general-purpose storage allocator */

void *my_malloc(unsigned nbytes)
{
    Header *p, *prevp;
    Header *morecore(unsigned);
    unsigned nunits;

    nunits = (nbytes + sizeof(Header)-1) / sizeof(Header) +1;
    if ((prevp = freep) == NULL)        // No free bytes list yet
    {
        base.s.ptr = freep = prevp = &base;
        base.s.size = 0;
    }

    for ( p = prevp -> s.ptr; ; prevp = p, p = p->s.ptr)
    {
        if (p -> s.size >= nunits)          // big enough
        {
            if ( p->s.size == nunits)        // exactly
            {
                prevp->s.ptr = p->s.ptr;

            } else {                                // allocate tail end
                    p->s.size -= nunits;
                    p += p->s.size;
                    p->s.size = nunits;
                }

                freep = prevp;
                return (void *) (p+1);
        }
        if (p == freep)                            // wrapped around free list
            if ((p = morecore(nunits)) == NULL)
                return NULL;                        // none left
    }
}

#define NALLOC      1024    // minimum #units to request
/*
 * the function morecore obtains storage from the os. The details of how varies from system
 * to system. since asking for memory is expensive, we dont want to do that on every call to
 * malloc, so morecore requests atleast NALLOC units; this larger block will be chopped up as
 * needed. After setting the size field, morecore inserts the additional memory into the arena
 * by calling free.
 *
 * Syscall , sbrk = returns a pointer to n more bytes of storage
 *
 * Aside; There is the assumption that pointers of different blocks returned by sbrk can be
 * meaningfully compared. This is not guaranteed by the standard, which permits pointer
 * comparisons only within an array, thus thus version of malloc is portable only among
 * machines for which general pointer comparison is meaningful
 */
/* morecore: ask system for more memory */
Header *morecore(unsigned nu)
{
    char *cp, *sbrk(int);
    Header *up;

    if (nu < NALLOC)
        nu = NALLOC;
	cp = sbrk(nu * sizeof(Header));
	if( cp == (char *) -1)		            //no space at all
		return NULL;
	up = (Header *) cp;
	up -> s.size = nu;
	free((void*) (up+1));
	return freep;
}

/*
 * free itself is the last thing. It scans the free list, starting at freep, looking for the place to insert
 * the free block. This is either between two existing blocks or at the end of the list. In case, if the block
 * beeing freed is adjacent to either neighbour, the adjacent blocks are combined. The only troubles are
 * keeping the pointers pointing to the right things and the sizes correct !
 */

/* free: 	put block ap in free list */
void free(void *ap)
{
	Header *bp, *p;

	bp = (Header *) ap -1;		        // point to block header
	for ( p = freep; !(bp > p && bp < p->s.ptr); p = p->s.ptr)
		if ( p >= p->s.ptr && ( bp > p || bp < p->s.ptr) )
			break;		                        //freed block at start or end of arena

	if (bp + bp->s.size == p->s.ptr)	//join to upper nbr
	{
		bp->s.size += p -> s.ptr-> s.size;
		bp->s.ptr = p->s.ptr->s.ptr;
	} else {
		bp ->s.ptr = p->s.ptr;
	}

	if (p + p->s.size == bp)			//join to lower nbr
	{
		p->s.size += bp -> s.size;
		p->s.ptr = bp->s.ptr;
	} else {
		p->s.ptr = bp;
	}
	freep = p;
}
