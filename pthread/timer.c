//
//  timer.c
//  pthread
//
//  Created by mac on 19/05/2019.
//  Copyright © 2019 mac. All rights reserved.
//

#include "timer.h"
#include <pthread.h>
#include <unistd.h>
#include <sys/wait.h>
#include <mm_malloc.h>


/**
 多进程和多线程对比：
 1）数据：  fork一个进程会拷贝父进程的地址空间，得到一个独立的地址空间，父进程可以随意的更改变量值
           pthread 线程是共享内存空间的，在主线程中可以malloc地址空间
 2）资源：  子进程等到程序退出由系统回收，通常回收子进程的方法是调用某个wait系列函数
           pthread 会持有线程的资源和返回值，如果不关心返回值则pthread_detach分离线程等线程终止了有系统自动回收
 **/

void  timer(){
    
    int secondes;
    char line[128];
    char message[64];
    while(1){
    
        if(fgets(line, sizeof(line), stdin)==NULL) exit(0);
        if(strlen(line)<=1) continue;
        if(sscanf(line, "%d %64[^\n]",&secondes,&message)<2){
            printf(stderr,"bad command\n");
        }else{
            //sleep
            sleep(secondes);
            printf("%d %s\n",secondes,message);
        }
        
    }
}


//难点在于：对所有已终止子进程的reap回收，如果程序处理，则要等到程序退出由系统回收
//通常回收子进程的方法是调用某个wait系列函数。本例中中调用waitpid并设置WNOHANG 父进程不必挂起等待子进程结束
void timermp(){
    int status ;
    char line[128];
    int seconds;
    pid_t pid;
    char message[64];
    while (1) {
        if(fgets(line, sizeof(line), stdin)==NULL) exit(0);
        if(strlen(line)<=1) continue;
        
        if(sscanf(line, "%d %64[^\n]",&seconds,&message)<2){
            printf(stderr,"bad command\n");
        }else{
            //fork 一个进程
            /*fork调用的一个奇妙之处就是它仅仅被调用一次，却能够返回两次，它可能有三种不同的返回值：
            1）在父进程中，fork返回新创建子进程的进程ID；
            2）在子进程中，fork返回0；
            3）如果出现错误，fork返回一个负值；
            
            在fork函数执行完毕后，如果创建新进程成功，则出现两个进程，一个是子进程，一个是父进程。在子进程中，fork函数返回0，在父进程中，
            
            fork返回新创建子进程的进程ID。我们可以通过fork返回的值来判断当前进程是子进程还是父进程。
            
            引用一位网友的话来解释fpid的值为什么在父子进程中不同。“其实就相当于链表，进程形成了链表，父进程的fpid(p 意味point)指向子进程的进程id,
            
            因为子进程没有子进程，所以其fpid为0.*/
            // fork 后就会克隆一个自己，使用pid区分是子进程还是父进程（有两个进程了执行以下相同的代码）
            pid = fork();
            if(pid==(pid_t)-1)
                exit(0);
            if(pid==(pid_t)0){
                sleep(seconds);
                printf("%d %s\n",seconds,message);
                exit(0);
            }else{
                do {
                    pid = waitpid((pid_t)-1,NULL,WNOHANG);
                    if(pid==(pid_t)-1)
                        exit(0);
                } while (pid!=(pid_t)0);
            }
            
        }
    }
}

typedef struct alarm_tag {
    int secondes;
    char message[64];
} alarm_t;

void *alarm_thread(void *arg){
    alarm_t *alarm = (alarm_t*) arg;
    int status;
    
    status = pthread_detach(pthread_self());
    if(status!=0){
        exit(0);
    }
    
    sleep(alarm->secondes);
    printf("%d %s\n",alarm->secondes,alarm->message);
    free(alarm);
    return NULL;
    
}

//多线程
void timerthread(){
    int status;
    char line[128];
    alarm_t *alarm;
    pthread_t thread;
    
    while(1){
        if(fgets(line, sizeof(line), stdin)==NULL) exit(0);
        if(strlen(line)<=1) continue;
        alarm = malloc(sizeof(alarm_t));
        if(alarm==NULL)
            exit(0);
        if(sscanf(line, "%d %64[^\n]",&alarm->secondes,alarm->message)<2){
            printf(stderr,"bad command\n");
            free(alarm);
        }else{
            status = pthread_create(&thread, NULL, alarm_thread, alarm);
            if(status!=0)
                exit(0);
        }
    }
    
}


