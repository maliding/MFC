#include <WinSock2.h>
#include <iostream>
#include <WS2tcpip.h>
#pragma warning(disable : 4996)
using namespace std;
#pragma comment(lib,"Ws2_32.lib")
int main()
{
    WORD V_version = MAKEWORD(2, 2);
    WSADATA wsaData;
    if (int result = WSAStartup(V_version, &wsaData) == 0) {
        cout << "成功加载WinSock动态链接库" << endl;
        cout << "Winsock库版本：" << wsaData.wVersion << "." << wsaData.wHighVersion << endl;
        cout << "描述：" << wsaData.szDescription << endl;
        cout << "系统状态：" << wsaData.szSystemStatus << endl;
    }
    else 
    {
        cout << "无法加载winSocket动态链接库" << endl;
    }

    SOCKET sv = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sv == INVALID_SOCKET) {
        cout << "创建失败" << endl;
    }

    sockaddr_in ADD;
    ADD.sin_family = AF_INET;
    ADD.sin_port = htons(5000);
    ADD.sin_addr.S_un.S_addr = inet_addr("0.0.0.0");

    bind(sv, (sockaddr*)&ADD, sizeof(sockaddr));

    char send_date[100] = "\0";
    char rev_date[100] = "\0";
    char temp_date[100] = "\0";


    int  len = sizeof(SOCKADDR);

    struct DIG
    {
        char line[100];
        char person[100];
        char bot[100];
    };

    DIG dig[1000];
    int count = 0;
    char sorry[100] = "不好意思，无法理解您的意思，尽情期待升级版！";
    
  /*  FILE* fp;
    fopen_s(&fp,"F:\keshe\ma1\chat_date.txt", "w");
    if (fp==NULL) {
        cout << "打开失败" << endl;
    }
    
    */
    while (true)
    {


        int i = recvfrom(sv, rev_date, 100, 0, (sockaddr*)&ADD, &len);
        if (i > 0)
        {   
            cout << rev_date << endl;
            //fputs(rev_date, fp);
        }
        else {
            cout << "接收报文失败" << endl;
            return 0;
        }

        bool B = false;
        FILE* FP = fopen("\\keshe\\ma1\\date.txt", "r");    //创建聊天文件进行检索
        while (!feof(FP))
        {

            fgets(dig[count].line, sizeof(dig[count].line), FP);
            int t=fscanf(FP, "%s%s", dig[count].person, dig[count].bot);    
            if (t == EOF) {
                cout << "读取失败" << endl;
               // return -1;
            }
            if (strcmp(rev_date, dig[count].person) == 0)
            {
                strcpy(send_date, dig[count].bot);
                B = true;

                break;
            }

        }
        fclose(FP);
        if (!B) strcpy(send_date, sorry);

        int k = sendto(sv, send_date, 100, 0, (sockaddr*)&ADD, len);
        if (k < 0) {
            cout << "发送失败" << endl;
        }

    }
    closesocket(sv);
    WSACleanup();
   // fclose(fp);
    return 0;
}