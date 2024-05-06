#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>

#define NUM_PHILOSOPHERS 5 // 哲學家數量
#define MAX_EATING 4 // 同時最多允許的進餐數量

// 互斥鎖和條件變量
pthread_mutex_t lock;
pthread_cond_t cond[NUM_PHILOSOPHERS];

// 哲學家狀態
enum philosopher_state { THINKING, HUNGRY, EATING };
enum philosopher_state state[NUM_PHILOSOPHERS];

// 記錄哲學家編號
int Philosophers[NUM_PHILOSOPHERS] = {0,1,2,3,4};

// 命令行參數
int passed_argc;
char **passed_argv;

// 函數聲明
void *chow_line(void *nphilosopher);
void pickup_forks(int ph_num);
void putdown_forks(int ph_num);
void control(int ph_num);

int main(int argc, char **argv)
{
  passed_argc = argc;
  passed_argv = argv;
  
  pthread_t philosopher_life[NUM_PHILOSOPHERS];
  pthread_mutex_init(&lock, NULL);

  for (int i = 0; i < NUM_PHILOSOPHERS; i++)
    pthread_cond_init(&cond[i], NULL);

  for (int i = 0; i < NUM_PHILOSOPHERS; i++)
    pthread_create(&philosopher_life[i], NULL, chow_line, &Philosophers[i]);
    
  for (int i = 0; i < NUM_PHILOSOPHERS; i++)
    pthread_join(philosopher_life[i], NULL);

  pthread_mutex_destroy(&lock);
  for (int i = 0; i < NUM_PHILOSOPHERS; i++)
    pthread_cond_destroy(&cond[i]);

  return 0;
}

void *chow_line(void *nphilosopher)
{
  while(1) {       
    int *no_philosopher = nphilosopher;
    sleep(1 + (rand() % 2));   
                                
    printf("Philosopher %d is THINKING.\n", *no_philosopher + 1);
    if(passed_argc > 1 && strcmp(passed_argv[1], "--with-time") == 0)
      sleep(1+(rand()%2));
    state[*no_philosopher] = HUNGRY;
    printf("Philosopher %d is HUNGRY.\n", *no_philosopher + 1);
    pickup_forks(*no_philosopher);    
    putdown_forks(*no_philosopher);   
  }
}

void pickup_forks(int no_philosopher) {
  pthread_mutex_lock(&lock);      

  // 資源分層,奇偶編號哲學家採取不同的請求順序
  if (no_philosopher % 2 == 0) {
    control(no_philosopher);
    control((no_philosopher + 1) % NUM_PHILOSOPHERS);
  } else {
    control((no_philosopher + 1) % NUM_PHILOSOPHERS);
    control(no_philosopher);
  }

  while (state[no_philosopher] != EATING) 
    pthread_cond_wait(&cond[no_philosopher], &lock);   
                                                      
  pthread_mutex_unlock(&lock);    
}

void putdown_forks(int no_philosopher) {
  pthread_mutex_lock(&lock);

  state[no_philosopher] = THINKING;
  control((no_philosopher + NUM_PHILOSOPHERS - 1) % NUM_PHILOSOPHERS); 
  control((no_philosopher + 1) % NUM_PHILOSOPHERS);

  pthread_mutex_unlock(&lock);
}

void control(int no_philosopher) {
  // 限制同時進餐哲學家數量
  int eating_cnt = 0;
  for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
    if (state[i] == EATING)
      eating_cnt++;
  }

  if (state[no_philosopher] == HUNGRY && eating_cnt < MAX_EATING) {      
    state[no_philosopher] = EATING;
    printf("Philosopher %d is EATING.\n", no_philosopher + 1);
    if(passed_argc > 1 && strcmp(passed_argv[1], "--with-time") == 0)
      sleep(1+(rand()%2));
    pthread_cond_signal(&cond[no_philosopher]);
  }
}