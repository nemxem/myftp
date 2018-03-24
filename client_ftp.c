#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define BUFSIZE 1000

void getargs(char *word);
void put(int s, char *com[]);
void get(int s, char *com[]);

int main(){
    int s;
    char *ip;
    in_port_t port;
    struct sockaddr_in myskt;
    struct sockaddr_in skt;
    char buf[1000];
    char *com[100];
    int i = 0;
    int t = -1;

    ip = (char *)malloc(sizeof(char)*100);
    for(int k = 0;k < 100; k++){
       com[k] = (char *)malloc(sizeof(char)*20);
    }

    strcpy(ip, "0.0.0.0");
    port = atoi("10001");
    
    s = socket(PF_INET, SOCK_STREAM, 0);

    memset(&skt, 0, sizeof skt);
    skt.sin_family = AF_INET;
    skt.sin_port = htons(port);
    skt.sin_addr.s_addr = inet_addr(ip);
    
    connect(s, (struct sockaddr *)&skt, sizeof skt);

    for(;;){
        //printf("FTP$");
        for(i=0;i<2;i++){
            getargs(com[i]);
            printf("i = %d\n",i);
        }
        printf("com[1] = %s\n",com[1]);
        if(strcmp(com[0], "put") == 0){
            put(s,&com[1]);
        }else if(strcmp(com[0], "get") == 0){
            get(s,&com[1]);
        }
        printf("end ok\n");
    }


}

void getargs(char *com){
    char word[10];
    char ch;
    int n = 0;
    for(;;){
        ch = getc(stdin);
        if(ch == ' '||ch =='\n'){
            printf("getargs;ok\n");
            strcpy(com,word);
            break;
        }else{
            word[n] = ch;
            n++;
        }
    }
}

void put(int s, char *com[]){
    //printf("ok");

    int n;    
    FILE *fp;
    uint8_t *data;
    char buf[BUFSIZE];
    uint8_t *ftp_pay;
    struct ftp_header{
        uint8_t type;
        uint8_t code;
        uint16_t length;
    };
    struct ftp_header *send_header;

    fp = fopen(com[1],"r");
    n = strlen(com[1])+1;
       
    data = (uint8_t*)malloc(sizeof(struct ftp_header)+sizeof(uint8_t)*n);
    send_header = (struct ftp_header *) data;


    ftp_pay = data + sizeof(struct ftp_header);
    memcpy(ftp_pay, com[1], n);

    send_header->type = 0x06;
    send_header->code = 0x00;
    send_header->length = htons(n);

    send(s, data,sizeof(struct ftp_header)+n ,0);

    do{
        n = fread(buf, sizeof(char), BUFSIZE, fp);
        data = (uint8_t*)malloc(sizeof(struct ftp_header)+sizeof(uint8_t)*n);
        send_header = (struct ftp_header *) data;
        ftp_pay = data + sizeof(struct ftp_header);

        memcpy(ftp_pay, buf, n);

        send_header->type = 0x20;
        if(n<BUFSIZE){
            send_header->code = 0x01;
        }else{
            send_header->code = 0x00;
        }
        send_header->length = htons(n);

        send(s,data,sizeof(struct ftp_header)+n,0);

        n -= BUFSIZE;
 
    }while(n >= 0);
}

void get(int s, char *com[]){
    
}
