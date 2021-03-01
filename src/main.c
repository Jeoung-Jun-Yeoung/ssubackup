#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <pthread.h>
#include <time.h>
#include "backup.h"

typedef struct backuplist {
	char filename[255];
	int period;
	char option;
	struct backuplist* next;
} list;

list* head = NULL;
pthread_mutex_t mutex;

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

// para : 경로 char* argv , 파일명 bcklist->filename, bcklist를 받아온다.
// 주기마다 실행 = bcklist->period
// 쓰레드함수로 만들어서 주기마다 카피 실행.
//병렬적으로 백업이 되어야 하기 때문.
// 
/*
void* copy_file (void* arg) {

	chdir(argv[1]); // 백업  디렉토리폴더로 이동.

	int count;
	char buffer [5] = { 0, };
	char backupname[200] = {0, };
	char time [50] = {0, };
	
	pthread_mutex_lock(&mutex);
	
	time_t t;
	struct tm* timeinfo;

	time(&t);
	timeinfo = localtime(&t);

	strcpy(time,asctime(timeinfo));

	strcpy(backupname,bcklist->filelist);
	strcat(backupname,time);

	FILE* src = fopen("tokenlist[1]","r");
	FILE* dest = fopen("backupname","w");

	while (feof(src) == 0) {
		count = fread(buffer,sizeof(char),4,src);
		fwrite(buffer,sizeof(char),count,dest);
		memset(buffer,0,5);
	}

	fclose(dest);
	fclose(src);

	pthread_mutex_unlock(&mutex);
}*/
void* search_list (void* head) {
	printf("검색 스레드 실행 된 유무확인");
/*	while(1){
		list* cur = (list*)head;
		while (cur->next != NULL) {
			cur = cur->next;
			printf("%s",cur->filename);

		}
	}*/
}

void prompt (char* argv) {

	while (1) {
		char order[500] = {0,};
		char tokenlist[6][300] = {0,};
		int i = 0;
		int a = 0;
		//system("clear");
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
			case 0: {	
						if (tokenlist[1] == " ") {
							printf("ERROR if  디버깅\n");
							printf("ERROR 공백 \n");
							break;
						}
						if (!is_basic_file(tokenlist[1])) { // file에 대한 유효성 검사.
							printf("ERROR 파일 유효성\n");
							break;
						}

						list* cur;
						cur = head;
						while (cur->next != NULL) {
							if (strcmp(cur->filename,tokenlist[1]) == 0){
								printf("ERROR 이미 존재\n");
								break;
							}
							cur = cur->next; // 리스트 중요한 부분.. 잘 기억하기
						}
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

						strcpy(bcklist->filename,tokenlist[1]); //file이름 넣어주기.
						bcklist->period = turncated; // 주기 넣어주기

						bcklist->next = head->next; // 리스트간 연결.
						head->next = bcklist;

						break;}
			case 1: { 
						list* front_target;
						while(front_target->next != NULL){
							front_target = front_target->next;
							if (strcmp(front_target->next->filename,tokenlist[1]) == 0) {
								front_target->next = front_target->next->next;
							}
						}
					}

					break;
			case 2:

			case 3:

			case 4:{
					   list* temp;
					   temp = head;
					   while (temp->next != NULL) {
						   temp = temp->next;
						   printf("%s %d\n",temp->filename,temp->period);
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
	head = (list*)malloc(sizeof(list));
	head->next = NULL;
	pthread_t search;
	prompt(argv[1]);
	pthread_create(&search,NULL,search_list,/*(void*)head*/NULL);

}
