//
//  main.c
//  ComputerNetworkProjectOne
//
//  Created by ylx on 2017/3/18.
//  Copyright © 2017年 ylx. All rights reserved.
//

#define LIST "ls"
#define DOWNLOAD "get"
#define UPLOAD "put"
#define EXIT "exit"
#define DELIM " "

#include "CommunicateFunction.h"


void start(char *ip,int port);

int main(int argc, const char * argv[]) {
    if (argc != 3 || (!strcmp(argv[1], "-h") || !strcmp(argv[2], "--help"))) {
        printf("");
        return 0;
    }else if (argc == 3) {
        start(argv[1],atoi(argv[2]));
    }
    return 0;
}

void start(char *ip,int port){
    while (YES) {
        int socket_fd = get_connection(ip, port);
        char command[256];
        char *temp;
        printf("> ");
        scanf("%s",command);
        if (!strcmp(command, LIST)) {
            get_file_list(socket_fd);
        }else if (!strcmp(command, DOWNLOAD)){
            scanf("%s",command);
            send_download_request(temp, socket_fd);
            scanf("%s",command);
            analyse_response(socket_fd, command);
        }else if (!strcmp(temp, UPLOAD)){
            temp = strtok(NULL, DELIM);
            upload(temp, socket_fd);
            analyse_response(socket_fd, NULL);
        }else if (!strcmp(temp, EXIT)){
            close(socket_fd);
            break;
        }else{
            printf("command %s not found\n",command);
        }
        close(socket_fd);
    }
    
}
