

void execute_com(int s);
void exe_recv();
void exe_str();

int main(){
    int master_sock;
    in_port_t port;
    struct sockaadr_in myskt;
    struct sockaddr_in skt;
    socklen_t sktlen;
    int pid;

    port = atoi("10001");

    master_sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&myskt, 0 sizeof myskt);
    myskt.sin_family = AF_INET;
    myskt.sin_port = htons(port);
    myskt.sin_addr.s_addr = htonl(INADDR_ANY);

    bind(master_sock, (struct sockaadr *)&myskt, sizeof myskt);
    listen(master_sock, 5);

    sktlen = sizeof skt;

    for(;;){
        client_sock = accept(master_sock, (struct sockaadr *)&skt, &sktlen);
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
    struct ftp_header *recv_header;
    uint8_t type;
    uint8_t code;
    uint16_t data_len;
    char buf[BUFSIZE];

    for(;;){
        recv(s, &recv_header, sizeof(struct ftp_header), 0);
        type = recv_header.type;
        code = recv_header.code;
        data_len = ntohs(recv_header.length);
        
        switch(type){
            case 0x05:
                exe_retr(s, buf);
                break;
            case 0x06:
                exe_str(s, buf);
                break;
            default:
                break;
        }
    }
}

void exe_retr(int s, char *buf){
    for(;;){
        do{

        data_len -= n;
        }while(data_len<0);
        if(s.code == 0x01){
            break;
        }
    }
}

void exe_str(int s, char *buf){
    
}


