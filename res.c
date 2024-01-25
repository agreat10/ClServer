#include <stdio.h>
#include <WinSock2.h>
#pragma comment (lib, "ws2_32.lib")
int is_big_endian()
{
    unsigned short test_value = 0x0001;
    char *ptr = (char *)&test_value;
    if(ptr[0] == 0) return 1;
    else return 0;
}
void h_to_n(void *p, int k)
{
    if(is_big_endian()) return;
    unsigned char * a = p;
    int r;
    for (size_t i = 0; i < k/2; i++)
    {
        r=a[i];
        a[i] = a[k-1-i];
        a[k-1-i] = r;
    }    
}

void n_to_h(void *p, int k){h_to_n(p, k);}


int main()
{
    WSADATA wsa_data;
    unsigned short wsa_version = MAKEWORD(2,2);
    WSAStartup(wsa_version, &wsa_data);
    //192.168.0.108
    struct in_addr ip;
    printf("%.2x %.2x %.2x %.2x\n", 192, 168, 0, 108);
    ip.S_un.S_addr = 0xc0a8006c;
    h_to_n(&ip, 4);
    unsigned short port = 8000;
    h_to_n(&port, 2);
    
    struct sockaddr_in addr_in;
    memset(&addr_in, 0, sizeof(struct sockaddr_in));
    addr_in.sin_addr = ip;
    addr_in.sin_port = port;
    addr_in.sin_family = AF_INET;

    struct sockaddr * addr = (struct sockaddr *)&addr_in;

    struct sockaddr remote_addr;
    int remote_addrlen;
    struct sockaddr_in * remote_addr_in = (struct sockaddr_in *)&remote_addr;

// создание дескриптора сокета для принятия соединения
    int socket_base = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    int bind_status = bind(socket_base, addr, sizeof(struct sockaddr));
    printf("bind_status: %d\n", bind_status);
    listen(socket_base, SOMAXCONN);
    // создание дескриптора сокета, по которому будет передаваться информация
    int sockfd = accept(socket_base, &remote_addr, &remote_addrlen);
    //сохранение удаленного ip
    unsigned char * remote_ip = (unsigned char *)&(remote_addr_in->sin_addr);
    //сохранение удаленного порта
    unsigned short remote_port = remote_addr_in->sin_port;

    printf("remote ip: %u.%u.%u.%u\n", remote_ip[0], remote_ip[0], remote_ip[0], remote_ip[0]);
    n_to_h(&remote_port, 2);
    printf("remote port: %hu\n", remote_port);

    //создание массива для принятия данных
    char msg[100];
    //считывание сообшения из сокета
    recv(sockfd, msg, 100,0);
    printf("Received message: %s/n", msg);

    closesocket(socket_base);
    closesocket(sockfd);
    WSACleanup();

    system("pause");
    return 0;
}