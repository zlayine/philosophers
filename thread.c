#include <stdio.h>
#include <pthread.h> 
#include <unistd.h>
#include <string.h> 

// void *myThreadFun(void *vargp) 
// { 
//     sleep(1); 
//     printf("Printing GeeksQuiz from Thread \n"); 
//     return NULL; 
// } 

// int main()
// {
// 	pthread_t thread_id; 
//     printf("Before Thread\n"); 
//     pthread_create(&thread_id, NULL, myThreadFun, NULL); 
//     pthread_join(thread_id, NULL); 
//     printf("After Thread\n"); 
// 	return (0);
// }

// int g = 0; 
  
// void *myThreadFun(void *vargp) 
// { 
//     int *myid = (int *)vargp; 
  
//     static int s = 0; 
  
//     ++s; ++g; 

// 	pthread_detach(pthread_self()); 
//     printf("Thread ID: %d, Static: %d, Global: %d\n", *myid, ++s, ++g);
// 	sleep(3);
//     printf("Thread Dead\n"); 
// 	return NULL;
// } 
  
// int main() 
// { 
//     int i; 
//     pthread_t tid; 
  
//     for (i = 0; i < 3; i++) 
// 	{
//         pthread_create(&tid, NULL, myThreadFun, (void *)&tid); 
// 		pthread_join(tid, NULL);
// 	}
  
//     pthread_exit(NULL); 
//     return 0; 
// } 


pthread_t tid[2]; 
int counter;
pthread_mutex_t lock[2]; 
  
void* trythis(void* arg) 
{
	int *name;

	pthread_mutex_lock(&lock[counter - 1]); 
    unsigned long i = 0; 
    counter += 1; 
    printf("\n Job %d has started\n", counter); 
    for (i = 0; i < (0xFFFFFFFF); i++) 
        ;
    printf("\n Job %d has finished\n", counter); 
  	pthread_mutex_unlock(&lock[counter - 2]); 
    return NULL; 
} 
  
int main(void) 
{ 
    int i = 0; 
    int error; 

	pthread_mutex_init(&lock[0], NULL);
	pthread_mutex_init(&lock[1], NULL);
	// if (pthread_mutex_init(&lock[0], NULL) != 0) { 
    //     printf("\n mutex init has failed\n"); 
    //     return 1; 
    // } 
  
    while (i < 2) { 
        error = pthread_create(&(tid[i]), NULL, &trythis, NULL);
        i++; 
    } 
  
    pthread_join(tid[0], NULL); 
    pthread_join(tid[1], NULL);
	pthread_mutex_destroy(&lock[0]); 
	pthread_mutex_destroy(&lock[1]); 
  
    return 0; 
} 
