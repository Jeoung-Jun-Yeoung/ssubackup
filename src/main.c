#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

typedef struct backuplist {
	char order[10];
	char filename[255];
	char period[3];
	char option;
	struct backuplist* next;
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

bool is_vaild_order (char order[]) {

	char useorder[8][15] = {"add", "remove","compare","recover","list","ls","vi(m)","exist"};
	int checkorder = 3;

	if(order[0] == '\n'){
		printf("ERROR");
		return false; // 입력이 없으면  에러처리후 프롬포트로.
	}

	char* token = strtok(order," ");

	while (token != NULL) { 
		if (i == 0) {

			for(int i = 0; i < 8; i++ ){

				checkorder = strcmp(token,useorder[i]);// 명령어가 담긴 토큰과 비교결과를 담아서 아래조건으로 확인.

				if (checkor == 0){ // 명령어가 존재하면 반복문을 빠져나와 다음조건 확인.
					break;
				}
				else if (i == 7) {
					return false; //다 확인했는데, 명령어가 없으면 f리턴
				}
			}

			else if (i == 1){
				int flieexist = access(token,F_OK);
				
				if (flieexist =  -1) {
					return false; // 파일이 존재하지 않다면 false
				}
			
			}

			else if (i == 2) {

				strcpy(bcklist->period,token);
			}
			i++;
			token = strtok(NULL," ");
		}
	}
}
void prompt() {
	char order[500];
	int  i = 0;

	system("clear");

	printf("20170819>");

	fgets(order,sizeof(order),stdin);
	order[strlen(order)-1] = '\0';

	is_vaild_order(order);

	char* token = strtok(order," ");

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
	head->next = bcklist;
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

	prompt();
}

