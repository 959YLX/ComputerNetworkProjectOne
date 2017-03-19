//
//  CommunicateFunction.h
//  ComputerNetworkProjectOne
//
//  Created by ylx on 2017/3/18.
//  Copyright © 2017年 ylx. All rights reserved.
//

#ifndef CommunicateFunction_h
#define CommunicateFunction_h

#include <inttypes.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/select.h>
#include <errno.h>

#define DOWNLOAD_TYPE 0
#define LIST_TYPE 1
#define UPLOAD_TYPE 2

#define ERROR_TYPE 0
#define SUCCESS_TYPE 1
#define STREAM_TYPE 2

#define YES 1
#define NO 0
#define BOOL unsigned char

#define FILE_PERMISSION 0644
#define FILE_CACHE_SIZE 4096
#define TEXT_CACHE_SIZE 1000

#define PROCESS_BAR_AMOUNT 50

#define OVERTIME 5


#endif /* CommunicateFunction_h */

void download(const char *local_file_name,const int socket_fd);
void send_download_request(const char *server_file_name,const int socket_fd);
void get_file_list(const int socket_fd);
char* catch_error(const int socket_fd);
char* catch_success(const int socket_fd);
void analyse_response(const int socket_fd,char *fileName);
void upload(const char *full_path_name,const int socket_fd);
int get_connection(const char *IP,const int PORT,BOOL first);
