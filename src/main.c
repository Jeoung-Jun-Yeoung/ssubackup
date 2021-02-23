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

void change_repath2abpath() {


// fullpath
// cd <path>; pwd

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
	/*
	   else if (i == 1){
	   int flieexist = access(token,F_OK);

	   if (flieexist =  -1) {
	   return false; // 파일이 존재하지 않다면 false
	   }
	   }
	 */
}

void prompt(list* head) {
	while (1) {
		char order[500];
		char tokenlist[6][300];
		int i = 0;
		int a = 0;
		
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

		a = is_valid_order(tokenlist[0]);
		
		switch (a) 
		{
			case 0:
				if (tokenlist[1] == " ") {
					printf("ERROR\n");
					break;
				}

				if (!is_basic_file(tokenlist[1])) { // file에 대한 유효성 검사.
					printf("ERROR\n");
					break;
				}
				
				   list* cur = (list*)malloc(sizeof(list));
				   cur = head;

				   while (cur->next != NULL) {
				   if (!strcmp(cur->filename,tokenlist[1])){
				   printf("ERROR\n");
				   break;
				}
				cur->next = cur;
				} // 이미 기존 리스트에 존재하는지 검사.
				// filename에 대한 유효성검사 끝.
				if (tokenlist[2] == " "){
					printf("ERROR\n");
				}
				double period = atof(tokenlist[2]);
				int turncated = (int)period;
				if (period != turncated || turncated == 0) {
					printf("ERROR\n");
					break;
				}
				//period 유효성 검사 끝.

				list* bcklist = (list*)malloc(sizeof(list));

				strcpy(bcklist->filename,tokenlist[1]);

				bcklist->period = turncated;
				bcklist->next = head->next;
				head->next = bcklist;
				// 아래도 리스트 확인차 만든 코드
				printf("%s\n",bcklist->filename);
				printf("%d\n",bcklist->period);

				break;
			case 1:

			case 2:

			case 3:

			case 4:

			case 5:

			case 6:

			case 7:
				break;

			case 8:

			default:

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

		is_valid_path(argv[1]);
		argv[strlen(argv[1])-1] = '\0'; // 개행제거
		
		if (!is_valid_path(argv[1])) {
			printf("uasge");
			return 0;
		}
		else {
			strcat(argv[1],foldername);
			int dir_result = mkdir(argv[1], 0755);
			//여기 아래는 없어도 됨. 폴더생성여부확인차 넣은 코드.
			if(dir_result == 0) {
				printf("폴더 생성 성공");
			}
			else {
				printf("실패");
			}
		}
	}
	list* head = (list*)malloc(sizeof(list));
	head->next = NULL;


	prompt(head);


}
