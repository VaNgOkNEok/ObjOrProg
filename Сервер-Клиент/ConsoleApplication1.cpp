#pragma comment(lib, "Ws2_32.lib")
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <string>
#include <cstring>
#include <vector>
#include <mutex>
#include <thread>

using namespace std;

/*Часть сервера*/
struct ClientInfo {
    SOCKET sock;
    std::string name;
};

std::vector<ClientInfo> clients; // Список всех клиентов
std::mutex clientsMutex; // Мьютекс для синхронизации доступа к списку клиентов

// Функция для отправки сообщения всем клиентам
void broadcastMessage(const std::string& message, SOCKET senderSock = INVALID_SOCKET)
{
    std::lock_guard<std::mutex> guard(clientsMutex); // Заблокировать доступ к списку клиентов
    for (const ClientInfo& client : clients)
    {
        if (client.sock != senderSock) // Не отправляем сообщение обратно отправителю
        {
            send(client.sock, message.c_str(), message.length(), 0);
        }
    }
}

// Оповещение о подключении нового клиента
void notifyClientsOfNewConnection(const std::string& name, const char* ip, int port, SOCKET senderSock = INVALID_SOCKET)
{
    std::lock_guard<std::mutex> guard(clientsMutex); // Заблокировать доступ к списку клиентов
    string message = "New client connected: Name=" + name + ", IP=" + ip + ", Port=" + to_string(port) + '\n';
    for (const ClientInfo& client : clients)
    {
        if (client.sock != senderSock) // Не отправляем сообщение обратно отправителю
        {
            send(client.sock, message.c_str(), message.length(), 0);
        }
    }
}

// Оповещение о отключении клиента
void notifyClientsOfDisconnection(const std::string& name, const char* ip, int port)
{
    std::ostringstream oss;
    oss << "Client disconnected: Name=" << name << ", IP=" << ip << ", Port=" << port << endl;

    std::string notification = oss.str();
    broadcastMessage(notification); // Оповещаем всех клиентов
}

bool work_with_client(SOCKADDR_IN from, char ipStr[], int retVal, SOCKET clientSock)
{
    inet_ntop(AF_INET, &from.sin_addr, ipStr, INET_ADDRSTRLEN); // Получаем строковое представление IP-адреса клиента
    int clientPort = ntohs(from.sin_port); // Получаем порт клиента
    printf("New connection accepted from %s, port %d\n", ipStr, clientPort);

    retVal = send(clientSock, "Enter your name: ", 17, 0);
    char szReq[256] = { 0 }; // Инициализация массива нулями
    if (retVal == SOCKET_ERROR || retVal == 0) // Проверка на ошибки и завершение соединения
    {
        printf("Unable to receive data\n");
        closesocket(clientSock);
        return 0; // Пропустить обработку этого клиента и продолжить
    }

    retVal = recv(clientSock, szReq, sizeof(szReq) - 1, 0); // Принимаем имя клиента
    if (retVal == SOCKET_ERROR || retVal == 0) // Проверка на ошибки и завершение соединения
    {
        printf("Unable to receive data\n");
        closesocket(clientSock);
        return 0; // Пропустить обработку этого клиента и продолжить
    }

    std::string clientName = szReq;
    printf("Client name: %s\n", clientName.c_str());

    // Оповещаем всех клиентов о подключении нового клиента
    notifyClientsOfNewConnection(clientName, ipStr, clientPort);

    // Добавляем клиента с его именем в список
    ClientInfo newClient = { clientSock, clientName };
    {
        std::lock_guard<std::mutex> guard(clientsMutex); // Блокировка для добавления клиента
        clients.push_back(newClient);
    }

    // Теперь ожидаем сообщения от клиента
    while (true)
    {
        char szReq[256] = { 0 };
        retVal = recv(clientSock, szReq, sizeof(szReq) - 1, 0); // Принимаем данные от клиента
        if (retVal == SOCKET_ERROR || retVal == 0) // Проверка на ошибки и завершение соединения
        {
            printf("Unable to receive data or connection closed\n");
            closesocket(clientSock);
            break;
        }

        std::string message = szReq;
        printf("%s: %s\n", clientName.c_str(), message.c_str()); // Локально выводим сообщение с именем// Проверяем команду на выключение сервера
        if (message[0] == 's')
        {
            const char* szResp = "Server shutdown";
            send(clientSock, szResp, strlen(szResp), 0);
            closesocket(clientSock);
            return 1; // Выход из цикла
        }

        // Отправляем сообщение всем клиентам, кроме отправителя
        broadcastMessage(clientName + ": " + message + '\n', clientSock);
    }

    // Оповещаем всех клиентов об отключении
    notifyClientsOfDisconnection(clientName, ipStr, clientPort);

    // Удаляем клиента из списка при отключении
    {
        std::lock_guard<std::mutex> guard(clientsMutex);
        clients.erase(std::remove_if(clients.begin(), clients.end(),
            [clientSock](const ClientInfo& client) {
                return client.sock == clientSock;
            }), clients.end());
    }

    return 1;
}

int main(void)
{
    WORD sockVer;
    WSADATA wsaData;
    int retVal;
    sockVer = MAKEWORD(2, 2);

    // Проверка на успешную инициализацию
    if (WSAStartup(sockVer, &wsaData) != 0)
    {
        printf("WSAStartup failed\n");
        return 1;
    }

    // Создаем сокет
    SOCKET servSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (servSock == INVALID_SOCKET)
    {
        printf("Unable to create socket\n");
        WSACleanup();
        return 1;
    }

    SOCKADDR_IN sin;
    sin.sin_family = AF_INET;
    sin.sin_port = htons(2002);
    sin.sin_addr.s_addr = INADDR_ANY;

    // Привязываем сокет
    retVal = bind(servSock, (LPSOCKADDR)&sin, sizeof(sin));
    if (retVal == SOCKET_ERROR)
    {
        printf("Unable to bind\n");
        closesocket(servSock);
        WSACleanup();
        return 1;
    }

    char ipStr[INET_ADDRSTRLEN]; // Буфер для хранения IP-адреса
    inet_ntop(AF_INET, &sin.sin_addr, ipStr, sizeof(ipStr)); // Используем inet_ntop
    printf("Server started at %s, port %d\n", ipStr, ntohs(sin.sin_port));

    // Начинаем слушать
    if (listen(servSock, SOMAXCONN) == SOCKET_ERROR) // Используйте SOMAXCONN для максимального числа ожиданий
    {
        printf("Unable to listen\n");
        closesocket(servSock);
        WSACleanup();
        return 1;
    }

    while (true)
    {
        // Ждем клиента
        SOCKET clientSock;
        SOCKADDR_IN from;
        int fromlen = sizeof(from);
        clientSock = accept(servSock, (struct sockaddr*)&from, &fromlen);
        if (clientSock == INVALID_SOCKET)
        {
            printf("Unable to accept\n");
            closesocket(servSock);
            WSACleanup();
            return 1;
        }


        std::thread clientThread([&from, ipStr, clientSock]()
            {
                int retVal = 0;
                if (!work_with_client(from, (char*)ipStr, retVal, clientSock))
                {
                    printf("Error while working with client\n");
                }

                closesocket(clientSock); // Закрываем сокет после завершения работы с клиентом
                printf("Connection closed\n");
            });

        // Отсоединяем поток, чтобы он работал независимо
        clientThread.detach();

    }

    // Закрываем сокет
    closesocket(servSock);
    WSACleanup();
    return 0;
}


/*Часть клиента*/
bool send(SOCKET s_client) {
    int retVal;
    string text;
    char pBuf[256];
    while(true)
    {
        getline(cin, text);
        retVal = send(s_client, text.c_str(), text.length(), 0);
        if (retVal == SOCKET_ERROR) {
            std::cerr << "Send failed" << std::endl;
            return false;
        }
        if (text == "s") break;
    }
    return true;
}

bool get(SOCKET s_client) {
    int bytesReceived;
    char pBuf[256];
    while (true)
    {
        bytesReceived = recv(s_client, pBuf, sizeof(pBuf), 0);
        if (bytesReceived == SOCKET_ERROR) {
            std::cerr << "Recv failed" << std::endl;
            return false;
        }
        else {
            int len = bytesReceived;
            for (int i = 0; i < len; i++) {
                std::cout << pBuf[i];
            }
        }
    }
}

void startClient() {
    const int queuesize = 10;
    SOCKADDR_IN serverInfo;
    SOCKET s_client;   // Удалил s_serv, он не используется
    char pBuf[256];
    int retVal, bytesReceived;

    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed" << std::endl;
        return;
    }

    s_client = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (s_client == INVALID_SOCKET) {
        std::cerr << "Socket creation failed" << std::endl;
        WSACleanup();
        return;
    }

    std::string ip;
    std::cout << "ip>";
    std::cin >> ip;
    std::cin.ignore();

    int port;
    std::cout << "port>";
    std::cin >> port;
    std::cin.ignore();

    serverInfo.sin_family = PF_INET;
    inet_pton(AF_INET, ip.c_str(), &serverInfo.sin_addr);
    serverInfo.sin_port = htons(port);

    retVal = connect(s_client, (LPSOCKADDR)&serverInfo, sizeof(serverInfo));
    if (retVal == SOCKET_ERROR) {
        std::cerr << "Connect failed" << std::endl;  // Incorrect error message
        closesocket(s_client);
        WSACleanup();
        return;
    }

    std::thread clientThread([s_client]()
        {
            int retVal = 0;
            if (!get(s_client))
            {
                printf("Error while working with client\n");
            }

        }
    );

    if (!send(s_client))
    {     
        printf("Error while working with client\n");
    }

    clientThread.detach();
    closesocket(s_client);  // Закрываем сокет
    WSACleanup();           // Освобождаем Winsock
}