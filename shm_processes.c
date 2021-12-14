#include  <stdio.h>
#include  <stdlib.h>
#include  <sys/types.h>
#include  <sys/ipc.h>
#include  <sys/shm.h>
#include <unistd.h>
#include <sys/wait.h>

void  Poor_Student_Process(int []);
void  Dear_old_dad_Process(int []);

int  main(int  argc, char *argv[])
{
     int    ShmID;
     int    *ShmPTR;
     pid_t  pid;
     int    status;

    

     ShmID = shmget(IPC_PRIVATE, 2*sizeof(int), IPC_CREAT | 0666);
     if (ShmID < 0) {
          printf("*** shmget error (server) ***\n");
          exit(1);
     }
     printf("main has received a shared memory of four integers...\n");

     ShmPTR = (int *) shmat(ShmID, NULL, 0);
     if (*ShmPTR == -1) {
          printf("*** shmat error (server) ***\n");
          exit(1);
     }
     printf("main has attached the shared memory...\n");
  
     ShmPTR[0]= 0;  //= bank account
     ShmPTR[1]= 0;// = turn
     

     
     printf("main has filled %d %d in shared memory...\n",
            ShmPTR[0], ShmPTR[1]);

     printf("main is about to fork a child process...\n");
     pid = fork();
     if (pid < 0) {
          printf("*** fork error (server) ***\n");
          exit(1);
     }
     else if (pid == 0) {
          Poor_Student_Process(ShmPTR);
          exit(0);
     }
     else{
          Dear_old_dad_Process(ShmPTR);
     }

     wait(&status);
     printf("main has detected the completion of its child...\n");
     shmdt((void *) ShmPTR);
     printf("main has detached its shared memory...\n");
     shmctl(ShmID, IPC_RMID, NULL);
     printf("main has removed its shared memory...\n");
     printf("main exits...\n");
     exit(0);
}

void  Poor_Student_Process(int  SharedMem[])
{
     int x, account, randomVal;
     srand(getpid());
     for(x=0; x<25; x++){
       sleep(rand()%6);
       account= SharedMem[0];
       while(SharedMem[1] != 0)//doing nothing
         ;
      SharedMem[1]=0;
      randomVal=rand()%51;
      printf("Poor Student needs $%d\n", randomVal);
      if(randomVal<= account){
        account -= randomVal;
        printf("Poor Student: Withdraws $%d / Balance = $%d\n", randomVal, account);
      }
      else if(randomVal > account){
        printf("Poor Student: Not Enough Cash ($%d)\n", account );
      }
      SharedMem[0]= account;
      
    }
  exit(1);
}

void  Dear_old_dad_Process(int  SharedMem[])
{
     int x, account, randomVal;
     srand(getpid());
     for(x=0; x<25; x++){
       sleep(rand()%6);
       account= SharedMem[0];
       
       while(SharedMem[1] != 0)//doing nothing
         ;
       SharedMem[1]=1;
       if(account <=100){
        
         randomVal=rand()%101;
         
         if(randomVal %2 == 0){
           account += randomVal;
           printf("Dear old Dad: Deposits $%d / Balance = $%d\n", randomVal, account);
         }
         else{
           printf("Dear old Dad: Doesn't have any money to give\n");

         }
         
        SharedMem[0]= account;
        
        
       }
       else{
         printf("Dear old Dad: Thinks Student has enough Cash ($%d)\n", account);
       }
       
    }
  exit(1);
}