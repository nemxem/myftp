#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>

#define BUFSIZE 1000

void execute_com(int);
void exe_retr(int, char *);
void exe_str(uint8_t, char *);

int main(){
    int master_sock;
    int client_sock;
    in_port_t port;
    struct sockaddr_in myskt;
    struct sockaddr_in skt;
    socklen_t sktlen;
    int pid;

    port = atoi("10001");

    master_sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&myskt, 0, sizeof myskt);
    myskt.sin_family = AF_INET;
    myskt.sin_port = htons(port);
    myskt.sin_addr.s_addr = htonl(INADDR_ANY);

    bind(master_sock, (struct sockaddr *)&myskt, sizeof myskt);
    listen(master_sock, 5);
    printf("listen\n");

    sktlen = sizeof skt;

    for(;;){
        client_sock = accept(master_sock, (struct sockaddr *)&skt, &sktlen);
        printf("accept\n");
        if((pid = fork()) < 0){
            //エラー処理
        }else if(pid == 0){
            execute_com(client_sock);
        }else{
            close(client_sock);
        }
    }
}

void execute_com(int s){
    struct ftp_header{
        uint8_t type;
        uint8_t code;
        uint16_t length;
    };
    struct ftp_header recv_header;
    uint8_t type;
    uint8_t code;
    uint16_t data_len;
    char buf[BUFSIZE];
    int n;
    uint8_t *ftp_pay;
    int error;

    for(;;){
        printf("before recv\n");
        error = recv(s, &recv_header, sizeof(struct ftp_header), 0);
        printf("error = %d", error);
        printf("after recv\n");
        type = recv_header.type;
        code = recv_header.code;
        data_len = ntohs(recv_header.length);
        
        printf("type: %x",type);

        ftp_pay = (uint8_t*)malloc(sizeof(uint8_t)*data_len);
        recv(s, &ftp_pay, data_len, 0);
        memcpy(buf, (char *)ftp_pay, data_len);
        printf("%s",buf);

        do{
            switch(type){
  //            case 0x05:
    //              exe_retr(s, buf);
      //            break;
                case 0x06:
                case 0x20:
                    exe_str(code, buf);
                    break;
                default:
                    break;
            }
        }while(code == 0x01);
    }
}

/*void exe_retr(int s, char *buf){
    for(;;){
        do{
            fp = fopen(buf, "a");
            fwrite
        data_len -= BUFSIZE;
        }while(s.data_len<0);
        if(s.code == 0x01){
            break;
        }
    }
}
*/

void exe_str(uint8_t code, char *buf){
    FILE *fp;
    for(;;){
        printf("%s",buf);
        fp = fopen(buf, "w");
        fwrite(buf, sizeof(char), BUFSIZE, fp);
        fclose(fp);
    }
}

