#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>
#include <conio.h>	// 콘솔 입출력 제공


#define BUFSIZE 1024
#define _WINSOCK_DEPRECATED_NO_WARNINGS
//#define _CRT_SECURE_NO_WARNINGS

void ErrorHandling(char* message);

int main(int argc, char** argv) {

	SOCKET clntSock;
	WSADATA wsaData;
	SOCKADDR_IN servAddr;

	char sAddr[15];
	int sPort;
	int nRcv;
	unsigned int Addr;
	char message[BUFSIZE];

	struct hostent* host;

	printf("서버 주소 : ");
	gets(sAddr, BUFSIZE);
	printf("포트 넘버 : ");
	gets_s(message, BUFSIZE);
	sPort = atoi(message);

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("Winsock 2.2 DLL 로드 에러");

	clntSock = socket(AF_INET, SOCK_STREAM, 0);
	if (clntSock == INVALID_SOCKET)
		ErrorHandling("소켓 에러");

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(sAddr);
	servAddr.sin_port = htons(sPort);

	if (connect(clntSock, (void*)& servAddr, sizeof(servAddr)) == SOCKET_ERROR) {
		ErrorHandling("연결 에러");
	}
	else {
		printf("연걸 성공!\n시작...\n");
	}
	// 서버에 접속을 요청하기 위한 함수
	// connect(소켓 디스크립터, 접속할 서버 주소포인터, 포인터가 가리키는 구조체 크기)
	while (1) {
		printf("\n보낼 메세지 : ");
		gets_s(message, BUFSIZE);

		if (strcmp(message, "exit") == 0) {
			send(clntSock, message, (int)strlen(message), 0);
			break;
		}

		send(clntSock, message, (int)strlen(message), 0);
		printf("메세지 수신...\n");

		nRcv = recv(clntSock, message, sizeof(message) - 1, 0);
		if (nRcv == SOCKET_ERROR) {
			printf("수신 에러\n");
			break;
		}
		message[nRcv] = '\0';

		if (strcmp(message, "exit") == 0) {
			printf("서버 연결을 닫습니다.\n");
			break;
		}

		printf("수신 메세지 : %s", message);
	}
	closesocket(clntSock);
	WSACleanup();
	printf("연결 종료.\n");
	_getch();

	return 0;
}

void ErrorHandling(char* message) {
	WSACleanup();
	fputs(message, stderr);
	fputc('\n', stderr);
	_getch();
	exit(1);
}