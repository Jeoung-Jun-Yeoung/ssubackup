#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <pthread.h>

typedef struct backuplist {
	char filename[255];
	int period;
	char option;
	struct backuplist* next;
} list;

bool is_valid_path (char* path) { // 경로를 검사하는 함수.
	struct stat buf;

	stat(path,&buf);

	if (S_ISDIR(buf.st_mode)){
		return true;
	}
	else{
		return false;
	}
}

bool is_basic_file (char* filename) {
	struct stat buf;

	stat(filename,&buf);
	if(S_ISREG(buf.st_mode)){
		return true;
	}
	else {
		return false;
	}			   
}

char* change_repath2abpath (char* path) {
	char static changepath[300] = {0,};
	realpath(path,changepath);
	return changepath;
}

int is_valid_order (char order[]) {
	char useorder[8][15] = {"add", "remove","compare","recover","list","ls","vi(m)","exist"};
	for(int i = 0; i < 8; i++ ){
		if (!strcmp(order,useorder[i])) { // 명령어가 존재하면 반복문을 빠져나와 다음조건 확인.
			return i;
		}
		else if (i == 7) {
			return i + 1; //다 확인했는데, 명령어가 없으면 f리턴
		}
	}		
}

void prompt (list* head,char* argv) {
	//	chdir(argv); // 작업을 현재 디렉토리폴더로 이동.

	while (1) {
		char order[500] = {0,};
		char tokenlist[6][300] = {0,};
		int i = 0;
		int a = 0;

		//system("clear");
		printf("20170819>");

		fgets(order,sizeof(order),stdin);
		order[strlen(order)-1] = '\0';

		//		while(getchar() != '\n');
		char* token = strtok(order," ");

		while(token != NULL){
			strcpy(tokenlist[i],token);
			token = strtok(NULL," ");
			i++;
		}

		a = is_valid_order(tokenlist[0]);

		switch (a) 
		{
			case 0: {
						if (tokenlist[1] == " ") {
							printf("ERROR 공백 \n");
							break;
						}

						if (!is_basic_file(tokenlist[1])) { // file에 대한 유효성 검사.
							printf("ERROR 파일 유효성\n");
							break;
						}

						list* cur = (list*)malloc(sizeof(list));
						cur = head;

						while (cur->next != NULL) {
							if (strcmp(cur->filename,tokenlist[1]) == 0){
								printf("ERROR 이미 존재\n");
								break;
							}
							cur->next = cur;
						}
						free(cur);
						// 이미 기존 리스트에 존재하는지 검사.
						// filename에 대한 유효성검사 끝.
						if (tokenlist[2] == " "){
							printf("ERROR 주기가 입력안됨\n");
						}
						double period = atof(tokenlist[2]);
						int turncated = (int)period;
						if (period != turncated || turncated == 0) {
							printf("ERROR 정수형이아님\n");
							break;
						}
						//period 유효성 검사 끝.

						list* bcklist = (list*)malloc(sizeof(list));

						strcpy(bcklist->filename,tokenlist[1]);
						bcklist->period = turncated;

						bcklist->next = head->next;
						head->next = bcklist;
						break;}
			case 1:

			case 2:

			case 3:

			case 4:{
					   list* temp = (list*)malloc(sizeof(list));
					   temp = head;

					   while (temp->next != NULL) {
						   printf("%s %d\n",temp->filename,temp->period);
						   temp->next = temp;
					   }


					   break; }
			case 5:

			case 6:

			case 7:
				   break;

			default:
				   printf("ERROR");

				   break;

		}
		if (a == 7) {
			break;
		}
	}
}
int main(int argc,char* argv[]) {

	char foldername [8] = "/backup";
	char strbuffer [300] = {0,};
	char* pstrbuffer = NULL;
	if (argc == 1) { // 입력이 안되면,

		pstrbuffer = getcwd(strbuffer,300); // 현재 디렉토리 경로얻기.
		strcat(strbuffer,foldername); // 현재 디렉토리경로 + 디렉토리이름

		int dir_result = mkdir(strbuffer,0755);

	}
	else { // 경로입력이 되었다면,
		//   경로를 유효성검사함수로 보낸다. 이후 결과에 따라 백업폴더 생성.
		argv[strlen(argv[1])-1] = '\0'; // 개행제거

		if (argv[1][0] != '/') { // 상대경로라면 절대경로 변환.
			argv[1] = change_repath2abpath(argv[1]); 
		}

		if (!is_valid_path(argv[1])) {
			printf("uasge"); // 이후 경로가 디렉토리 파일이 맞는지 점검. 
			return 0;
		}
		else { // 다 통과하면 경로에 백업폴더 생성.
			strcat(argv[1],foldername);
			int dir_result = mkdir(argv[1], 0755);
			//여기 아래는 없어도 됨. 폴더생성여부확인차 넣은 코드.
			if(dir_result != 0) {
				printf("실패");
				return 0;
			}
		}
	}
	list* head = (list*)malloc(sizeof(list));
	head->next = NULL;
	prompt(head,argv[1]);
}
