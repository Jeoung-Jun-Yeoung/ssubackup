# ssubackup

# ssubackup develop record

###20210217 Study

1. 프로세스란?
- 프로그램의 실행 단위. 컴퓨터는 병렬처리가 가능하다. 즉, 여러 프로그램을 동시에 수행하는것처럼 보이지만, 사실은 여러 프로그램을 빠르게 전환하면서 수행하는 것이다. 이때 각 프로그램에게 가상메모리를 제공하고, 실제 메모리를 사용해야 할때마다 할당해서 프로그램이 실행이 된다. 
프로그램이 좀더 빨리 수행된다면 전환이 더 빨라질수있지 않을까? 어떻게 하면 더 빠르게 수행할수있을까. 

2. 쓰레드란?
- 쓰레드는 프로세스의 단위라고 볼수도 있다. 정확하게는 흐름이다. 프로세스는 1개이상의 쓰레드를 가진다. 어떤 프로세스가 연산과 출력을 수행한다면, 각각을 쓰레드에게 나눠서 실행할수있다. 그러나 이때 쓰레드의 충돌이 발생할수있다. 변경값을 가져와야 하는 상황에서 이전값을 가져올수도 있기 때문이다. 그래서 mutex를 통해 우선순위를 정해주어야 한다. 일종의 flag다.


###20210219 Study


경로를 입력받아서 유효성검사하는 함수 구현 완료.

이제 add명령어 구현중. 입력받은 명령어를 다시 공백기준으로 쪼개는것까지 구현함.

이것을 백업파일 리스트의 구조체에 각각 다시 넣어서 보관하는것을 구현할 예정.
그리고 필요한것을 여기서 뽑아서 add를 포함한 나머지 기능을 구현할 계획.

주말까지 구조체와 add명령어 구현예정. log파일 작성하는것은 가장 마지막에 구현할예정.


###20210220


백업파일리스트 구조체 구현 완료. 토큰별로 쪼개서 명령어의 유효성을 검사하는 함수 제작. 구현 완료했고, 디테일 잡는 디버깅중.

명령어 특히 add명령어를 구현할 예정. 토큰별로 쪼개서 파일리스트구조체에 담는것은 성공함.

주말간 add 구현예정.



###20210221

1. cp명령어로 콘솔에 입력해서 파일 복붙하기. 
2. 주기마다 해야하니까 삭제했다가 다시만들기.
3. pwd로 상대경로 -> 절대경로로 만드는것 공부하기
4. 경로 인자 입력받을때 argc, argv로 받는것 수정하기



###20210222

1. 경로입력 main에서 fget로 입력받는것으로 착각함. 그게 아니라 파일 실행시에  main의 인자로 넘겨줘야 하는거였음. 명세를 다시 읽어보고, 분석해서
argv, argc로 입력받아서 경로의 유효성을 검사받는것으로 수정함. 이과정에서 시간이 오래 소모되었는데, main에서 argc와 argv의 인자를 받는 순서를 반대로 하고있었음. 이가 문제임을 가장 늦게 깨달아서 조금 시간이 오래걸렸던것으로 파악. 후에는 예시 코드를 꼭 잘 보고 적용해야겠다 생각함.

2. add 명령어에서 명령어의 유효조건들은 전부다 검사해서 유효한지 안한지 판단하는 함수는 구현함. 이 과정에서 list구조체의 생성도 확인했음. 처음에 노드가 생길때마다 head를 만드는 코드로 짜서 디버깅의 시간이 오래걸림. 리스트자료구조를 다시 공부하게됨.  리스트의 head를 전역변수로 선언해주었다가,  그냥 메인에서 prompt함수가 실행전 선언해서 인자로 넘겨주는 방식으로 바꿈, 혹시 나중에 또 main에서 다른함수의 인자로 넘겨줄 일이 있을수도 있다고 판단했기때문,

3. 절대경로는 잘 받아지는데, 상대경로를 입력받았을때 절대경로로 어떻게 만들지 고민중.

4. add 구현은 system콜을 해서 입력받은 파일을 copy 하는 방향으로 생각중. 아니면, 그 파일을 fopen해서 읽고 써야할것도 고려중.

5. 현재까지 작업의 가장 큰 문제는, 상대경로에 대한 처리가 안된다는점. 
realpath 함수로 상대경로 > 절대경로 변환이 가능함을 알게되어서 상대경로 입력시 이 함수를 통해 상대경로 입력시 절대경로로 변환할 함수를 구햔 예정

6. prompt 반복문 버그 발생. 현재 디버깅중. exist명령어는 잘 작동함.


###20210223 study

1. 입력에 대한 처리를 전부 구현함. 

내용으로는 main에서 fgets로 입력받던것을 argv형식으로 파일실행하면서 입력받는것으로 다 수정함. 그리고 이에대한 유효성검사함수도 다시 짬. (기존은 고수준, 저수준입출력형식으로 구분. 지금 수정한 내용은 stat함수를 이용)

2. 그래서 절대경로입력에 대해서는 디버깅까지 완료.(상대경로바꾸는함수구현중)

3. 추가로 절대경로를 입력해서 prompt에서 명령어중 add를 70%정도 구현함.

4. add명령어에 대한 예외처리를 구현. filename, period등 기타 에러처리해야 할 부분을 구현해서 이가 동작하는것까지 확인했고. 디버깅도 완료.

5. 어제 생긴 prompt버그 잡았음. exist입력시 탈출조건 코드가 조금 더러워서 다시 설계할 예정.

---------------

현재부터 해야할것. 

이제 add명령어를 통해서 유효성검사가 완료된 파일들을 백업폴더에 백업하는것을 구현해야함. 

이때 시스템을 콜해서 cp명령어? 를 사용할지, 아니면 fopen해서 읽으면서 쓸지 고민중.


두번째로, 상대경로 처리를 해야함. 

상대경로처리는 지금 구현중에 있고,수시간내로 디버깅까지 할 예정.



### 20210224 study

상대경로 to 절대경로 함수구현완료.

명령어로 백업폴더에 파일을 카피하기 위해서 현재 작업폴더를 백업폴더로 이동하는 함수 공부해 사용.

list함수와 add 함수 구현중 

list명령어부터 구현하고 있는데 무한루프발생해서 디버깅중. 

빠르게 해결하고 목요일까지 add구현예정.


### 20210225 study 

list 명령어 구현완료.

어제 list디버깅을 오늘까지 이어서 했는데, 시간이 매우 오래걸림. 

디버깅하면서 배운점이 많아서 따로 적어두고 세겨두자.

코드를 적는데 실수했는데, 맞다고 생각하고 다른데서 문제점을 찾으려고 하니 안보였음. 
틀린곳을 찾을때 틀린곳이 없다고 생각하니 멘탈이 나갔다. 내가생각했을때는 틀린게 없는데 찾아야 하니까 뭘 괜히 멀쩡한 코드도 건들여 더 해결이 어려워 졌다. 결국 배운 교훈은 디버깅시 틀렸을만한곳(의심지점)을 먼저 디버깅한다. 그리고 문제가 아니라면 그 다음으로 틀리기 쉬운곳을 디버깅 한다. 마지막으로 여기까지도 없다면 내가 맞다고 생각하는 부분들을 디버깅 해본다. 

list디버깅의 문제는 다음과 같았음.

첫번째로 문제는 cur = cur -> next; 가 되어야 하는데 반대로 cur -> next = cur;로 코드를 짰음. 그냥 맞다고 생각하니까 이 내용이 틀린것을 인지를 전혀 못함. 2~3시간동안 한줄씩 코드의 결과를 종이에 그려보다가 이 내용을 보고 틀린것을 깨닫게됨. 이전의 단어프로그램을 짯을때 연결리스트구현을 했던것을 참고했으면, 더 빨리깨달았을텐데 아쉬움이 매우 남고, 여기서 체력과 시간을 많이 뺏김.

두번째로 add 명령어의 유효성을 검사할때, 입력한 파일이 이미 기존 백업리스트에 존재하는지 판단하기 위해서 리스트 탐색함수를 만들었었음. 그런데 여기서 cur을 동적할당한 후에, cur = head로 놓고 NULL이 아닐때까지 잘 탐색을 한 다음에 cur을 free해주었음. 그런데 문제가 free하면서 이유는 모르나 head도 같이 free가 되버렸음. 그래서 결론적으로는 1번째에서는 잘 작동하나 2번째부터 문제가 생겼었음. cur은 쓰임이 끝났다고 판단해 free를 해주었으나 해주면 안되었음. 그래서 free코드를 지우고 첫번째 문제도 해결하니 list명령어가 잘 작동함.

허무했던것은 add안에 리스트탐색함수에서는 cur = cur -> next;로 잘 코딩했었다는것. 낫놓고 기억자 모르는 상황이었다. 스스로 너무 한심했다.


이후에는 add에 사용할 리스트에 존재하는 파일들을 백업하기위해 파일을 카피할 함수를 설계했고, 따로 쓰레드를 한번 사용해보려고 c파일을 만들어 쓰레드를 연습해봄. 

최종결론
add 명령어 구현
list 명령어 구현
(위 두개는 디버깅까지함)
쓰레드 연습함
cp함수 설계 (내일 구현예정)


