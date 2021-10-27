// best_fit.c
// chooses the block that is closest in size; for memory allocation

#include <stdio.h>

#define m_max		25

void main()
{
	int fragment[m_max], b[m_max],p[m_max],  nb, np, temp;
	int lowest = 9999;

	static int barray[m_max], parray[m_max];
	printf("\n Memory Management Scheme - Best Fit \n");
	printf("Number of blocks ? ");
	scanf("%d", &nb);
	
	printf("\n Number of Processes ? ");
	scanf("%d", &np);
	
	printf("\n Size of blocks ? ");
	for ( int i = 1; i<nb; i++)
	{
	    printf("Block nr: %d:", i);
	        scanf("%d", &b[i]);
	}
	
	printf("\nEnter size of processes");
	for (int i = 1; i < np; i++)
	 {
	    printf("Process nr: %d", i);
	    scanf("%d", &p[i]);
	 }
	 
	 for ( int i = 1; i <=np; i++) 
	 {
	    for ( int j = 1; j <=nb; j++)
	    {
	        if (barray[j] != 1)
	        {
	            temp = b[j] - p[i];
	                if (temp >= 0)
	                    if (lowest > temp)
	                    {
	                        parray[i] = j;
	                        lowest = temp;
	                    }
	        }
	    }
	    fragment[i] = lowest;
	    barray[parray[i]] = 1;
	    lowest = 10000;
	 }
	 printf("\n Process nr: \t Process size \t Block nr \t Block size \t Fragment");
	 for (int i = 1; i<np && parray[i] != 0; i++)
	    printf("\n%d\t%d\t%d\t%d\t%d", i,p[i],parray[i],b[parray[i]],fragment[i]);
}
