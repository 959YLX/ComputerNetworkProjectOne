//
//  CommunicateFunction.c
//  ComputerNetworkProjectOne
//
//  Created by ylx on 2017/3/18.
//  Copyright © 2017年 ylx. All rights reserved.
//

#include "CommunicateFunction.h"

void download(const char *local_file_name,const int socket_fd){
    int file_fd = open(local_file_name, O_WRONLY|O_CREAT,FILE_PERMISSION);
    uint32_t file_size;
    read(socket_fd, &file_size, sizeof(uint32_t));
    file_size = ntohl(file_size);
    int current = 0,temp,location = 0;
    ssize_t delta = file_size/PROCESS_BAR_AMOUNT;
    ssize_t receive_size,surplus_size = file_size;
    char cache[FILE_CACHE_SIZE];
    printf("[");
    fflush(stdout);
    while (YES) {
        if (surplus_size == 0) {
            break;
        }
        receive_size = read(socket_fd, cache, (surplus_size <= FILE_CACHE_SIZE ? surplus_size : FILE_CACHE_SIZE) );
        write(file_fd, cache, receive_size);
        
        
        surplus_size = (surplus_size >= receive_size ? surplus_size - receive_size : 0);
        temp = (int)(file_size - surplus_size)/delta;
        current = temp > current ? temp : current;
        if (current - location == 1) {
            printf("=");
            fflush(stdout);
            location++;
        }

    }
    printf("]\n");
    fflush(stdout);
    close(file_fd);
}

void send_download_request(const char *server_file_name,const int socket_fd){
    long file_name_length = strlen(server_file_name);
    ssize_t size = (2 * sizeof(uint8_t) + file_name_length);
    char *data = (char*)malloc(size);
    data[0] = DOWNLOAD_TYPE;
    data[1] = (uint8_t)file_name_length;
    memcpy(data + 2, server_file_name, file_name_length);
    write(socket_fd, data, size);
    free(data);
}

void get_file_list(const int socket_fd){
    uint8_t type = LIST_TYPE;
    write(socket_fd, &type, sizeof(type));
    read(socket_fd, &type, sizeof(type));
    if (type == SUCCESS_TYPE) {
        char buff[TEXT_CACHE_SIZE];
        read(socket_fd, buff, sizeof(uint16_t));
        while (read(socket_fd, buff, TEXT_CACHE_SIZE) > 0) {
            printf("%s\n",buff);
        }
    }
}

char* catch_error(const int socket_fd){
    uint8_t info_size;
    read(socket_fd, &info_size, sizeof(info_size));
    char *info = (char*)malloc(info_size);
    read(socket_fd, info, info_size);
    return info;
}

char* catch_success(const int socket_fd){
    uint16_t info_size;
    read(socket_fd, &info_size, sizeof(info_size));
    info_size = ntohs(info_size);
    char *info = (char*)malloc(info_size);
    read(socket_fd, info, info_size);
    return info;
}

void analyse_response(const int socket_fd,char *fileName){
    uint8_t type = 3;
    read(socket_fd, &type, sizeof(uint8_t));
    switch (type) {
        case STREAM_TYPE:{
            download(fileName, socket_fd);
            analyse_response(socket_fd, NULL);
            break;
        }
            
        case ERROR_TYPE:{
            char *error_info = catch_error(socket_fd);
            printf("%s",error_info);
            break;
        }
            
        case SUCCESS_TYPE:{
            char *success_info = catch_success(socket_fd);
            printf("%s",success_info);
            break;
        }
            
        default:
            break;
    }
}

void upload(const char *full_path_name,const int socket_fd){
    char *temp = strrchr(full_path_name, '/');
    char *file_name = (char*)malloc(strlen(temp) - 1);
    strcpy(file_name, temp + 1);
    struct stat file_info;
    stat(full_path_name, &file_info);
    uint32_t file_real_size = (uint32_t)file_info.st_size;
    uint32_t file_size = htonl(file_real_size);
    uint32_t delta = file_real_size/PROCESS_BAR_AMOUNT;
    ssize_t filename_size = strlen(file_name);
    int file_fd = open(full_path_name, O_RDONLY);
    ssize_t part_size = 2 * sizeof(uint8_t) + sizeof(uint32_t) + filename_size;
    char *part_of_data = (char*)malloc(part_size);
    part_of_data[0] = UPLOAD_TYPE;
    part_of_data[1] = filename_size;
    memcpy(part_of_data + 2, &file_size, sizeof(uint32_t));
    memcpy(part_of_data + 2 + sizeof(uint32_t), file_name, filename_size);
    write(socket_fd, part_of_data, part_size);
    char buff[512];
    ssize_t read_size,total_size = 0,current = 0,location = 0;
    printf("[");
    fflush(stdout);
    while ((read_size = read(file_fd, buff, sizeof(buff))) > 0) {
        write(socket_fd, buff, read_size);
        total_size += read_size;
        current = (total_size/delta) > current ? (total_size/delta) : current;
        while (current > location) {
            printf("=");
            fflush(stdout);
            location++;
        }
    }
    printf("]\n");
    close(file_fd);
    free(file_name);
    free(part_of_data);
}

int get_connection(const char *IP,const int PORT){
    int socked_fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in socket_addr;
    socket_addr.sin_family = AF_INET;
    socket_addr.sin_port = htons(PORT);
    socket_addr.sin_addr.s_addr = inet_addr(IP);
    socket_addr.sin_len = sizeof(struct sockaddr_in);
    connect(socked_fd, (struct sockaddr*)&socket_addr, sizeof(socket_addr));
    return socked_fd;
}
