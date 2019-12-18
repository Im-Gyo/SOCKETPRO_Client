#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>
#include <conio.h>	// �ܼ� ����� ����


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

	printf("���� �ּ� : ");
	gets(sAddr, BUFSIZE);
	printf("��Ʈ �ѹ� : ");
	gets_s(message, BUFSIZE);
	sPort = atoi(message);

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("Winsock 2.2 DLL �ε� ����");

	clntSock = socket(AF_INET, SOCK_STREAM, 0);
	if (clntSock == INVALID_SOCKET)
		ErrorHandling("���� ����");

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(sAddr);
	servAddr.sin_port = htons(sPort);

	if (connect(clntSock, (void*)& servAddr, sizeof(servAddr)) == SOCKET_ERROR) {
		ErrorHandling("���� ����");
	}
	else {
		printf("���� ����!\n����...\n");
	}
	// ������ ������ ��û�ϱ� ���� �Լ�
	// connect(���� ��ũ����, ������ ���� �ּ�������, �����Ͱ� ����Ű�� ����ü ũ��)
	while (1) {
		printf("\n���� �޼��� : ");
		gets_s(message, BUFSIZE);

		if (strcmp(message, "exit") == 0) {
			send(clntSock, message, (int)strlen(message), 0);
			break;
		}

		send(clntSock, message, (int)strlen(message), 0);
		printf("�޼��� ����...\n");

		nRcv = recv(clntSock, message, sizeof(message) - 1, 0);
		if (nRcv == SOCKET_ERROR) {
			printf("���� ����\n");
			break;
		}
		message[nRcv] = '\0';

		if (strcmp(message, "exit") == 0) {
			printf("���� ������ �ݽ��ϴ�.\n");
			break;
		}

		printf("���� �޼��� : %s", message);
	}
	closesocket(clntSock);
	WSACleanup();
	printf("���� ����.\n");
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