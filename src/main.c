#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

typedef struct backup_list {
	char order[10];
	char filename[255];
	char period[3];
	char option;
	struct backup_list* next;
} list;

bool is_vaild_path (char path[]) { // 경로를 검사하는 함수.
	DIR* dp = NULL;
	int i = 0;

	while (1) { // 입력받은 경로에 공백이 있으면, 경로가 2개 입력되었다 판단.
		if (path[i] == '\0') {
			break;
		}
		if (path[i] == ' ') {
			return false;
		}
		i++;
		}

	dp = opendir(path);

	if (dp == NULL) { // name is not dir, reject permission, not find dir 
		closedir(dp);
		return false;
	}
	else {
		printf("정상적인 경로입니다.\n");
		closedir(dp);
		return true;
	}
}

void prompt() {
	char order[300] = {0,};
	int flag = 0;
	
	printf("20170819>");

	fgets(order,300,stdin);
	order[strlen(order)-1] = '\0';
	
	char* token = strtok(order," ");
	
	list* head = (list*)malloc(sizeof(list));
	head->next = NULL;

	while (token != NULL) { 
		list* bcklist = (list*)malloc(sizeof(list));
		
		if (flag = 0) {
			strcpy(bcklist->order,token);
			printf("%s %d order\n",token,flag);
		}
		else if (flag = 1) {
			strcpy(bcklist->filename,token);
			printf("%s %d filename\n", token,flag);
		}
		else if (flag = 2) {
			strcpy(bcklist->period,token);
			printf("%s per\n",token);
		}
		token = strtok(NULL," ");
		flag++;
		bcklist->next = head->next;
		head->next = bcklist;
	}
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

system("clear");
prompt();
}

