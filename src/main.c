#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

bool is_vaild_path(char path[]) { // 4개중 3개조건은 opendir로 검사가능. 인자 2개는 어떻게?
	DIR* dp = NULL;

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

int main() {

	char path [300] = {0,}; // 경로를 입력받는 변수.
	char foldername [8] = "/backup";
	char strbuffer [300] = {0,};
	char* pstrbuffer = NULL;

	fgets(path,300,stdin); 

	if(path[0] == '\n') { // 입력이 안되면,

		pstrbuffer = getcwd(strbuffer,300); // 현재 디렉토리 경로얻기.
		strcat(strbuffer,foldername); // 현재 디렉토리경로 + 디렉토리이름

		int dir_result = mkdir(strbuffer,0755);

	}
	else { // 경로입력이 되었다면,
		/*
		   경로를 유효성검사함수에 보내서 확인해야함.
		   이후에 합친다. 아래코드로 입력한 경로에  backup폴더 생기는건 확인함.
		 */
		path[strlen(path)-1] = '\0'; // 개행제거

		if (is_vaild_path(path) == 0) {
			printf("uasge");
			return 0;
		}
		else {
			strcat(path,foldername);

			int dir_result = mkdir(path,0755);

			if(dir_result == 0) {
				printf("폴더 생성 성공");
			}
			else {
				printf("실패");
			}
		}
	}
}
