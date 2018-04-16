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
	Item items[MAX_ITEMS_IN_BACKPACK];//vol = 40L
};

// my structs
struct Xkey {//for storing key data about where to use
	WORD key;
	char loc[5];
	int dir;

};
struct KeyVault {
	Xkey keys[1000];
	int numkeys = 0;
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

//WORD Known_keys[255];//known keys 
//int keypointer = -1;

WORD keys[] = {0x0000};

int Known_keys[] =
{
	0x12fc,0x49cd,0xc75f,0xd976,0xe333,
	0xab62,0xf904,0x395d,0x34c2,0x69f4,
	0x58b8,0xc4b6,0x19f4,0x222c,0x6ba3,
	0x55d5,0x7f07,0x89f3,0x88de,0xbb54,
	0x200f,0xbcd5,0xe4a8,0x71da,0x29fc,
	0xa100,0xbe27,0x9f4a,0x8e28,0xd75b,
	0x090d,0x0172,0xd2f7,0xc567,0x8fd4,
	0xfd5a,0x8f32,0x1ae1,0x0eb9,0x43ab};
int keypointer = 40;
int last_direction = DIRECTION_NORTH;
int try_key = 0;
//srand((unsigned)time(&t));
//srand(time(NULL));
//int MAX_RAND = 3

//random movement
int getrandomdir() {//gets rndom val
	return (rand() % 6);//get random direction for each way
}

int tryrandommove() {//trys for valid mv rerolls if inv
	int mvd = -2;
	mvd = getrandomdir();
	while (true){
		if (room.direction[mvd]) {
			mvd = getrandomdir();
		}
		else {
			return mvd;//movedirection
		}
	}
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
bool haskey(int key) {//chk
	for (int i = 0; i<keypointer; i++) {//iterate through then if not in keys, add to end
		if (Known_keys[i] == key) {
			return true;
		}
	}
	return true;
	//return false;
}

//pickup key
//pickup items in room
bool add_item_topack(Item item) {//adds key to known keys if non duplicate
	if (backpack.number_of_items >= MAX_ITEMS_IN_BACKPACK) {
		printf("failed!");
		return false;
	}
	printf("added:%x code:%X", item.number, OPTION_BASE_FOR_PICKUPS + item.number);
	sentOption(OPTION_BASE_FOR_PICKUPS + item.number,0);//add item as a move
	//getchar();
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
	bool tr;
	if (room.number_of_items != 0) {
		for (int i = 0; i < room.number_of_items; i++) {
			//printf("item_pickup nvv %X:%X:%X\n", room.items[i].number, room.items[i].value, room.items[i].volume);
			//add_key_tokeys(room.keys[i]);
			// find what it is then do things with it if it is a good item
			tr = add_item_topack(room.items[i]);
			printf("\nitem_pickup nvv %X:%X:%X |%X|\n", room.items[i].number, room.items[i].value, room.items[i].volume,tr);
			//getchar();
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

//event in room?
int getevents() {
	return 0;
}
//is worth
bool checkeventworth(int eventno) {
	return false;
}
//do event
void doevent(int eventno) {

}
void process_room_events() {
	int noevents = getevents();//get events
	printf("doing events :%X\n",noevents);
	for (int i = 0; i < noevents; i++) {
		if (checkeventworth(i)) {//if event worth doing do it
			doevent(i);
		}
	}
}
void randommovedo() {
	switch (tryrandommove())
	{
	case 0://north
		if ((room.direction[DIRECTION_NORTH] == DIRECTION_OPEN) && (last_direction != DIRECTION_SOUTH)) {
			sentOption(OPTION_MOVE_NORTH, 0);
			last_direction = DIRECTION_NORTH;
		}
		else if ((room.direction[DIRECTION_NORTH] == DIRECTION_LOCKED) && (last_direction != DIRECTION_SOUTH)) {
			sentOption(OPTION_UNLOCK_NORTH, Known_keys[try_key]);
			printf("ul:n %X", Known_keys[try_key]);
			try_key++;
		}
		break;
	case 1://south
		if ((room.direction[DIRECTION_SOUTH] == DIRECTION_OPEN) && (last_direction != DIRECTION_NORTH)) {
			sentOption(OPTION_MOVE_SOUTH, 0);
			last_direction = DIRECTION_SOUTH;
		}
		else if ((room.direction[DIRECTION_SOUTH] == DIRECTION_LOCKED) && (last_direction != DIRECTION_NORTH)) {
			sentOption(OPTION_UNLOCK_SOUTH, Known_keys[try_key]);
			printf("ul:s %X", Known_keys[try_key]);
			try_key++;
		}
		break;

	case 2://east
		if ((room.direction[DIRECTION_EAST] == DIRECTION_OPEN) && (last_direction != DIRECTION_WEST)) {
			sentOption(OPTION_MOVE_EAST, 0);
			last_direction = DIRECTION_EAST;
		}
		else if ((room.direction[DIRECTION_EAST] == DIRECTION_LOCKED) && (last_direction != DIRECTION_WEST)) {
			sentOption(OPTION_UNLOCK_EAST, Known_keys[try_key]);
			printf("ul:e %X", Known_keys[try_key]);
			try_key++;
		}
		break;
	case 3://west
		if ((room.direction[DIRECTION_WEST] == DIRECTION_OPEN) && (last_direction != DIRECTION_EAST)) {
			sentOption(OPTION_MOVE_WEST, 0);
			last_direction = DIRECTION_WEST;
		}
		else if ((room.direction[DIRECTION_WEST] == DIRECTION_OPEN) && (last_direction != DIRECTION_EAST)) {
			sentOption(OPTION_MOVE_WEST, 0);
			last_direction = DIRECTION_WEST;
		}
		break;
	case 4://up
		if ((room.direction[DIRECTION_UP] == DIRECTION_OPEN) && (last_direction != DIRECTION_DOWN)) {
			sentOption(OPTION_MOVE_UP, 0);
			last_direction = DIRECTION_UP;
		}
		break;
	case 5://down
		if ((room.direction[DIRECTION_UP] == DIRECTION_OPEN) && (last_direction != DIRECTION_DOWN)) {
			sentOption(OPTION_MOVE_UP, 0);
			last_direction = DIRECTION_UP;
		}
		break;
	default:
		break;
	}
}


//preset route execution		
//int route[2][2000];
int mvkeyarray[] = {
	4,0x0,
	10,0xBB54,
	4,0x0,
	4,0x0,
	4,0x0,
	5,0x0,
	3,0x0,
	2,0x0,
	1359,0x0,
	8,0x090D,
	2,0x0,
	2,0x0,
	4,0x0,
	1325,0x0,
	1,0x0,
	1,0x0,
	7,0x69F4,
	1,0x0,
	5,0x0,
	8,0x8F32,
	2,0x0,
	2,0x0,
	8,0x9F4A,
	2,0x0,
	334,0x0,
	1334,0x0,
	634,0x0,
	1,0x0,
	1,0x0,
	1,0x0,
	6,0x0,
	6,0x0,
	8,0x12FC,
	2,0x0,
	2,0x0,
	8,0xFD5A,
	2,0x0,
	204,0x0,
	504,0x0,
	1,0x0,
	1,0x0,
	3,0x0,
	1340,0x0,
	7,0xBE27,
	1,0x0,
	4,0x0,
	5,0x0,
	3,0x0,
	9,0xE4A8,
	3,0x0,
	3,0x0,
	1339,0x0,
	545,0x0,
	8,0x58B8,
	2,0x0,
	10,0xE333,
	4,0x0,
	1309,0x0,
	575,0x0,
	1,0x0,
	4,0x0,
	4,0x0,
	6,0x0,
	2,0x0,
	2,0x0,
	216,0x0,
	516,0x0,
	1,0x0,
	1,0x0,
	5,0x0,
	5,0x0,
	9,0x89F3,
	3,0x0,
	3,0x0,
	249,0x0,
	1357,0x0,
	4,0x0,
	4,0x0,
	6,0x0,
	3,0x0,
	2,0x0,
	2,0x0,
	2,0x0,
	250,0x0,
	269,0x0,
	1354,0x0,
	569,0x0,
	8,0x34C2,
	2,0x0,
	4,0x0,
	225,0x0,
	1321,0x0,
	7,0x43AB,
	1,0x0,
	1,0x0,
	1,0x0,
	1,0x0,
	5,0x0,
	3,0x0,
	3,0x0,
	9,0x34C2,
	3,0x0,
	202,0x0,
	1168,0x0,
	1316,0x0,
	568,0x0,
	584,0x0,
	8,0x8FD4,
	2,0x0,
	10,0xA100,
	4,0x0,
	1355,0x0,
	2,0x0,
	4,0x0,
	1338,0x0,
	7,0xA100,
	1,0x0,
	1,0x0,
	4,0x0,
	6,0x0,
	6,0x0,
	2,0x0,
	2,0x0,
	2,0x0,
	3,0x0,
	1346,0x0,
	619,0x0,
	4,0x0,
	1,0x0,
	1,0x0,
	1,0x0,
	5,0x0,
	3,0x0,
	2,0x0,
	2,0x0,
	2,0x0,
	9,0xC75F,
	3,0x0,
	1117,0x0,
	1347,0x0,
	8,0xD976,
	2,0x0,
	2,0x0,
	4,0x0,
	4,0x0,
	1307,0x0,
	1,0x0,
	1,0x0,
	1,0x0,
	1,0x0,
	1,0x0,
	5,0x0,
	3,0x0,
	3,0x0,
	3,0x0,
	3,0x0,
	537,0x0,
	629,0x0,
	4,0x0,
	2,0x0,
	2,0x0,
	2,0x0,
	4,0x0,
	10,0x8FD4,
	4,0x0,
	307,0x0,
	1,0x0,
	3,0x0,
	3,0x0,
	3,0x0,
	3,0x0,
	1,0x0,
	3,0x0,
	9,0xF904,
	3,0x0,
	7,0x0172,
	1,0x0,
	3,0x0,
	3,0x0,
	6,0x0,
	6,0x0,
	8,0x1AE1,
	2,0x0,
	2,0x0,
	580,0x0,
	1,0x0,
	1,0x0,
	10,0x0EB9,
	4,0x0,
	10,0x88DE,
	4,0x0,
	4,0x0,
	4,0x0,
	4,0x0,
	4,0x0,
	10,0x12FC,
	4,0x0,
	4,0x0,
	4,0x0,
	5,0x0,
	2,0x0,
	2,0x0,
	2,0x0,
	2,0x0,
	2,0x0,
	8,0x090D,
	2,0x0,
	1114,0x0,
	314,0x0,
	514,0x0,
	523,0x0,
	614,0x0,
	1,0x0,
	1,0x0,
	1,0x0,
	1,0x0,
	1,0x0,
	1,0x0,
	5,0x0,
	3,0x0,
	3,0x0,
	8,0xC567,
	2,0x0,
	2,0x0,
	8,0x8F32,
	2,0x0,
	2,0x0,
	10,0x090D,
	4,0x0,
	564,0x0,
	7,0xD976,
	1,0x0,
	1,0x0,
	1,0x0,
	1,0x0,
	4,0x0,
	6,0x0,
	6,0x0,
	3,0x0,
	3,0x0,
	3,0x0,
	3,0x0,
	8,0x200F,
	2,0x0,
	288,0x0,
	588,0x0,
	1,0x0,
	4,0x0,
	4,0x0,
	4,0x0,
	4,0x0,
	5,0x0,
	2,0x0,
	2,0x0,
	2,0x0,
	2,0x0,
	2,0x0,
	2,0x0,
	2,0x0,
	318,0x0,
	3,0x0,
	9,0x55D5,
	3,0x0,
	3,0x0,
	8,0xBCD5,
	2,0x0,
	3,0x0,
	9,0x29FC,
	3,0x0,
	3,0x0,
	3,0x0,
	3,0x0,
	3,0x0,
	2,0x0,
	5,0x0,
	7,0x0172,
	1,0x0,
	7,0xBB54,
	1,0x0,
	1,0x0,
	7,0x12FC,
	1,0x0,
	1,0x0,
	7,0x88DE,
	1,0x0,
	330,0x0,
	630,0x0,
	1,0x0,
	7,0x29FC,
	1,0x0,
	1,0x0,
	6,0x0,
	6,0x0,
	2,0x0,
	2,0x0,
	8,0x200F,
	2,0x0,
	2,0x0,
	2,0x0,
	8,0x8F32,
	2,0x0,
	1332,0x0,
	621,0x0,
	1,0x0,
	231,0x0,
	531,0x0,
	638,0x0,
	1,0x0,
	1,0x0,
	1,0x0,
	1,0x0,
	1,0x0,
	5,0x0,
	5,0x0,
	2,0x0,
	2,0x0,
	2,0x0,
	2,0x0,
	1330,0x0,
	1,0x0,
	1,0x0,
	1,0x0,
	1,0x0,
	5,0x0,
	2,0x0,
	10,0x71DA,
	4,0x0,
	10,0x43AB,
	4,0x0,
	4,0x0,
	10,0xC4B6,
	4,0x0,
	4,0x0,
	10,0xFD5A,
	4,0x0,
	4,0x0,
	10,0xBCD5,
	4,0x0,
	278,0x0,
	578,0x0,
	2,0x0,
	4,0x0,
	596,0x0,
	7,0x090D,
	1,0x0,
	1,0x0,
	6,0x0,
	6,0x0,
	6,0x0,
	3,0x0,
	3,0x0,
	3,0x0,
	3,0x0,
	3,0x0,
	3,0x0,
	2,0x0,
	8,0xBCD5,
	2,0x0,
	2,0x0,
	2,0x0,
	1311,0x0,
	8,0xBE27,
	2,0x0,
	10,0x090D,
	4,0x0,
	1318,0x0,
	2,0x0,
	4,0x0,
	1341,0x0,
	3,0x0,
	9,0x395D,
	3,0x0,
	3,0x0,
	9,0xE333,
	3,0x0,
	9,0x71DA,
	3,0x0,
	2,0x0,
	2,0x0,
	2,0x0,
	5,0x0,
	5,0x0,
	10,0x090D,
	4,0x0,
	10,0x395D,
	4,0x0,
	247,0x0,
	1356,0x0,
	547,0x0,
	3,0x0,
	7,0xBB54,
	1,0x0,
	7,0xF904,
	1,0x0,
	4,0x0,
	10,0xD2F7,
	4,0x0,
	1358,0x0,
	3,0x0,
	3,0x0,
	3,0x0,
	2,0x0,
	2,0x0,
	6,0x0,
	6,0x0,
	1,0x0,
	1,0x0,
	1,0x0,
	1,0x0,
	1,0x0,
	1,0x0,
	4,0x0,
	4,0x0,
	4,0x0,
	10,0xAB62,
	4,0x0,
	4,0x0,
	244,0x0,
	522,0x0,
	544,0x0,
	2,0x0,
	10,0x0172,
	4,0x0,
	8,0x58B8,
	2,0x0,
	4,0x0,
	505,0x0,
	7,0x69F4,
	1,0x0,
	1,0x0,
	1,0x0,
	1,0x0,
	1,0x0,
	3,0x0,
	3,0x0,
	3,0x0,
	3,0x0,
	3,0x0,
	3,0x0,
	3,0x0,
	5,0x0,
	2,0x0,
	4,0x0,
	4,0x0,
	2,0x0,
	2,0x0,
	8,0x0EB9,
	2,0x0,
	1329,0x0,
	9,0x55D5,
	3,0x0,
	2,0x0,
	8,0x8E28,
	2,0x0,
	2,0x0,
	9,0xD2F7,
	3,0x0,
	2,0x0,
	2,0x0,
	5,0x0,
	1,0x0,
	1,0x0,
	4,0x0,
	4,0x0,
	4,0x0,
	4,0x0,
	554,0x0,
	3,0x0,
	3,0x0,
	3,0x0,
	3,0x0,
	2,0x0,
	2,0x0,
	6,0x0,
	1,0x0,
	4,0x0,
	4,0x0,
	4,0x0,
	1,0x0,
	1327,0x0,
	2,0x0,
	3,0x0,
	3,0x0,
	3,0x0,
	2,0x0,
	6,0x0,
	1,0x0,
	1,0x0,
	1,0x0,
	1,0x0,
	1,0x0,
	10,0x8FD4,
	4,0x0,
	10,0x71DA,
	4,0x0,
	4,0x0,
	10,0x55D5,
	4,0x0,
	1155,0x0,
	1,0x0,
	3,0x0,
	3,0x0,
	3,0x0,
	235,0x0,
	535,0x0,
	2,0x0,
	4,0x0,
	1185,0x0,
	585,0x0,
	8,0x69F4,
	2,0x0,
	4,0x0,
	233,0x0,
	2,0x0,
	4,0x0,
	595,0x0,
	3,0x0,
	3,0x0,
	3,0x0,
	3,0x0,
	2,0x0,
	2,0x0,
	2,0x0,
	5,0x0,
	1,0x0,
	4,0x0,
	4,0x0,
	4,0x0,
	4,0x0,
	4,0x0,
	317,0x0,
	1303,0x0,
	3,0x0,
	3,0x0,
	3,0x0,
	3,0x0,
	3,0x0,
	2,0x0,
	6,0x0,
	10,0x49CD,
	4,0x0,
	10,0x71DA,
	4,0x0,
	1336,0x0,
	3,0x0,
	3,0x0,
	5,0x0,
	5,0x0,
	1,0x0,
	1,0x0,
	1,0x0,
	4,0x0,
	4,0x0,
	10,0xBCD5,
	4,0x0,
	4,0x0,
	4,0x0,
	4,0x0,
	10,0xFD5A,
	4,0x0,
	8,0xC75F,
	2,0x0,
	328,0x0,
	628,0x0,
	2,0x0,
	2,0x0,
	10,0x71DA,
	4,0x0,
	4,0x0,
	6,0x0,
	7,0x090D,
	1,0x0,
	7,0xBCD5,
	1,0x0,
	1,0x0,
	1,0x0,
	1,0x0,
	1,0x0,
	3,0x0,
	3,0x0,
	3,0x0,
	562,0x0,
	567,0x0,
	8,0x12FC,
	2,0x0,
	4,0x0,
	301,0x0,
	1315,0x0,
	601,0x0,
	1,0x0,
	4,0x0,
	1,0x0,
	1,0x0,
	1,0x0,
	4,0x0,
	6,0x0,
	3,0x0,
	3,0x0,
	3,0x0,
	3,0x0,
	2,0x0,
	2,0x0,
	8,0x55D5,
	2,0x0,
	2,0x0,
	8,0xBCD5,
	2,0x0,
	1189,0x0,
	589,0x0,
	1,0x0,
	1,0x0,
	1,0x0,
	1,0x0,
	1,0x0,
	4,0x0,
	4,0x0,
	4,0x0,
	4,0x0,
	5,0x0,
	5,0x0,
	3,0x0,
	3,0x0,
	3,0x0,
	3,0x0,
	9,0xE333,
	3,0x0,
	1314,0x0,
	532,0x0,
	4,0x0,
	4,0x0,
	4,0x0,
	4,0x0,
	4,0x0,
	6,0x0,
	6,0x0,
	2,0x0,
	2,0x0,
	2,0x0,
	2,0x0,
	8,0xF904,
	2,0x0,
	506,0x0,
	1,0x0,
	1,0x0,
	3,0x0,
	9,0xBB54,
	3,0x0,
	1,0x0,
	3,0x0,
	9,0x29FC,
	3,0x0,
	265,0x0,
	1335,0x0,
	1,0x0,
	1,0x0,
	4,0x0,
	4,0x0,
	4,0x0,
	4,0x0,
	5,0x0,
	5,0x0,
	3,0x0,
	3,0x0,
	3,0x0,
	2,0x0,
	2,0x0,
	2,0x0,
	2,0x0,
	2,0x0,
	8,0x8E28,
	2,0x0,
	228,0x0,
	1319,0x0,
	528,0x0,
	1,0x0,
	9,0x0EB9,
	3,0x0,
	3,0x0,
	3,0x0,
	3,0x0,
	224,0x0,
	9,0x49CD,
	3,0x0,
	9,0x222C,
	3,0x0,
	2,0x0,
	2,0x0,
	2,0x0,
	2,0x0,
	6,0x0,
	1,0x0,
	1,0x0,
	297,0x0,
	597,0x0,
	2,0x0,
	2,0x0,
	6,0x0,
	1,0x0,
	1,0x0,
	1,0x0,
	4,0x0,
	4,0x0,
	4,0x0,
	1352,0x0,
	612,0x0,
	3,0x0,
	3,0x0,
	3,0x0,
	2,0x0,
	2,0x0,
	2,0x0,
	5,0x0,
	1,0x0,
	4,0x0,
	4,0x0,
	4,0x0,
	4,0x0,
	4,0x0,
	4,0x0,
	1,0x0,
	1138,0x0,
	1342,0x0,
	538,0x0,
	1,0x0,
	7,0x19F4,
	1,0x0,
	1,0x0,
	1,0x0,
	9,0x8E28,
	3,0x0,
	3,0x0,
	9,0xBE27,
	3,0x0,
	3,0x0,
	9,0xD976,
	3,0x0,
	511,0x0,
	1,0x0,
	4,0x0,
	1,0x0,
	2,0x0,
	3,0x0,
	2,0x0,
	8,0x6BA3,
	2,0x0,
	2,0x0,
	2,0x0,
	2,0x0,
	3,0x0,
	2,0x0,
	2,0x0,
	6,0x0,
	4,0x0,
	4,0x0,
	4,0x0,
	324,0x0,
	577,0x0,
	624,0x0,
	1,0x0,
	1,0x0,
	7,0xD75B,
	1,0x0,
	1,0x0,
	1,0x0,
	1,0x0,
	1,0x0,
	1,0x0,
	9,0x395D,
	3,0x0,
	320,0x0,
	620,0x0,
	7,0xFD5A,
	1,0x0,
	3,0x0,
	3,0x0,
	5,0x0,
	2,0x0,
	4,0x0,
	4,0x0,
	2,0x0,
	10,0xD75B,
	4,0x0,
	4,0x0,
	8,0x69F4,
	2,0x0,
	4,0x0,
	1324,0x0,
	576,0x0,
	8,0xC4B6,
	2,0x0,
	2,0x0,
	2,0x0,
	10,0xD2F7,
	4,0x0,
	4,0x0,
	10,0x1AE1,
	4,0x0,
	583,0x0,
	7,0xD976,
	1,0x0,
	1,0x0,
	1,0x0,
	1,0x0,
	1,0x0,
	1,0x0,
	4,0x0,
	6,0x0,
	3,0x0,
	3,0x0,
	3,0x0,
	3,0x0,
	2,0x0,
	2,0x0,
	2,0x0,
	3,0x0,
	1337,0x0,
	3,0x0,
	1,0x0,
	1,0x0,
	1,0x0,
	3,0x0,
	3,0x0,
	3,0x0,
	5,0x0,
	5,0x0,
	2,0x0,
	2,0x0,
	10,0x43AB,
	4,0x0,
	10,0x7F07,
	4,0x0,
	4,0x0,
	1333,0x0,
	3,0x0,
	3,0x0,
	2,0x0,
	9,0x55D5,
	3,0x0,
	2,0x0,
	2,0x0,
	2,0x0,
	2,0x0,
	2,0x0,
	2,0x0,
	6,0x0,
	6,0x0,
	1,0x0,
	4,0x0,
	4,0x0,
	1351,0x0,
	1,0x0,
	7,0x55D5,
	1,0x0,
	1,0x0,
	9,0x6BA3,
	3,0x0,
	253,0x0,
	2,0x0,
	4,0x0,
	226,0x0,
	1190,0x0,
	1305,0x0,
	590,0x0,
	3,0x0,
	3,0x0,
	2,0x0,
	2,0x0,
	2,0x0,
	5,0x0,
	1,0x0,
	4,0x0,
	4,0x0,
	4,0x0,
	4,0x0,
	4,0x0,
	4,0x0,
	1,0x0,
	4,0x0,
	4,0x0,
	208,0x0,
	1,0x0,
	3,0x0,
	3,0x0,
	3,0x0,
	3,0x0,
	3,0x0,
	3,0x0,
	9,0x71DA,
	3,0x0,
	1,0x0,
	1,0x0,
	1,0x0,
	4,0x0,
	4,0x0,
	4,0x0,
	242,0x0,
	542,0x0,
	1,0x0,
	1,0x0,
	3,0x0,
	7,0x43AB,
	1,0x0,
	3,0x0,
	3,0x0,
	3,0x0,
	5,0x0,
	2,0x0,
	2,0x0,
	2,0x0,
	4,0x0,
	10,0x0172,
	4,0x0,
	4,0x0,
	251,0x0,
	1323,0x0,
	551,0x0,
	1,0x0,
	7,0x0EB9,
	1,0x0,
	3,0x0,
	1353,0x0,
	1,0x0,
	3,0x0,
	3,0x0,
	6,0x0,
	6,0x0,
	2,0x0,
	2,0x0,
	2,0x0,
	2,0x0,
	2,0x0,
	10,0xAB62,
	4,0x0,
	4,0x0,
	4,0x0,
	4,0x0,
	4,0x0,
	10,0x69F4,
	4,0x0,
	4,0x0,
	10,0x19F4,
	4,0x0,
	1312,0x0,
	7,0x8FD4,
	1,0x0,
	10,0x69F4,
	4,0x0,
	566,0x0,
	3,0x0,
	3,0x0,
	3,0x0,
	3,0x0,
	3,0x0,
	3,0x0,
	3,0x0,
	3,0x0,
	8,0x0172,
	2,0x0,
	2,0x0,
	3,0x0,
	2,0x0,
	2,0x0,
	2,0x0,
	5,0x0,
	1,0x0,
	4,0x0,
	4,0x0,
	4,0x0,
	4,0x0,
	4,0x0,
	4,0x0,
	1,0x0,
	4,0x0,
	2,0x0,
	2,0x0,
	248,0x0,
	548,0x0,
	1,0x0,
	1,0x0,
	7,0x0EB9,
	1,0x0,
	1,0x0,
	9,0xE4A8,
	3,0x0,
	3,0x0,
	3,0x0,
	9,0x12FC,
	3,0x0,
	227,0x0,
	1313,0x0,
	2,0x0,
	8,0x8E28,
	2,0x0,
	2,0x0,
	4,0x0,
	4,0x0,
	8,0x43AB,
	2,0x0,
	4,0x0,
	10,0x200F,
	4,0x0,
	4,0x0,
	10,0x090D,
	4,0x0,
	5,0x0,
	7,0xC4B6,
	1,0x0,
	7,0x8E28,
	1,0x0,
	9,0xBB54,
	3,0x0,
	1,0x0,
	3,0x0,
	1300,0x0,
	1,0x0,
	3,0x0,
	1,0x0,
	3,0x0,
	9,0x71DA,
	3,0x0,
	3,0x0,
	1304,0x0,
	9,0xFD5A,
	3,0x0,
	1,0x0,
	7,0xBE27,
	1,0x0,
	1,0x0,
	4,0x0,
	4,0x0,
	7,0xBE27,
	1,0x0,
	4,0x0,
	4,0x0,
	4,0x0,
	4,0x0,
	4,0x0,
	5,0x0,
	3,0x0,
	9,0xBE27,
	3,0x0,
	520,0x0,
	3,0x0,
	8,0x58B8,
	2,0x0,
	3,0x0,
	3,0x0,
	3,0x0,
	7,0x8E28,
	1,0x0,
	3,0x0,
	9,0xBB54,
	3,0x0,
	9,0x9F4A,
	3,0x0,
	6,0x0,
	2,0x0,
	2,0x0,
	2,0x0,
	2,0x0,
	2,0x0,
	4,0x0,
	1,0x0,
	7,0x1AE1,
	1,0x0,
	1,0x0,
	3,0x0,
	1,0x0,
	1,0x0,
	5,0x0,
	2,0x0,
	4,0x0,
	4,0x0,
	4,0x0,
	4,0x0,
	4,0x0,
	4,0x0,
	4,0x0,
	587,0x0,
	3,0x0,
	3,0x0,
	3,0x0,
	3,0x0,
	1,0x0,
	3,0x0,
	605,0x0,
	3,0x0,
	3,0x0,
	6,0x0,
	2,0x0,
	2,0x0,
	2,0x0,
	2,0x0,
	2,0x0,
	2,0x0,
	2,0x0,
	2,0x0,
	2,0x0,
	5,0x0,
	4,0x0,
	4,0x0,
	1308,0x0,
	3,0x0,
	3,0x0,
	6,0x0,
	6,0x0,
	6,0x0,
	1,0x0,
	4,0x0,
	4,0x0,
	10,0x7F07,
	4,0x0,
	4,0x0,
	10,0xD976,
	4,0x0,
	4,0x0,
	10,0x88DE,
	4,0x0,
	240,0x0,
	299,0x0,
	513,0x0,
	540,0x0,
	599,0x0,
	3,0x0,
	3,0x0,
	3,0x0,
	3,0x0,
	3,0x0,
	3,0x0,
	3,0x0,
	2,0x0,
	5,0x0,
	1,0x0,
	1,0x0,
	4,0x0,
	1,0x0,
	1,0x0,
	9,0xD75B,
	3,0x0,
	333,0x0,
	4,0x0,
	2,0x0,
	2,0x0,
	3,0x0,
	2,0x0,
	2,0x0,
	6,0x0,
	1,0x0,
	4,0x0,
	4,0x0,
	4,0x0,
	4,0x0,
	4,0x0,
	622,0x0,
	3,0x0,
	3,0x0,
	3,0x0,
	3,0x0,
	3,0x0,
	2,0x0,
	5,0x0,
	5,0x0,
	1,0x0,
	1,0x0,
	4,0x0,
	4,0x0,
	4,0x0,
	2,0x0,
	10,0xC4B6,
	4,0x0,
	2,0x0,
	1349,0x0,
	594,0x0,
	1,0x0,
	1,0x0,
	7,0x55D5,
	1,0x0,
	1,0x0,
	1,0x0,
	7,0x43AB,
	1,0x0,
	1,0x0,
	1,0x0,
	3,0x0,
	3,0x0,
	1,0x0,
	3,0x0,
	3,0x0,
	5,0x0,
	4,0x0,
	4,0x0,
	4,0x0,
	327,0x0,
	2,0x0,
	2,0x0,
	4,0x0,
	4,0x0,
	10,0xE4A8,
	4,0x0,
	4,0x0,
	219,0x0,
	1317,0x0,
	7,0xC75F,
	1,0x0,
	3,0x0,
	230,0x0,
	593,0x0,
	1,0x0,
	4,0x0,
	4,0x0,
	4,0x0,
	6,0x0,
	6,0x0,
	6,0x0,
	2,0x0,
	2,0x0,
	2,0x0,
	2,0x0,
	2,0x0,
	2,0x0,
	8,0x71DA,
	2,0x0,
	1328,0x0,
	3,0x0,
	9,0x0172,
	3,0x0,
	3,0x0,
	9,0xE333,
	3,0x0,
	3,0x0,
	9,0xD976,
	3,0x0,
	3,0x0,
	3,0x0,
	3,0x0,
	2,0x0,
	2,0x0,
	5,0x0,
	5,0x0,
	1,0x0,
	1,0x0,
	4,0x0,
	4,0x0,
	4,0x0,
	2,0x0,
	4,0x0,
	215,0x0,
	1301,0x0,
	1,0x0,
	3,0x0,
	3,0x0,
	3,0x0,
	3,0x0,
	2,0x0,
	2,0x0,
	6,0x0,
	6,0x0,
	1,0x0,
	4,0x0,
	4,0x0,
	4,0x0,
	4,0x0,
	4,0x0,
	4,0x0,
	613,0x0,
	3,0x0,
	3,0x0,
	3,0x0,
	3,0x0,
	3,0x0,
	3,0x0,
	2,0x0,
	5,0x0,
	1,0x0,
	4,0x0,
	4,0x0,
	4,0x0,
	4,0x0,
	4,0x0,
	2,0x0,
	1,0x0,
	3,0x0,
	3,0x0,
	3,0x0,
	3,0x0,
	3,0x0,
	2,0x0,
	6,0x0,
	1,0x0,
	1,0x0,
	4,0x0,
	4,0x0,
	4,0x0,
	4,0x0,
	4,0x0,
	207,0x0,
	241,0x0,
	541,0x0,
	3,0x0,
	3,0x0,
	3,0x0,
	3,0x0,
	3,0x0,
	2,0x0,
	2,0x0,
	5,0x0,
	5,0x0,
	1,0x0,
	1,0x0,
	4,0x0,
	4,0x0,
	4,0x0,
	2,0x0,
	4,0x0,
	2,0x0,
	4,0x0,
	10,0x55D5,
	4,0x0,
	4,0x0,
	1348,0x0,
	1,0x0,
	1,0x0,
	1,0x0,
	1,0x0,
	7,0x090D,
	1,0x0,
	1,0x0,
	1,0x0,
	1,0x0,
	1,0x0,
	4,0x0,
	4,0x0,
	5,0x0,
	3,0x0,
	8,0x88DE,
	2,0x0,
	2,0x0,
	256,0x0,
	556,0x0,
	1,0x0,
	1,0x0,
	4,0x0,
	6,0x0,
	2,0x0,
	2,0x0,
	2,0x0,
	2,0x0,
	8,0x8FD4,
	2,0x0,
	2,0x0,
	2,0x0,
	2,0x0,
	2,0x0,
	5,0x0,
	9,0x43AB,
	3,0x0,
	3,0x0,
	200,0x0,
	500,0x0,
	9,0x8F32,
	3,0x0,
	3,0x0,
	9,0xC567,
	3,0x0,
	3,0x0,
	9,0x8E28,
	3,0x0,
	3,0x0,
	3,0x0,
	6,0x0,
	1,0x0,
	1,0x0,
	1,0x0,
	1,0x0,
	1,0x0,
	1,0x0,
	1,0x0,
	1,0x0,
	1,0x0,
	5,0x0,
	2,0x0,
	4,0x0,
	1215,0x0,
	326,0x0,
	626,0x0,
	4,0x0,
	4,0x0,
	4,0x0,
	4,0x0,
	4,0x0,
	1,0x0,
	4,0x0,
	4,0x0,
	4,0x0,
	6,0x0,
	6,0x0,
	6,0x0,
	2,0x0,
	2,0x0,
	2,0x0,
	2,0x0,
	2,0x0,
	2,0x0,
	1210,0x0,
	1302,0x0,
	610,0x0,
	1,0x0,
	1,0x0,
	1,0x0,
	1,0x0,
	1,0x0,
	1,0x0,
	5,0x0,
	2,0x0,
	2,0x0,
	2,0x0,
	2,0x0,
	2,0x0,
	2,0x0,
	2,0x0,
	2,0x0,
	2,0x0,
	5,0x0,
	3,0x0,
	3,0x0,
	3,0x0,
	3,0x0,
	210,0x0,
	1,0x0,
	632,0x0,
	2,0x0,
	4,0x0,
	4,0x0,
	4,0x0,
	4,0x0,
	5,0x0,
	1,0x0,
	3,0x0,
	9,0x71DA,
	3,0x0,
	336,0x0,
	1350,0x0,
	2,0x0,
	4,0x0,
	4,0x0,
	6,0x0,
	1,0x0,
	1,0x0,
	1,0x0,
	1,0x0,
	1,0x0,
	1,0x0,
	1,0x0,
	1,0x0,
	1,0x0,
	5,0x0,
	3,0x0,
	2,0x0,
	2,0x0,
	8,0x58B8,
	2,0x0,
	1,0x0,
	9,0x58B8,
	3,0x0,
	3,0x0,
	339,0x0,
	3,0x0,
	1320,0x0,
	1,0x0,
	4,0x0,
	1,0x0,
	4,0x0,
	4,0x0,
	4,0x0,
	6,0x0,
	2,0x0,
	2,0x0,
	2,0x0,
	2,0x0,
	2,0x0,
	2,0x0,
	2,0x0,
	2,0x0,
	2,0x0,
	5,0x0,
	1,0x0,
	3,0x0,
	3,0x0,
	3,0x0,
	9,0xA100,
	3,0x0,
	209,0x0,
	260,0x0,
	509,0x0,
	560,0x0,
	3,0x0,
	335,0x0,
	2,0x0,
	3,0x0,
	3,0x0,
	3,0x0,
	3,0x0,
	6,0x0,
	1,0x0,
	1,0x0,
	1,0x0,
	1,0x0,
	1,0x0,
	1,0x0,
	1,0x0,
	1,0x0,
	1,0x0,
	5,0x0,
	2,0x0,
	2,0x0,
	8,0x34C2,
	2,0x0,
	4,0x0,
	4,0x0,
	10,0xBE27,
	4,0x0,
	4,0x0,
	1,0x0,
	1,0x0,
	4,0x0,
	4,0x0,
	1,0x0,
	4,0x0,
	4,0x0,
	4,0x0,
	6,0x0,
	2,0x0,
	2,0x0,
	2,0x0,
	2,0x0,
	2,0x0,
	2,0x0,
	2,0x0,
	2,0x0,
	2,0x0,
	5,0x0,
	1,0x0,
	1,0x0,
	7,0xD976,
	1,0x0,
	272,0x0,
	1306,0x0,
	572,0x0,
	9,0x71DA,
	3,0x0,
	3,0x0,
	9,0xC567,
	3,0x0,
	3,0x0,
	2,0x0,
	1331,0x0,
	1,0x0,
	9,0x29FC,
	3,0x0,
	3,0x0,
	9,0x89F3,
	3,0x0,
	221,0x0,
	521,0x0,
	4,0x0,
	4,0x0,
	4,0x0,
	4,0x0,
	4,0x0,
	1345,0x0,
	3,0x0,
	3,0x0,
	3,0x0,
	561,0x0,
	3,0x0,
	637,0x0,
	4,0x0,
	4,0x0,
	4,0x0,
	274,0x0,
	3,0x0,
	3,0x0,
	3,0x0,
	3,0x0,
	4,0x0,
	3,0x0,
	2,0x0,
	1,0x0,
	3,0x0,
	2,0x0,
	4,0x0,
	4,0x0,
	7,0x090D,
	1,0x0,
	4,0x0,
	3,0x0,
	3,0x0,
	2,0x0,
	2,0x0,
	1,0x0,
	3,0x0,
	1,0x0,
	3,0x0,
	2,0x0,
	4,0x0,
	4,0x0,
	4,0x0,
	4,0x0,
	7,0xD2F7,
	1,0x0,
	4,0x0,
	3,0x0,
	3,0x0,
	3,0x0,
	2,0x0,
	2,0x0,
	2,0x0,
	1,0x0,
	3,0x0,
	1,0x0,
	3,0x0,
	2,0x0,
	8,0xE4A8,
	2,0x0,
	4,0x0,
	4,0x0,
	4,0x0,
	4,0x0,
	1,0x0,
	1,0x0,
	10,0x1AE1,
	4,0x0,
	1,0x0,
	4,0x0,
	3,0x0,
	3,0x0,
	3,0x0,
	3,0x0,
	3,0x0,
	2,0x0,
	2,0x0,
	2,0x0,
	1,0x0,
	3,0x0,
	2,0x0,
	4,0x0,
	4,0x0,
	4,0x0,
	4,0x0,
	1,0x0,
	4,0x0,
	7,0x1AE1,
	1,0x0,
	4,0x0,
	1,0x0,
	4,0x0,
	3,0x0,
	3,0x0,
	3,0x0,
	3,0x0,
	3,0x0,
	3,0x0,
	3,0x0,
	2,0x0,
	2,0x0,
	2,0x0,
	6,0x0,
	4,0x0,
	4,0x0,
	3,0x0,
	3,0x0,
	5,0x0,
	4,0x0,
	4,0x0,
	4,0x0,
	4,0x0,
	4,0x0,
	7,0x19F4,
	1,0x0,
	4,0x0,
	7,0xD75B,
	1,0x0,
	10,0x7F07,
	4,0x0,
	7,0x200F,
	1,0x0,
	4,0x0,
	2,0x0,
	8,0xF904,
	2,0x0,
	2,0x0,
	4,0x0,
	6,0x0,
	3,0x0,
	3,0x0,
	3,0x0,
	3,0x0,
	3,0x0,
	1,0x0,
	1,0x0,
	3,0x0,
	3,0x0,
	3,0x0,
	2,0x0,
	2,0x0,
	1,0x0,
	9,0xE4A8,
	3,0x0,
	2,0x0,
	6,0x0,
	4,0x0,
	4,0x0,
	10,0xE4A8,
	4,0x0,
	3,0x0,
	3,0x0,
	3,0x0,
	5,0x0,
	1,0x0,
	1,0x0,
	4,0x0,
	4,0x0,
	4,0x0,
	2,0x0,
	4,0x0,
	4,0x0,
	2,0x0,
	4,0x0,
	4,0x0,
	4,0x0,
	4,0x0,
	5,0x0,
	3,0x0,
	3,0x0,
	3,0x0,
	7,0x49CD,
	1,0x0,
	4,0x0,
	1,0x0,
	4,0x0,
	1,0x0,
	4,0x0,
	2,0x0,
	2,0x0,
	2,0x0,
	6,0x0,
	3,0x0,
	3,0x0,
	3,0x0,
	3,0x0,
	1,0x0,
	1,0x0,
	9,0x89F3,
	3,0x0,
	3,0x0,
	3,0x0,
	3,0x0,
	3,0x0,
	2,0x0,
	2,0x0,
	6,0x0,
	6,0x0,
	5,0x0,
	5,0x0,
	1,0x0,
	1,0x0,
	4,0x0,
	4,0x0,
	4,0x0,
	2,0x0,
	4,0x0,
	4,0x0,
	10,0x8E28,
	4,0x0,
	3,0x0,
	1,0x0,
	3,0x0,
	3,0x0,
	3,0x0,
	3,0x0,
	2,0x0,
	1,0x0,
	3,0x0,
	4,0x0,
	4,0x0,
	4,0x0,
	4,0x0,
	4,0x0,
	1,0x0,
	1,0x0,
	1,0x0,
	7,0xFD5A,
	1,0x0,
	1,0x0,
	3,0x0,
	1,0x0,
	3,0x0,
	3,0x0,
	1,0x0,
	3,0x0,
	3,0x0,
	5,0x0,
	2,0x0,
	2,0x0,
	2,0x0,
	4,0x0,
	4,0x0,
	7,0xA100,
	1,0x0,
	1,0x0,
	1,0x0,
	3,0x0,
	3,0x0,
	6,0x0,
	6,0x0,
	2,0x0,
	4,0x0,
	4,0x0,
	2,0x0,
	2,0x0,
	2,0x0,
	2,0x0,
	8,0xFD5A,
	2,0x0,
	4,0x0,
	2,0x0,
	10,0x090D,
	4,0x0,
	3,0x0,
	2,0x0,
	3,0x0,
	8,0x0EB9,
	2,0x0,
	3,0x0,
	3,0x0,
	5,0x0,
	5,0x0,
	4,0x0,
	4,0x0,
	4,0x0,
	4,0x0,
	4,0x0,
	4,0x0,
	4,0x0,
	1,0x0,
	4,0x0,
	1,0x0,
	10,0x58B8,
	4,0x0,
	2,0x0,
	2,0x0,
	6,0x0,
	3,0x0,
	3,0x0,
	3,0x0,
	4,0x0,
	4,0x0,
	4,0x0,
	6,0x0,
	3,0x0,
	1,0x0,
	3,0x0,
	9,0xA100,
	3,0x0,
	3,0x0,
	3,0x0,
	3,0x0,
	3,0x0,
	3,0x0,
	3,0x0,
	2,0x0,
	6,0x0,
	4,0x0,
	1,0x0,
	3,0x0,
	1,0x0,
	1,0x0,
	1,0x0,
	1,0x0,
	1,0x0,
	1,0x0,
	1,0x0,
	1,0x0,
	5,0x0,
	5,0x0,
	5,0x0,
	2,0x0,
	2,0x0,
	2,0x0,
	4,0x0,
	4,0x0,
	4,0x0,
	1,0x0,
	1,0x0,
	4,0x0,
	4,0x0,
	4,0x0,
	1,0x0,
	4,0x0,
	4,0x0,
	4,0x0,
	6,0x0,
	3,0x0,
	3,0x0,
	2,0x0,
	2,0x0,
	2,0x0,
	2,0x0,
	2,0x0,
	2,0x0,
	2,0x0,
	2,0x0,
	3,0x0,
	3,0x0,
	1,0x0,
	3,0x0,
	3,0x0,
	3,0x0,
	3,0x0,
	1,0x0,
	3,0x0,
	4,0x0,
	4,0x0,
	4,0x0,
	4,0x0,
	4,0x0,
	1,0x0,
	1,0x0,
	1,0x0,
	1,0x0,
	3,0x0,
	1,0x0,
	3,0x0,
	3,0x0,
	1,0x0,
	3,0x0,
	3,0x0,
	5,0x0,
	4,0x0,
	4,0x0,
	4,0x0,
	10,0x43AB,
	4,0x0,
	3,0x0,
	3,0x0,
	2,0x0,
	2,0x0,
	3,0x0,
	8,0x29FC,
	2,0x0,
	1,0x0,
	1,0x0,
	7,0x29FC,
	1,0x0,
	3,0x0,
	6,0x0,
	6,0x0,
	2,0x0,
	4,0x0,
	4,0x0,
	2,0x0,
	2,0x0,
	2,0x0,
	2,0x0,
	2,0x0,
	4,0x0,
	4,0x0,
	3,0x0,
	2,0x0,
	2,0x0,
	3,0x0,
	3,0x0,
	8,0x88DE,
	2,0x0,
	1,0x0,
	3,0x0,
	2,0x0,
	5,0x0,
	5,0x0,
	4,0x0,
	4,0x0,
	4,0x0,
	4,0x0,
	4,0x0,
	4,0x0,
	4,0x0,
	4,0x0,
	1,0x0,
	4,0x0,
	2,0x0,
	4,0x0,
	6,0x0,
	3,0x0,
	3,0x0,
	4,0x0,
	4,0x0,
	6,0x0,
	3,0x0,
	1,0x0,
	3,0x0,
	3,0x0,
	3,0x0,
	2,0x0,
	1,0x0,
	3,0x0,
	3,0x0,
	3,0x0,
	3,0x0,
	3,0x0,
	2,0x0,
	6,0x0,
	4,0x0,
	4,0x0,
	1,0x0,
	3,0x0,
	1,0x0,
	3,0x0,
	1,0x0,
	1,0x0,
	1,0x0,
	1,0x0,
	1,0x0,
	1,0x0,
	1,0x0,
	5,0x0,
	5,0x0,
	5,0x0,
	2,0x0,
	2,0x0,
	2,0x0,
	4,0x0,
	4,0x0,
	4,0x0,
	4,0x0,
	1,0x0,
	3,0x0,
	1,0x0,
	4,0x0,
	4,0x0,
	4,0x0,
	1,0x0,
	4,0x0,
	4,0x0,
	4,0x0,
	6,0x0,
	3,0x0,
	2,0x0,
	2,0x0,
	2,0x0,
	2,0x0,
	2,0x0,
	2,0x0,
	2,0x0,
	2,0x0
};
int arraysize = sizeof(mvkeyarray) / sizeof(mvkeyarray[0]);
int movesinarray = arraysize/2;



//
int moves = 0;
//move code
void yourMove() {
	//if (moves > 2050) {
	//	return;
	//}
	if (moves > arraysize) {
		printf("moves:%d : arrm:%d , arrsz:%d", moves, movesinarray, arraysize);
		return;
	}
	sentOption(mvkeyarray[moves], mvkeyarray[moves+1]);
	moves++;
	moves++;
}


void x()
{

	printf("\n<><><><><><><><>\n");
	printf("#%X#\n", getrandomdir());
	if (moves > 2050) {
		return;
	}
	
	printf("=========\n");
	pickup_keys_in_room();
	pickup_items_in_room();
	printf("=========\n");
	process_room_events();
	printf("=========\n");
	//getchar();
	//MOVE CODE
	

	if ((room.direction[DIRECTION_NORTH] == DIRECTION_OPEN) && (last_direction != DIRECTION_SOUTH)) {
		sentOption(OPTION_MOVE_NORTH, 0);
		last_direction = DIRECTION_NORTH;
	}
	else if ((room.direction[DIRECTION_SOUTH] == DIRECTION_OPEN) && (last_direction != DIRECTION_NORTH)) {
		sentOption(OPTION_MOVE_SOUTH, 0);
		last_direction = DIRECTION_SOUTH;
	}
	else if ((room.direction[DIRECTION_EAST] == DIRECTION_OPEN) && (last_direction != DIRECTION_WEST)) {
		sentOption(OPTION_MOVE_EAST, 0);
		last_direction = DIRECTION_EAST;
	}
	else if ((room.direction[DIRECTION_WEST] == DIRECTION_OPEN) && (last_direction != DIRECTION_EAST)) {
		sentOption(OPTION_MOVE_WEST, 0);
		last_direction = DIRECTION_WEST;
	}
	else if ((room.direction[DIRECTION_UP] == DIRECTION_OPEN) && (last_direction != DIRECTION_DOWN)) {
		sentOption(OPTION_MOVE_UP, 0);
		last_direction = DIRECTION_UP;
	}
	else if ((room.direction[DIRECTION_DOWN] == DIRECTION_OPEN) && (last_direction != DIRECTION_UP)) {
		sentOption(OPTION_MOVE_DOWN, 0);
		last_direction = DIRECTION_DOWN;
	}
	// locked keys
	else if ((room.direction[DIRECTION_NORTH] == DIRECTION_LOCKED) && (last_direction != DIRECTION_SOUTH)) {
		sentOption(OPTION_UNLOCK_NORTH, Known_keys[try_key]);
		printf("ul:n %X", Known_keys[try_key]);
		try_key++;
	}
	else if ((room.direction[DIRECTION_SOUTH] == DIRECTION_LOCKED) && (last_direction != DIRECTION_NORTH)) {
		sentOption(OPTION_UNLOCK_SOUTH, Known_keys[try_key]);
		printf("ul:s %X", Known_keys[try_key]);
		try_key++;
	}
	else if ((room.direction[DIRECTION_EAST] == DIRECTION_LOCKED) && (last_direction != DIRECTION_WEST)) {
		sentOption(OPTION_UNLOCK_EAST, Known_keys[try_key]);
		printf("ul:e %X", Known_keys[try_key]);
		try_key++;
	}
	else if ((room.direction[DIRECTION_WEST] == DIRECTION_LOCKED) && (last_direction != DIRECTION_EAST)) {
		sentOption(OPTION_UNLOCK_WEST, Known_keys[try_key]);
		printf("ul:w %X", Known_keys[try_key]);
		try_key++;
	}
	/*else if ((room.direction[DIRECTION_UP] == DIRECTION_LOCKED) && (last_direction != DIRECTION_DOWN)) {
	sentOption(OPTION_UNLOCK_UP, Known_keys[try_key]);
	try_key++;
	}
	else if ((room.direction[DIRECTION_DOWN] == DIRECTION_LOCKED) && (last_direction != DIRECTION_UP)) {
	sentOption(OPTION_UNLOCK_DOWN, Known_keys[try_key]);
	try_key++;
	}*/
	
	//
	else {
		sentOption(OPTION_MOVE_NORTH,0);
		last_direction = DIRECTION_NORTH;
		try_key = 0;
		
		
	}

	//if (strcmp(room_name, room.name) != 0) option_count = 0;

	//sentOption(options[option_count], 0x1234);

	//option_count = (option_count + 1) % number_of_options;
	//option_count = OPTION_MOVE_NORTH;;
	/*char x[255];
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
	sentOption(option_count, 0x1234);*/
	moves++;
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

