#include<stdio.h>
#include <WinSock2.h>

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

int main()
{
    WSADATA wsa_data;
    unsigned short wsa_version = MAKEWORD(2,2);
    WSAStartup(wsa_version, &wsa_data);
      //192.168.0.108
    /* struct in_addr ip;
    printf("%.2x %.2x %.2x %.2x\n", 192, 168, 0, 108);
    ip.S_un.S_addr = 0xc0a8006c;
    h_to_n(&ip, 4);
    unsigned short port = 7777;
    h_to_n(&port, 2);

    struct sockaddr_in addr_in;
    memset(&addr_in, 0, sizeof(struct sockaddr_in));
    addr_in.sin_addr = ip;
    addr_in.sin_port = port;
    addr_in.sin_family = AF_INET;
    struct sockaddr * addr = (struct sockaddr *)&addr_in; */

    struct in_addr remote_ip;
    remote_ip.S_un.S_addr = 0xc0a8006c;
    h_to_n(&remote_ip, 4);
    unsigned short remote_port = 8000;
    h_to_n(&remote_port, 2);

    struct sockaddr_in remote_addr_in;
    memset(&remote_addr_in, 0, sizeof(struct sockaddr_in));
    remote_addr_in.sin_addr = remote_ip;
    remote_addr_in.sin_port = remote_port;
    remote_addr_in.sin_family = AF_INET;
    struct sockaddr * remote_addr = (struct sockaddr *)&remote_addr_in;

    int sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    //bind(sockfd, addr, sizeof(struct sockaddr));
    connect(sockfd, remote_addr, sizeof(struct sockaddr));

    char msg[100];
    printf("Input message: ");
    scanf("%[^\n]", msg);
    int len = strlen(msg);
    send(sockfd, msg, len, 0);
    closesocket(sockfd);
    WSACleanup();
    return 0;
}