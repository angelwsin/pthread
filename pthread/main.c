//
//  main.c
//  pthread
//
//  Created by mac on 19/05/2019.
//  Copyright © 2019 mac. All rights reserved.
//

#include <stdio.h>

int main(int argc, const char * argv[]) {
  
    //同步版定时器
    //timer();
    //多进程版
    //timermp();
    //多线程版
    timerthread();
    return 0;
}
