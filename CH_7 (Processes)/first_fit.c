// first_fit.c
// chooses the first available block that is large enough; for memory allocation

#include <stdio.h>

#define m_max		25

void main()
{
	int allocation[m_max], bsize[m_max],psize[m_max], flags[m_max], bno, pno, temp;

	printf("\n Memory Management Scheme - First Fit");

    for (int i = 0; i < m_max; i++)
    {
        flags[i] = 0;
        allocation[i] = -1;
    }
    
	printf("Number of blocks ? ");
	scanf("%d", &bno);
	
	printf("\n Number of Processes ? ");
	scanf("%d", &pno);
	
	printf("\n Size of blocks ? ");
	for ( int i = 0; i<bno; i++)
	{
	        scanf("%d", &bsize[i]);
	}
	
	printf("\nEnter size each process");
	for (int i = 0; i < pno; i++)
	 {
	    scanf("%d", &psize[i]);
	 }
	 for (int i = 0; i < pno; i++)      // first fit
	 {
	    for (int j = 0; j < bno; j++)
	    {
	        if(flags[j] == 0 && bsize[j] >= psize[i])
	        {
	            allocation[i] = i;
	            flags[j] = 1;
	            break;
	        }
	    }
	 }
	 
	 printf("\nBlock nr \t Block size \t Process nr: \t Process size");
	 for (int i = 1; i<bno ; i++)
	 {
	    printf("\n%d\t%d\t", i+1, bsize[i]);
	    if(flags[i] == 1)
	    {
	        printf("%d\t\t%d", allocation[i]+1, psize[allocation[i]]);
	    } else {
	        printf("Not allocated");
	    }
    }
}
