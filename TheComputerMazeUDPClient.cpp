// TheComputerMazeUDPClient.cpp : Defines the entry point for the console application.
//
//my includes
#include <stdio.h>     
#include <stdlib.h>
#include <string.h>
//#include <iostream>

//using namespace std;
//library includes
#include "stdafx.h"
#include <winsock2.h>
#include <time.h>


#pragma comment(lib, "wsock32.lib")

#define STUDENT_NUMBER		"17024721"
#define STUDENT_FIRSTNAME	"Robert"
#define STUDENT_FAMILYNAME	"Painter"

#define IP_ADDRESS_SERVER	"127.0.0.1"
//#define IP_ADDRESS_SERVER "164.11.80.69"


#define PORT_SERVER 0x1984 // We define a port that we are going to use.
#define PORT_CLIENT 0x1985 // We define a port that we are going to use.

#define MAX_FILENAME_SIZE 500

#define MAX_BUFFER_SIZE   5000
#define MAX_STRING_SIZE   200
#define MAX_NO_TOKENS     50

#define MAX_ITEMS_IN_ROOM		20
#define MAX_ITEMS_IN_BACKPACK	50


//my defs
#define OPTION_MOVE_NORTH	1
#define OPTION_MOVE_SOUTH	2
#define OPTION_MOVE_EAST	3
#define OPTION_MOVE_WEST	4
#define OPTION_MOVE_UP		5
#define OPTION_MOVE_DOWN	6

#define OPTION_UNLOCK_NORTH	7
#define OPTION_UNLOCK_SOUTH	8
#define OPTION_UNLOCK_EAST	9
#define OPTION_UNLOCK_WEST	10

#define OPTION_BASE_FOR_READS	200
#define OPTION_BASE_FOR_PICKUPS	500
#define OPTION_BASE_FOR_DROPS	800
#define OPTION_BASE_FOR_DOS		1100
#define OPTION_BASE_FOR_EVENTS	1300
//end

// my enums
enum typeofroom
{
	ROOM_NONE = 0,
	ROOM_LECTURE_SMALL = 1,
	ROOM_LECTURE_MEDIUM = 2,
	ROOM_LECTURE_LARGE = 3,
	ROOM_CORRIDOR = 4,
	ROOM_LAB_SMALL = 5,
	ROOM_LAB_MEDIUM = 6,
	ROOM_LAB_LARGE = 7,
	ROOM_MEETING_ROOM = 8,
	ROOM_SEMINAR = 9,
	ROOM_HIVE = 10, //one per floor
	ROOM_COFFEESHOP = 11, //one
	ROOM_LIBRARY = 12, //only one
	ROOM_SHOP_SELL = 13,
	ROOM_SHOP_BUY = 14,
	ROOM_SHOP_BUYSELL = 15,
	ROOM_OFFICE = 16, //maybe only one door
	ROOM_LOBBY = 17, //Only one
	ROOM_EXIT = 18, //only one
	ROOM_STAIRS = 19,
	ROOM_ENTRANCE = 20 //only one
};


//end
enum directions
{
	DIRECTION_NORTH = 0,
	DIRECTION_SOUTH = 1,
	DIRECTION_EAST = 2,
	DIRECTION_WEST = 3,
	DIRECTION_UP = 4,
	DIRECTION_DOWN = 5
};


enum direction_possible
{
	DIRECTION_NOT_PRESENT = -1,
	DIRECTION_LOCKED = 0,
	DIRECTION_OPEN = 1
};


enum item_types
{
	ITEM_NONE = 0,
	ITEM_BOOK = 1,
	ITEM_JUNK = 2,
	ITEM_EQUIPMENT = 3,
	ITEM_MANUSCRIPT = 4,
	ITEM_TEST = 5,
	ITEM_OTHER = 10
};



struct Item
{
	int  number;
	int  value;
	int  volume;
};


struct Student
{
	int level;
	int rooms_visited;
	int doors_openned;
	int number_of_moves;
	int score;
};


struct Room
{
	char name[5];
	int  type;
	int  direction[6];
	int  number_of_keys;
	int  keys[4];
	int  number_of_items;
	Item items[MAX_ITEMS_IN_ROOM];
};


struct Backpack
{
	int number_of_items;
	Item items[MAX_ITEMS_IN_BACKPACK];
};

// my structs
struct Xkey {//for storing key data about where to use
	WORD key;
	char loc[5];
	int dir;

};
//end




#define MAX_OPTIONS	50

int number_of_options;
int options[MAX_OPTIONS];


Student student;
Room room;
Backpack backpack;



SOCKADDR_IN server_addr;
SOCKADDR_IN client_addr;

SOCKET sock;  // This is our socket, it is the handle to the IO address to read/write packets

WSADATA data;




char InputBuffer[MAX_BUFFER_SIZE];

char Tokens[MAX_NO_TOKENS][MAX_STRING_SIZE];

char text_student[1000];
char text_backpack[1000];
char text_room[1000];
char text_keys[1000];
char text_items[1000];
char text_options[1000];



void sentOption(int option, int key)
{
	char buffer[100];

	sprintf(buffer, "Option %d, %x", option, key);
	sendto(sock, buffer, strlen(buffer), 0, (SOCKADDR *)&server_addr, sizeof(SOCKADDR));
}


/*************************************************************/
/********* Your tactics code starts here *********************/
/*************************************************************/

int option_count = 0;
char room_name[10] = " ";

WORD Known_keys[255];//known keys 
int keypointer = -1;

WORD keys[] = {0x0000};

//srand((unsigned)time(&t));
//srand(time(NULL));
//int MAX_RAND = 3

//random movement
int getrandomdir() {
	return (rand() % 6);//get random direction for each way
}
int tryrandommove() {
	return 0;
}

int isitem() {
	return 0;
}
bool is_locked(int roomid) {
	if (roomid == -1) {
		return true;
	}
	return false;
}
WORD bruteforce_result() {
	WORD key =	0x0000;
	WORD keyM =	0xFFFF;
	for (int i=0; i>=keyM; i++) {
		return 1;
	}
	return -1;
}

//key stuff
bool add_key_tokeys(WORD key) {//adds key to known keys if non duplicate
	for (int i=0; i<keypointer; i++) {//iterate through then if not in keys, add to end
		if (Known_keys[i] == key) {
			return false;
		}
	}
	keypointer++;
	Known_keys[keypointer] = key;
	return true;
	//return false;
}
//
void pickup_keys_in_room() {//picks up keys if there are any in room
	printf("keys: %X\n", room.number_of_keys);
	if (room.number_of_keys != 0) {
		for (int i = 0; i < room.number_of_keys; i++) {
			printf("key_pickup %X\n", room.keys[i]);
			add_key_tokeys(room.keys[i]);
		}
		printf("\n");
	}
}

//pickup key
//pickup items in room
bool add_item_topack(Item item) {//adds key to known keys if non duplicate
	if (backpack.number_of_items >= MAX_ITEMS_IN_BACKPACK) {
		printf("failed!");
		return false;
	}
	sentOption(OPTION_BASE_FOR_PICKUPS + item.number,0);//add item as a move
	/*
	backpack.number_of_items++;
	backpack.items[backpack.number_of_items] = item;
	*/
	return true;
	//return false;
}
//
void pickup_items_in_room() {//picks up keys if there are any in room
	printf("items: %X\n", room.number_of_items);
	if (room.number_of_items != 0) {
		for (int i = 0; i < room.number_of_items; i++) {
			printf("item_pickup %X\n", room.items[i].number);
			//add_key_tokeys(room.keys[i]);
			// find what it is then do things with it if it is a good item
			add_item_topack(room.items[i]);
		}
		printf("\n");
	}
}
//invsort for best
bool sort_inv() {//sort for highest value and lowest volume
	//
	int value[MAX_ITEMS_IN_BACKPACK];
	//sort to max

	//
	return false;
}




//move code
void yourMove()
{
	printf("=========\n");
	pickup_keys_in_room();
	pickup_items_in_room();
	printf("=========\n");

	//if (strcmp(room_name, room.name) != 0) option_count = 0;

	//sentOption(options[option_count], 0x1234);

	//option_count = (option_count + 1) % number_of_options;
	//option_count = OPTION_MOVE_NORTH;;
	char x[255];
	for (int i = 0; i < 255; i++) {
		x[i] = '\0';
	}
	//String x;
	//cin >> x;
	scanf(" %c", &x); // Notice the whitespace in the format string
	printf("-%s-", x);
	switch (x[0])
	{
	case'w'://n
		option_count = OPTION_MOVE_NORTH;
		break;
	case's'://s
		option_count = OPTION_MOVE_SOUTH;
		break;
	case'a'://e
		option_count = OPTION_MOVE_EAST;
		break;
	case'd'://w
		option_count = OPTION_MOVE_WEST;
		break;
	case'q'://d
		option_count = OPTION_MOVE_DOWN;
		break;
	case'e'://u
		option_count = OPTION_MOVE_UP;
		break;
	default:
		printf("IV ix");//invalid instruction
		break;
	}
	sentOption(option_count, 0x1234);
}


/*************************************************************/
/********* Your tactics code ends here ***********************/
/*************************************************************/



int getTokens(char* instring, char seperator)
{
	int  number_of_tokens;
	char chr;
	int  state;
	int  i;
	int  j;


	for (i = 0; i < MAX_NO_TOKENS; i++)
	{
		for (j = 0; j < MAX_STRING_SIZE; j++)
		{
			Tokens[i][j] = '\0';
		}
	}

	number_of_tokens = -1;
	chr = instring[0];
	state = 0;
	i = 0;

	while (chr != '\0')
	{
		switch (state)
		{
		case 0:  // Initial state
			if (chr == seperator)
			{
				number_of_tokens++;
				state = 1;
			}
			else if ((chr == ' ') || (chr == '\t') || (chr == '\n'))
			{
				state = 1;
			}
			else
			{
				number_of_tokens++;
				j = 0;
				Tokens[number_of_tokens][j] = chr;
				Tokens[number_of_tokens][j + 1] = '\0';
				state = 2;
			}
			break;

		case 1:  // Leading white space
			if (chr == seperator)
			{
				number_of_tokens++;
				state = 1;
			}
			else if ((chr == ' ') || (chr == '\t') || (chr == '\n'))
			{
				state = 1;
			}
			else
			{
				number_of_tokens++;
				j = 0;
				Tokens[number_of_tokens][j] = chr;
				Tokens[number_of_tokens][j + 1] = '\0';
				state = 2;
			}
			break;

		case 2:  // Collecting chars
			if (chr == seperator)
			{
				//number_of_tokens++;
				state = 1;
			}
			else
			{
				j++;
				Tokens[number_of_tokens][j] = chr;
				Tokens[number_of_tokens][j + 1] = '\0';
				state = 2;
			}
			break;

		default:
			break;
		}

		i++;
		chr = instring[i];
	}

	return (number_of_tokens + 1);
}



bool getline(FILE *fp, char *buffer)
{
	bool rc;
	bool collect;
	char c;
	int  i;

	rc = false;
	collect = true;

	i = 0;
	while (collect)
	{
		c = getc(fp);

		switch (c)
		{
		case EOF:
			if (i > 0)
			{
				rc = true;
			}
			collect = false;
			break;

		case '\n':
			if (i > 0)
			{
				rc = true;
				collect = false;
				buffer[i] = '\0';
			}
			break;

		default:
			buffer[i] = c;
			i++;
			break;
		}
	}

	return (rc);
}



void printRoom()
{
	int i;

	printf("Room\n");
	printf("Room = %s, Room type = %d\n", room.name, room.type);

	printf("Directions = ");
	for (i = 0; i < 6; i++)
	{
		printf("%d  ", room.direction[i]);
	}
	printf("\n");

	if (room.number_of_keys > 0)
	{
		printf("Keys = ");
		for (i = 0; i < room.number_of_keys; i++)
		{
			printf("0x%X  ", room.keys[i]);
		}
		printf("\n");
	}
	else
	{
		printf("No keys in this room\n");
	}

	if (room.number_of_items > 0)
	{
		for (i = 0; i < room.number_of_items; i++)
		{
			printf("Item=%d, Value=%d, Volume=%d\n", room.items[i].number, room.items[i].value, room.items[i].volume);
		}
	}
	else
	{
		printf("No items in this room\n");
	}

	printf("\n");
}


void printStudent()
{
	printf("Student\n");
	printf("Level=%d,  Number of rooms visited = %d,  Number of doors openned = %d,  Number of moves = %d,  Score = %d\n", student.level, student.rooms_visited, student.doors_openned, student.number_of_moves, student.score);
	printf("\n");
}


void printBackpack()
{
	int i;

	printf("Backpack\n");

	if (backpack.number_of_items > 0)
	{
		for (i = 0; i < backpack.number_of_items; i++)
		{
			printf("Item=%d, Value=%d, Volume=%d\n", backpack.items[i].number, backpack.items[i].value, backpack.items[i].volume);
		}
	}
	else
	{
		printf("Your backpack is empty\n");
	}
	printf("\n");
}


void printOptions()
{
	int i;

	printf("Options\n");
	printf("Options = ");
	for (i = 0; i < number_of_options; i++)
	{
		printf("%d  ", options[i]);
	}
	printf("\n");
	printf("\n");
}




void communicate_with_server()
{
	char buffer[4096];
	int  len = sizeof(SOCKADDR);
	int  i;
	char* p;
	int	 number_of_tokens;


	sprintf_s(buffer, "Register  %s %s %s", STUDENT_NUMBER, STUDENT_FIRSTNAME, STUDENT_FAMILYNAME);
	sendto(sock, buffer, strlen(buffer), 0, (SOCKADDR *)&server_addr, sizeof(SOCKADDR));

	while (true)
	{
		memset(buffer, '\0', sizeof(buffer));

		if (recvfrom(sock, buffer, sizeof(buffer) - 1, 0, (SOCKADDR *)&client_addr, &len) != SOCKET_ERROR)
		{
			p = ::inet_ntoa(client_addr.sin_addr);

			if ((strcmp(IP_ADDRESS_SERVER, "127.0.0.1") == 0) || (strcmp(IP_ADDRESS_SERVER, p) == 0))
			{
				printf("%s\n\n", buffer);

				number_of_tokens = getTokens(buffer, '|');

				if (number_of_tokens == 6)
				{
					strcpy(text_student, Tokens[0]);
					strcpy(text_backpack, Tokens[1]);
					strcpy(text_room, Tokens[2]);
					strcpy(text_keys, Tokens[3]);
					strcpy(text_items, Tokens[4]);
					strcpy(text_options, Tokens[5]);

					printf("Student  = '%s'\n", text_student);
					printf("Backpack = '%s'\n", text_backpack);
					printf("Room     = '%s'\n", text_room);
					printf("Keys     = '%s'\n", text_keys);
					printf("Items    = '%s'\n", text_items);
					printf("Options  = '%s'\n", text_options);

					student.level = -1;
					student.rooms_visited = -1;
					student.doors_openned = -1;
					student.number_of_moves = -1;

					if (sscanf(text_student, "%d,%d,%d,%d,%d", &student.level, &student.rooms_visited, &student.doors_openned, &student.number_of_moves, &student.score) == 5)
					{
					}

					if (strlen(text_backpack) > 0)
					{
						backpack.number_of_items = getTokens(text_backpack, '&');

						if (backpack.number_of_items > 0)
						{
							for (i = 0; i < backpack.number_of_items; i++)
							{
								if (i < MAX_ITEMS_IN_BACKPACK)
								{
									backpack.items[i].number = -1;

									if (sscanf(Tokens[i], "%d, %d, %d", &backpack.items[i].number, &backpack.items[i].value, &backpack.items[i].volume) == 3)
									{
									}
								}
							}
						}
					}
					else
					{
						backpack.number_of_items = 0;
					}

					sscanf(text_room, "%s ,%d, %d, %d, %d, %d, %d, %d", &room.name, &room.type, &room.direction[DIRECTION_NORTH], &room.direction[DIRECTION_SOUTH], &room.direction[DIRECTION_EAST], &room.direction[DIRECTION_WEST], &room.direction[DIRECTION_UP], &room.direction[DIRECTION_DOWN]);

					if (strlen(text_keys) > 0)
					{
						room.number_of_keys = getTokens(text_keys, '&');

						if (room.number_of_keys > 0)
						{
							for (i = 0; i < room.number_of_keys; i++)
							{
								if (i < 4)
								{
									room.keys[i] = -1;

									if (sscanf(Tokens[i], "%x", &room.keys[i]) == 1)
									{
									}
								}
							}
						}
					}
					else
					{
						room.number_of_keys = 0;
					}

					if (strlen(text_items) > 0)
					{
						room.number_of_items = getTokens(text_items, '&');

						if (room.number_of_items > 0)
						{
							for (i = 0; i < room.number_of_items; i++)
							{
								if (i < MAX_ITEMS_IN_ROOM)
								{
									room.items[i].number = -1;

									if (sscanf(Tokens[i], "%d, %d, %d", &room.items[i].number, &room.items[i].value, &room.items[i].volume) == 3)
									{
									}
								}
							}
						}
					}
					else
					{
						room.number_of_items = 0;
					}

					if (strlen(text_options) > 0)
					{
						number_of_options = getTokens(text_options, ',');

						if (number_of_options > 0)
						{
							for (i = 0; i < number_of_options; i++)
							{
								if (i < MAX_OPTIONS)
								{
									options[i] = -1;

									if (sscanf(Tokens[i], "%d", &options[i]) == 1)
									{
									}
								}
							}
						}
					}
					else
					{
						number_of_options = 0;
					}
				}

				printStudent();
				printBackpack();
				printRoom();
				printOptions();

				//system("timeout /t 60");//commented out

				yourMove();
			}
		}
		else
		{
			printf_s("recvfrom error = %d\n", WSAGetLastError());
		}
	}

	printf_s("Student %s\n", STUDENT_NUMBER);
}




int main()
{
	char chr = '\0';

	printf("\n");
	printf("The Computer Maze Student Program\n");
	printf("UWE Computer and Network Systems Assignment 2 \n");
	printf("\n");

	if (WSAStartup(MAKEWORD(2, 2), &data) != 0) return(0);

	//sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);  // Here we create our socket, which will be a UDP socket (SOCK_DGRAM).
	//if (!sock)
	//{	
	//	printf("Socket creation failed!\n"); 
	//}

	sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);  // Here we create our socket, which will be a UDP socket (SOCK_DGRAM).
	if (!sock) 
	{
		// Creation failed! 
	}

	memset(&server_addr, 0, sizeof(SOCKADDR_IN));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(IP_ADDRESS_SERVER);
	server_addr.sin_port = htons(PORT_SERVER);

	memset(&client_addr, 0, sizeof(SOCKADDR_IN));
	client_addr.sin_family = AF_INET;
	client_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	client_addr.sin_port = htons(PORT_CLIENT);

	//int ret = bind(sock_recv, (SOCKADDR *)&receive_addr, sizeof(SOCKADDR));
	////	int ret = bind(sock_send, (SOCKADDR *)&receive_addr, sizeof(SOCKADDR));
	//if (ret)
	//{
	//	printf("Bind failed! %d\n", WSAGetLastError());
	//}

	communicate_with_server();

	closesocket(sock);
	WSACleanup();

	while (chr != '\n')
	{
		chr = getchar();
	}

	return 0;
}

