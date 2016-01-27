/***************************************************************/
/*         PROGRAM NAME:         PRODUCER_CONSUMER             */
/*    This program simulates two processes, producer which     */
/* continues  to produce message and  put it into a buffer     */
/* [implemented by PIPE], and consumer which continues to get  */
/* message from the buffer and use it.                         */
/*    The program also demonstrates the synchronism between    */
/* processes and uses of PIPE.                                 */
/***************************************************************/

#define PIPESIZE 8

#define PRODUCER 0
#define CONSUMER 1

#define RUN      0    /* statu of process */
#define WAIT     1    /* statu of process */
#define READY    2    /* statu of process */

#define NORMAL   0
#define SLEEP    1
#define AWAKE    2

#include <stdio.h>

struct pcb
{
	char *name;
	int  statu;
	int  time;	/* times of execution */
};

struct pipetype
{
	char type;
	int  writeptr;
	int  readptr;
	struct pcb *pointp;	/* write wait point */ // producer queue
	struct pcb *pointc;	/* read wait point  */ // consumer queue
};

int pipe[PIPESIZE];
struct pipetype pipetb;
struct pcb process[2];
int count=0;	// resource count
int producerCount=0, consumerCount=0;	// semaphore

int runp(int out, struct pcb p[], int pipe[], struct pipetype *tb, int t)
{
	p[t].statu=RUN;
	printf("run PRODUCER. product %d     ",out);
	if(count >= 8)	// full
	{
		p[t].statu=WAIT;
		return(SLEEP);
	}
	pipe[tb->writeptr]=out;
	tb->writeptr=++tb->writeptr%8;	// revised
	// tb->writeptr++;
	printf("writeptr%d\n",tb->writeptr);	// revised
	count++;	// revised
	printf("count = %d\n",count);	// revised
	p[t].time++;
	printf("time = %d\n",p[t].time);	// revised
	p[t].statu=READY;
	if((tb->pointc) != NULL)
	{
		printf("return AWAKE");
		return(AWAKE);
	}
	return(NORMAL);
}

int runc(struct pcb p[], int pipe[], struct pipetype *tb, int t)
{
	int c;
	p[t].statu = RUN;
	printf("run CONSUMER. ");
	if(count <= 0)	//
	{
		p[t].statu=WAIT;
		return(SLEEP);
	}
	c=pipe[tb->readptr];
	pipe[tb->readptr]=0; // revised
	// tb->readptr++;
	tb->readptr=++tb->readptr%8;	// revised
	printf("readptr=%d\n",tb->readptr);	// revised
	printf(" use %d      ",c);
	// delete some code
	// if(tb->readptr >= tb->writeptr)
	// {
	// 	tb->readptr=tb->writeptr=0;
	// }
	count--;// revised
	printf("count = %d\n",count);	// revised
	p[t].time++; 
	printf("time = %d\n",p[t].time);	// revised
	p[t].statu=READY;
	if(tb->pointp != NULL)
	{
		printf("return AWAKE\n");	// revised
		return(AWAKE);
	}
	return(NORMAL);
}

void prn(struct pcb p[], int pipe[], struct pipetype tb)
{
	int i;
	printf("\n         ");
	for(i=0;i<PIPESIZE;i++)
	{
		printf("------ ");	// print upper border
	}
	printf("\n        |");	// left dash
	for(i=0;i<PIPESIZE;i++)	// second line and vertical bar
	{
		if(pipe[i] != 0)
		{
			printf("  %2d  |",pipe[i]);
		}
		else
		{
			printf("      |");
		}
	}
	printf("\n         ");
	for(i=0;i<PIPESIZE;i++)	// print lower border
	{
		printf("------ ");
	}
	printf("\nwriteptr = %d, readptr = %d,  ",tb.writeptr,tb.readptr);
	if(p[PRODUCER].statu == WAIT)
	{
		printf("PRODUCER wait ");
	}
	else
	{
		printf("PRODUCER ready ");
	}
	if(p[CONSUMER].statu == WAIT)
	{
		printf("CONSUMER wait ");
	}
	else
	{
		printf("CONSUMER ready ");
	}
	printf("\n");
}

int main(int argc, char** argv)
{
	int output,ret,i;
	char in[2];

	pipetb.type = 'c';
	pipetb.writeptr = 0;
	pipetb.readptr = 0;
	pipetb.pointp = pipetb.pointc = NULL;
	process[PRODUCER].name = "Producer\0";
	process[CONSUMER].name = "Consumer\0";
	process[PRODUCER].statu = process[CONSUMER].statu = READY;
	process[PRODUCER].time = process[CONSUMER].time = 0;
	output = 0;
	printf("Now starting the program!\n");
	printf("Press 'p' to run PRODUCER, press 'c' to run CONSUMER.\n");
	printf("Press 'e' to exit from the program.\n");
	for(i=0;i<1000;i++)
	{
		in[0]='N';
		while(in[0] == 'N')
		{
			scanf("%s",in);
			if(in[0] != 'e'&&in[0] != 'p'&&in[0] != 'c')	// if input is not e/p/c, input again
			{
				in[0]='N';
			}
		}
		if(in[0] == 'e')
		{
			return 1;	// exit(1);
		}
		if(in[0] == 'p'&&process[PRODUCER].statu == READY)
		{
			if(count<8)	// revised
			{
				output = (output+1)%100;
			}
			if((ret=runp(output,process,pipe,&pipetb,PRODUCER)) == SLEEP)
			{
				pipetb.pointp = &process[PRODUCER];
			}
			if(ret == AWAKE)
			{
				(pipetb.pointc)->statu=READY;
				runc(process,pipe,&pipetb,CONSUMER);	// revised
				consumerCount--;	// revised
				printf("consumerCount=%d\n",consumerCount);	// revised
				if(consumerCount == 0)	// revised
				{	// revised
					pipetb.pointc=NULL;// revised
				}	// revised
			}
		}
		if(in[0] == 'c'&&process[CONSUMER].statu == READY)
		{
			if((ret=runc(process,pipe,&pipetb,CONSUMER)) == SLEEP)
			{
				pipetb.pointc = &process[CONSUMER];
			}
			if(ret == AWAKE)
			{
				(pipetb.pointp)->statu=READY;
				output=(output+1)%100;	// revised
				runp(output,process,pipe,&pipetb,PRODUCER);
				producerCount--;	// revised
				printf("producerCount=%d\n",producerCount);	// revised
				if(producerCount == 0)	// revised
				{	// revised
					pipetb.pointp=NULL;// revised
				}	// revised
			}
		}
		if(in[0] == 'p'&&process[PRODUCER].statu == WAIT)
		{
			producerCount++;	// revised
			printf("producerCount=%d\n",producerCount);	// revised
			printf("PRODUCER is waiting, can't be scheduled.\n");
		}
		if(in[0] == 'c'&&process[CONSUMER].statu == WAIT)
		{
			consumerCount++;	// revised
			printf("consumerCount=%d\n",consumerCount);	// revised
			printf("CONSUMER is waiting, can't be scheduled.\n");
		}
		prn(process,pipe,pipetb); 
		in[0]='N';
	}
	return 0;
}
