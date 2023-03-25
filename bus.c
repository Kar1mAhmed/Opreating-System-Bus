#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define Bus_size 5`



/* 
this code simulate a bus with number of seats if there is seats available people are abel to get into the bus and everyone come into the number
of seats decreases by one , then if someone in the bus then people able to get out , the process repeat until the driver reach his daily target.
*/ 


/
int target;
int available_seats=Bus_size;


// to make sure number of people got in = number of people got out = target
int num_got_in=0;
int num_got_out=0;

//declare mutex with default behavior
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;

pthread_cond_t c_in = PTHREAD_COND_INITIALIZER;
pthread_cond_t c_out= PTHREAD_COND_INITIALIZER;

// make the compiler enable to see the function before declaration (Function prototypes)
void *get_in (void *param);
void *get_out (void *param);

//boolen to check if the target reached
int target_not_reached=1;

int main(int argc, char *argv[]) {

      printf("enter the target : ");
      scanf("%d",&target);
      


      pthread_t tid1,tid2;

      // start the children threads
      pthread_create(&tid1, NULL, get_in, NULL) ;
	pthread_create(&tid2, NULL, get_out, NULL) ;
		

      /* wait for created thread to exit */
      pthread_join(tid1, NULL);
      pthread_join(tid2, NULL);
      printf("num got in %d and num got out %d and the target was %d \n",num_got_in,num_got_out,target);
      printf("Parent quiting\n");

      return 0;

}

void *get_in(void *param){


      int i;

      // work until reach the target of the day
      for(i=0;i<target;i++)
      {
            pthread_mutex_lock(&m);

            // waiting someone get into the bus
            while(available_seats==0)
            {
                  pthread_cond_wait(&c_in,&m);
            }

            //someone take a seat
            available_seats--;
            num_got_in++;
		printf ("someone got into the bus there is %d  seats left\n", available_seats);

            pthread_mutex_unlock (&m);

            pthread_cond_signal (&c_out);

      }
      
      printf("the target has reached\n");
      target_not_reached=0;
      return 0;
}


void *get_out(void *param){

      while(target_not_reached || available_seats!=Bus_size)
      {
            pthread_mutex_lock(&m);

            while(available_seats==Bus_size){
                  pthread_cond_wait(&c_out,&m);
            }

            //someone left the bus
            available_seats++;
            num_got_out++;
		printf ("someone got out  the bus there is %d seats left \n", available_seats); 

            pthread_mutex_unlock (&m);

            pthread_cond_signal (&c_in);

      }

      printf("day has ended i am out \n");
      return 0;

}