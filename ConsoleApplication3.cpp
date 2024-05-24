// ConsoleApplication3.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#define WIN32_LEAN_AND_MEAN
#include  <Windows.h>
#include <WS2tcpip.h>
#include <Winsock2.h>
#include <iostream>

using namespace std;
int main()

{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    WSADATA wsaData;                                   // устанавливаем русский язык                        
    ADDRINFO hints;                                   // устанавливаем русский язык
    ADDRINFO* addrResult;                            // харинт в себе информацию о том, как взаимодействовать с сокетами 
    const char* sendBuffer = "\nHello from Server"; // сообщение, выводимое на сервере

    SOCKET ConnectSocket = INVALID_SOCKET;         // при создании сокета даётся значение INVALID_SOCKET. SOCKET, в сути, это ENUM, хранящий значения 1,2,3,4,5, где каждое имеет свой функцилнал. INVALID_SOCEKT - это -1, сделано для того, чтобы не было магического числа
    SOCKET ListenSocket = INVALID_SOCKET;         // при создании сокета даётся значение INVALID_SOCKET. SOCKET, в сути, это ENUM, хранящий значения 1,2,3,4,5, где каждое имеет свой функцилнал. INVALID_SOCEKT - это -1, сделано для того, чтобы не было магического числа

    ZeroMemory(&hints, sizeof(hints));          // Обнуляет память, принимает в себя 2 параметра, где первый - это то, с какого момента чистить память, второй - на какое количество байт мы обнуляем
    hints.ai_family = AF_INET;                  // указание семейства адресов IPv4
    hints.ai_socktype = SOCK_STREAM;            // используется для указания потока
    hints.ai_protocol = IPPROTO_TCP;            // указание протокола TCP.TCP - протокол, позволяющий двум пользователям сети обмениваться данными
    hints.ai_flags = AI_PASSIVE;                // указание флага. Данный флаг указывает, что структура

    char recvBuffer[512];
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        cout << "беда" << endl;
        return 1;
    }


    result = getaddrinfo(NULL, "666", &hints, &addrResult);
    if (result != 0) {
        cout << "беда 2" << endl;
        freeaddrinfo(addrResult);
        WSACleanup();
        return 1;
    }
    ListenSocket = socket(addrResult->ai_family, addrResult->ai_socktype, addrResult->ai_protocol);
    if (ListenSocket == INVALID_SOCKET) {
        cout << "Сокет не создан" << endl;
        freeaddrinfo(addrResult);
        WSACleanup();
        return 1;
    }

    result = bind(ListenSocket, addrResult->ai_addr, addrResult->ai_addrlen);
    if (result == SOCKET_ERROR) {
        cout << "binding error to server <3" << endl;
        closesocket(ListenSocket);
        ListenSocket = INVALID_SOCKET;
        freeaddrinfo(addrResult);
        return 1;
    }

    result = listen(ListenSocket,SOMAXCONN);
    if (result == SOCKET_ERROR) {
        cout << "Error to listen <3" << endl;
        closesocket(ListenSocket);
        ListenSocket = INVALID_SOCKET;
        freeaddrinfo(addrResult);
        return 1;
    }

    ConnectSocket = accept(ListenSocket, NULL, NULL);
    if (result == SOCKET_ERROR) {
        cout << "accepting socket failed <3" << endl;
        WSACleanup();
        return 1;
    }

    closesocket(ListenSocket);


    
    do {
        ZeroMemory(recvBuffer, 512, 0);
        result = recv(ConnectSocket,recvBuffer, 512, 0);
        if (result > 0) {

            cout << "received" << result << "bytes" << endl;
            cout << "received data" << recvBuffer<< endl;

            result = send(ConnectSocket, sendBuffer, (int)strlen(sendBuffer), 0);

            if (result == SOCKET_ERROR) {
                cout << "send failde error" << endl;
                freeaddrinfo(addrResult);
                WSACleanup();
                return 1;
            }

        }
        else if (result == 0) {
            cout << "Connection closed" << endl;
        }
        else {
            cout << "received with ERRROR" << endl;
        }

    } while (result > 0);

    result = shutdown(ConnectSocket, SD_SEND);
    if (result == SOCKET_ERROR) {
        cout << "shutdown failde error" << endl;
        freeaddrinfo(addrResult);
        WSACleanup();
        return 1;
    }
    closesocket(ListenSocket);
    freeaddrinfo(addrResult);
    WSACleanup();
    return 1;
}

