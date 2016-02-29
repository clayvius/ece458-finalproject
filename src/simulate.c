#include <stdio.h>
#include <string.h>
#include <time.h>
#include <inttypes.h>

// DRAM timing constraints

#define tRC 50
#define tRAS 36
#define tRRD 6
#define tRP 14
#define tRFC 172
#define tCWL 10 // (tCWD)
#define tCAS 14 // (CL)
#define tRCD 14
#define tWR 16
#define tRTP 8
#define tBURST 4
#define tWTR 8

clock_t global_tick; /*CPU clock tick since the program execution*/

typedef struct
        {		
			char Operation[32];
            unsigned int full_address;
		    unsigned int row;
			unsigned int bank;
			unsigned int column;
            int time_issued;
        } instruction;

int main(int argc, char **argv)
{
		/* clock_t gives the CPU clock ticks since the start of the process */
		clock_t current_tick; 
		global_tick = clock();   	/* Start of the clock cycle*/
	    instruction Queue[15];  	/*Arrays of Structs */
		FILE * fp;					/* File handler */
        char buf[128];				/* Temporary buffer */
        char commandFile[64];
        char *token;
        int OpenSlot = 0;
		double cpu_ticks_used;
        int i;

        if (argc != 2)
        {
                printf("You must enter a testfile to use, 'simulate testfile.txt'\n");
                return -1;
        }

        if ((fp = fopen(argv[1],"r")) == NULL)
        {
                printf("Could not open file: %s\n", argv[1]);
                return -1;
        }
        
	    while (fgets(buf,128,fp)!=NULL)
        {
	          	token = strtok(buf,"\t"); 
		               
	         	Queue[OpenSlot].full_address = (unsigned int) strtol(token,NULL,16);
		
				/*Break into row, bank and column */
				Queue[OpenSlot].row = (Queue[OpenSlot].full_address & 0xFFFE0000)>>17;
				Queue[OpenSlot].bank = (Queue[OpenSlot].full_address & 0x1C000)>>14 ;
				Queue[OpenSlot].column = (Queue[OpenSlot].full_address & 0x3FF8)>>3;

                token = strtok(NULL,"\t");  /*strtok already contains the string, just point to next string until \t */
                strcpy(Queue[OpenSlot].Operation,token);

                token = strtok(NULL,"\n");
                Queue[OpenSlot].time_issued = atoi(token);
                OpenSlot++;
			
		}

        fclose(fp);

        for (i=0; i<OpenSlot; i++) 
        {
                printf("----------------------------------\n");
                printf("Slot             :%d\n",i);
                printf("Operation        : %s\n",Queue[i].Operation);
                printf("Full_Address     : 0x%x\n",Queue[i].full_address);
			    printf("Row              : 0x%x\n",Queue[i].row);
				printf("Bank             : 0x%x\n",Queue[i].bank);
				printf("Column           : 0x%x\n",Queue[i].column);
                printf("Time_Issued      : %d\n",Queue[i].time_issued);
                printf("-----------------------------------\n");
        }
	
		current_tick = clock();
		cpu_ticks_used = (double)(current_tick-global_tick);
		printf("Total clock ticks used: %f \n",(double)(clock())/CLOCKS_PER_SEC);
        return 0;

}