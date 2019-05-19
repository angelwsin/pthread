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
            printf("%d %s",secondes,message);
        }
        
    }
}
