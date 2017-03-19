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
#define HELP "help"
#define DELIM " "

#include "CommunicateFunction.h"

void show_use_info();
void show_command_info();
void start(char *ip,int port);

int main(int argc, const char * argv[]) {
    if (argc != 3 || (!strcmp(argv[1], "-h") || !strcmp(argv[2], "--help"))) {
        show_use_info();
        return 0;
    }else if (argc == 3) {
        start((char*)argv[1],atoi(argv[2]));
    }
    return 0;
}

void show_use_info(){
    printf("usage: command <IP> <PORT>\n");
    printf("       -h,--help Display available options\n");
}

void show_command_info(){
    printf("COMMAND:\n");
    printf("  ls    List existed file in server\n");
    printf("  get <Server-FileName> <Local-FileName>    Download selected file in server ,save it in an appointed path\n");
    printf("  put <Local-FileName>    Upload selected file to server\n");
}

void start(char *ip,int port){
    int socket_fd = 0;
    do{
        socket_fd = get_connection(ip, port,!socket_fd);
        if (socket_fd == 0) {
            break;
        }
        char command[256];
        char *temp;
        do{
            printf("> ");
            fgets(command , 255, stdin);
        }while(!strcmp(command, "\n"));
        command[strlen(command) - 1] = '\0';
        temp = strtok(command, DELIM);
        if (!strcmp(temp, LIST)) {
            get_file_list(socket_fd);
        }else if (!strcmp(temp, DOWNLOAD)){
            temp = strtok(NULL, DELIM);
            send_download_request(temp, socket_fd);
            temp = strtok(NULL, "");
            analyse_response(socket_fd, temp);
        }else if (!strcmp(temp, UPLOAD)){
            temp = strtok(NULL, "");
            upload(temp, socket_fd);
            analyse_response(socket_fd, NULL);
        }else if (!strcmp(temp, EXIT)){
            close(socket_fd);
            break;
        }else if(!strcmp(temp, HELP)){
            show_command_info();
            continue;
        }else{
            printf("command %s not found\n",command);
            printf("type \"help\" to get help document\n");
            continue;
        }
        close(socket_fd);
    }while (YES);
}
