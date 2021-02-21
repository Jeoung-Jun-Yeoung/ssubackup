#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

typedef struct backuplist {
	char filename[255];
	int period;
	char option;
	struct backuplist* next;
} list;

list* head = (list*)malloc(sizeof(list));

bool is_vaild_path (char path[]) { // 경로를 검사하는 함수.
	DIR* dp = NULL;

	dp = opendir(path);

	if (dp == NULL) { // 경로가 유효하지 않으면 NULL이 반환됨.
		return false;
	}
	else {
		printf("정상적인 경로입니다.\n");
		closedir(dp);
		return true;
	}
}

int is_vaild_order (char order[]) {

	char useorder[8][15] = {"add", "remove","compare","recover","list","ls","vi(m)","exist"};

	for(int i = 0; i < 8; i++ ){
		if (!strcmp(order,useorder[i])) { // 명령어가 존재하면 반복문을 빠져나와 다음조건 확인.
			return i;
		}
		else if (i == 7) {
			return i + 1; //다 확인했는데, 명령어가 없으면 f리턴
		}
	}		
	/*
	   else if (i == 1){
	   int flieexist = access(token,F_OK);

	   if (flieexist =  -1) {
	   return false; // 파일이 존재하지 않다면 false
	   }
	   }
	 */
}

list* head = (list*)malloc(sizeof(list));
head->next = NULL;

void prompt() {
	char order[500];
	char tokenlist[6][300];
	int i = 0;
	int a = 0;
// while exist 입력시 종료.
	system("clear");

	printf("20170819>");

	fgets(order,sizeof(order),stdin);
	order[strlen(order)-1] = '\0';

	char* token = strtok(order," ");

	while(token != NULL){
		strcpy(tokenlist[i],token);
		token = strtok(NULL," ");
		i++;
	}

	a = is_vaild_order(tokenlist[0]);
	
	switch (a) 
	{
		case 0:
			if (tokenlist[1] == " ") {
				printf("ERROR");
				// go prompt
			}
			
			FILE* fp = fopen(tokenlist[1],"r"); // file이 존재하지 않음,fopen에서는  특수파일이 열리지않음.
			if (fp == NULL) {
				printf("ERROR");
				// go prompt
			}
/*
	  		list* cur = (list*)malloc(sizeof(list));
			cur = head;
			
			while (cur->next != NULL) {
				if (!strcmp(cur->filename,tokenlist[1])){
					printf("ERROR");
					//go prompt
				}
				cur->next = cur;
			}*/
			// filename에 대한 유효성검사 끝.
			double period = atof(tokenlist[2]);
			int turncated = (int)period;
			if (period != turncated) {
				printf("ERROR");
				//go prompt
			}
			//period 유효성 검사 끝.

			list* list = (list*)malloc(sizeof(list));
			strcpy(list->filename,tokenlist[1]);
			list->period = turncated;
			list->next = head->next;
			head->next = list;

			break;
		case 1:

		case 2:

		case 3:

		case 4:

		case 5:

		case 6:

		case 7:

		case 8:

		default:

			break;

	}


	/*
	   list* head = (list*)malloc(sizeof(list));
	   head->next = NULL;

	   list* bcklist = (list*)malloc(sizeof(list));

	   while (token != NULL) { 
	   if (i == 0) {
	   strcpy(bcklist->order,token);

	   }
	   else if (i == 1){
	   strcpy(bcklist->filename,token);
	   }
	   else if (i == 2) {

	   strcpy(bcklist->period,token);
	   }
	   i++;
	   token = strtok(NULL," ");
	   }
	   bcklist->next = head->next;
	   head->next = bcklist;*/
}

int main() {

	char path [300] = {0,}; // 경로를 입력받는 변수.
	char foldername [8] = "/backup";
	char strbuffer [300] = {0,};
	char* pstrbuffer = NULL;

	fgets(path,300,stdin); 

	if (path[0] == '\n') { // 입력이 안되면,

		pstrbuffer = getcwd(strbuffer,300); // 현재 디렉토리 경로얻기.
		strcat(strbuffer,foldername); // 현재 디렉토리경로 + 디렉토리이름

		int dir_result = mkdir(strbuffer,0755);

	}
	else { // 경로입력이 되었다면,
		/*
		   경로를 유효성검사함수로 보낸다. 이후 결과에 따라 백업폴더 생성.
		 */
		path[strlen(path)-1] = '\0'; // 개행제거

		if (is_vaild_path(path) == 0) {
			printf("uasge");
			return 0;
		}
		else {
			strcat(path,foldername);

			int dir_result = mkdir(path,0755);
			//여기 아래는 없어도 됨. 폴더생성여부확인차 넣은 코드.
			if(dir_result == 0) {
				printf("폴더 생성 성공");
			}
			else {
				printf("실패");
			}
		}
	}
	head->next = NULL;
	prompt();
}

