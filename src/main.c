#include "backup.h"

typedef struct backuplist {
	char filename[255];
	char option;
	int nowsec;
	int period;
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

char* now_time () {
	struct tm *tn;
	time_t tnow;
	char static date[16];

	tnow = time(NULL);

	tn = (struct tm*)localtime(&tnow);
	sprintf(date,"%02d%02d%02d%02d%02d%02d",(tn->tm_year + 1900)-2000,tn->tm_mon + 1,tn->tm_mday,tn->tm_hour,tn->tm_min,tn->tm_sec);

	return date;
}

int sec () {
	struct tm *tn;
	time_t tsec;
	int static sec;

	tsec = time(NULL);

	tn = (struct tm*)localtime(&tsec);

	sec = tn->tm_sec;

	return sec;

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
void* copy_file (void *filelist) {

	int count;
	char buffer [5] = { 0, };
	char* nowtime;
	char strbuffer [300];
	char temp [50];
	char filename [100];
	char* wirtelog = " genrated";

	list* bcklist = (void*)filelist;

	getcwd(strbuffer,300);

	pthread_mutex_lock(&mutex);

	nowtime = now_time();
	strcpy(filename,bcklist->filename);

	char* token = strtok(filename,"/");

	while (token != NULL) { 
		strcpy(temp,token);
		token = strtok(NULL,"/");
	}

	strcat(strbuffer,"/");
	strcat(strbuffer,temp);
	strcat(strbuffer,"_");
	strcat(strbuffer,nowtime);

	FILE* src = fopen(bcklist->filename,"r");
	FILE* dest = fopen(strbuffer,"w");

	while (feof(src) == 0) {
		count = fread(buffer,sizeof(char),4,src);
		fwrite(buffer,sizeof(char),count,dest);
		memset(buffer,0,5);
	}

	strcat(strbuffer,wirtelog);

	FILE* fp = fopen("log.txt","a");
	fputs(nowtime,fp);
	fputs(" ",fp);
	fputs(strbuffer,fp);
	fputs("\n",fp);

	fclose(fp);
	fclose(dest);
	fclose(src);

	pthread_mutex_unlock(&mutex);
}

void* search_list (void *head) {
	while(1){	
		list* cur = (list*)head;
		sleep(1);
		int second = 0;
		second = sec();
		while (cur->next != NULL) {
			cur = cur->next;
			if(second % cur->period == 0){
				pthread_t backup;
				pthread_create(&backup,NULL,copy_file,(void*)cur);
			}
		}
	}
}
/*
void* remove_list (void* head){						

	list* front_target = (void*)filelist;

	while(front_target->next != NULL){

		front_target = front_target->next;

		if (strcmp(front_target->next->filename,tokenlist[1]) == 0) {
			pthread_mutex_lock(&mutex);

			FILE* fp = fopen("log.txt","a");
			nowtime = now_time();
			fputs(nowtime,fp);
			fputs(" ",fp);
			fputs(tokenlist[1],fp);
			fputs(" delete",fp);
			fputs("\n",fp);
			fclose(fp);

			front_target->next = front_target->next->next;

			pthread_mutex_unlock(&mutex);
		}
	}
}
*/
void prompt () {

	while (1) {
		char order[500] = {0,};
		char tokenlist[6][300] = {0,};
		char* nowtime;
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
			case 0: { if (tokenlist[1] == " ") {
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

					FILE* fp = fopen("log.txt","a");
					nowtime = now_time();
					fputs(nowtime,fp);
					fputs(" ",fp);
					fputs(bcklist->filename,fp);
					fputs(" added",fp);
					fputs("\n",fp);
					fclose(fp);

					break;}
			case 1: { 	/*
						pthread_t delete;

						pthread_create(&delete,NULL,remove_list,(void*)head);
						tokenlist와 head둘다 어떻게 넘기나?
						*/ 
						   list* front_target;
						   front_target = head;

						   while(front_target->next != NULL){
						   front_target = front_target->next;
						   if (strcmp(front_target->next->filename,tokenlist[1]) == 0) {

						   FILE* fp = fopen("log.txt","a");
						   nowtime = now_time();
						   fputs(nowtime,fp);
						   fputs(" ",fp);
						   fputs(tokenlist[1],fp);
						   fputs(" delete",fp);
						   fputs("\n",fp);
						   fclose(fp);

						   front_target->next = front_target->next->next;
						   }
						   }
					}

					break;
			case 2:

			case 3:

			case 4:{ list* temp;
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
		chdir(strbuffer);
		FILE* fp = fopen("log.txt","a");
		fclose(fp);
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
		chdir(argv[1]);	

		FILE* fp = fopen("log.txt","a");
		fclose(fp);
	}
	head = (list*)malloc(sizeof(list));
	head->next = NULL;

	pthread_t search;
	pthread_create(&search,NULL,search_list,(void*)head);

	prompt();

}
