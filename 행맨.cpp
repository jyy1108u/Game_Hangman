#define _CRT_SECURE_NO_WARNINGS

#include <bangtal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

//전역변수 선언
SceneID gamemain;

ObjectID line[8]; //라인 8개
ObjectID Letter[8]; //알파벳 8개
ObjectID inputbutton; //알파벳 입력버튼

ObjectID keyboard; //키 배경
ObjectID Key[26]; //알파벳 누르는 키
ObjectID EnterB; //확인버튼

ObjectID key_ef[26]; //클릭효과
ObjectID key_x[26]; //닫기효과

ObjectID hangman; //행맨

ObjectID closeB; //게임 종료 버튼

int ANum = 0; //답 번호(배열 기준)
int Llength = 0; //답의 글자수
int countBlank = 100; //맞추지 않은 스펠링 갯수

bool selected; //골랐는가? 일단은 전부 거짓.. 인데..
int selectedNum = -1; //우선 선택된번호 없는걸로... 해둠

bool Winclude = false; //글자포함여부
int hangmanNum = 0; //행맨넘버


// 단어 목록을 배열로 만든다. 배열로 된거??? 를 어떻게함.? ok
// 배영로 만들어서 랜덤으로 답을 추출한다
// 입력 버튼을 만들고, 키패드를 눌러 입력한다.
// 답을 스캔해서 입력되면, 답과 입력된 키를 비교한다.
// 만일 중간에 같은 것이 있다면, 해당 자리를 해당 알파벳으로 변경한다.
// 만일 끝까지 가도 같은 것이 없다면, 행맨을 하나 추가한다.
// 행맨이 모두 끝까지 가면 패배, 답이 모두 true로 변경되면 승리한다.


//오브젝트 배치용 함수 선언(show여부는 제외함)
ObjectID CreateObject1(const char* Obimage, int x, int y) {
	ObjectID object = createObject(Obimage);
	locateObject(object, gamemain, x, y);
	return object;
}

const char* answer[12] = { //답 목록 추가가능.
	"apple","galaxy", "bangtal", "computer", "google", "english", "program", "zombie", "white", "black", "coffee", "game" };


void initgame() { //게임 시작 시 실행. 답을 고른다.

	srand((unsigned int)time(NULL));

	ANum = rand() % 12; //답 랜덤뽑기.
	Llength = strlen(answer[ANum]); //길이 구하기
	countBlank = Llength; //공백갯수 = 글자길이로 초기화

	printf("%s", answer[ANum]); printf("%d", Llength);

	for (int i = 0; i < Llength; i++) {
		setObjectImage(line[i], "images/line.png"); //답 글자수만큼 라인 생성
	}
}


void finishGame() { //게임 끝났는지 판별하는 함수

	if (hangmanNum == 9) { //행맨넘버가 9가 되어버리면
		ObjectID window = CreateObject1("images/Lose.png", 0, 0); showObject(window);
		closeB = CreateObject1("images/close.png", 498, 249); showObject(closeB);
	}

	if (countBlank == 0) { //공백갯수가 0이 되면
		ObjectID window = CreateObject1("images/Win.png", 0, 0); showObject(window);
		closeB = CreateObject1("images/close.png", 498, 249); showObject(closeB);
	}

}

void matchWord() { //글자가 포함되는지 확인해주고 결과 표시하는 함수

	if (selected) { //선택 후 ENTER 눌렀을때만 작용. 선택 안한채로 ENTER 누르면 무시

		char changeNumToWord = 97 + selectedNum; //아스키코드 이용해 A~Z 식별-

		for (int i = 0; i < Llength; i++) { //답의 글자수만큼.
			if (answer[ANum][i] == changeNumToWord) { //만약 답의 n번째글자가 같다면?
				char images[40]; //문자열 저장할 배열 선언
				sprintf(images, "images/L%d.png", selectedNum);
				setObjectImage(Letter[i], images);
				Winclude = true; //포함된것 확인해줌.
				countBlank--; //공백갯수 줄이기!
			}
			else { //같지 않다면?
				if (i == Llength - 1 && !Winclude) { //마지막 글자이고 여전히 불포함상태라면?
					char images[40]; //문자열 저장할 배열 선언
					sprintf(images, "images/h%d.png", hangmanNum);
					setObjectImage(hangman, images); //행맨 이미지 변경
					hangmanNum++; //행맨 숫자 증가
				}
			}
		}

		showObject(key_x[selectedNum]); //키_x를 활성화

		Winclude = false; //체크 끝났으니 꺼줌.

		selected = false; //선택취소
		selectedNum = -1; //번호 초기화

		finishGame();
	}
}


void mouseCallack1(ObjectID obj, int x, int y, MouseAction act) {

	if (obj == closeB) endGame(); //닫기버튼 누르면 게임 종료

	if (obj == inputbutton) { //인풋 버튼 누르면

		showObject(keyboard);
		for (int i = 0; i < 26; i++) {
			showObject(Key[i]); //키 보이기
			setObjectImage(key_x[i], "images/K_died.png"); //보여진 key_x는 모두 x자표시하기!
		}
		showObject(EnterB);

	}

	if (obj == EnterB) { //확인버튼 누르면?

		hideObject(keyboard);
		for (int i = 0; i < 26; i++) {
			hideObject(Key[i]);
			hideObject(key_ef[i]);
			setObjectImage(key_x[i], "images/N.png"); //투명화시키기
		}
		hideObject(EnterB);

		matchWord();
	}

	for (int i = 0; i < 26; i++) { //키보드 키
		if (obj == Key[i]) { //키보드 키 중에 하나 누르면
			if (!selected) { //선택 안된상태면
				selectedNum = i; //선택된번호를 i번으로 변경
				selected = true; //선택상태를 ON
				showObject(key_ef[i]); //키 선택된거 보여줌
			}
			else { //이미 다른게 선택된상태면?
				hideObject(key_ef[selectedNum]); //선택된건 숨김
				showObject(key_ef[i]);
				selectedNum = i; //번호 재저장하기
			}
		}

		if (obj == key_ef[i]) { //선택된거 눌렀을때
			hideObject(key_ef[i]); //숨기고
			selectedNum = -1; //번호변경
			selected = false; //선택취소
		}
	}
}

int main()
{
	gamemain = createScene("행맨", "images/gamemain.png");

	//오브젝트 생성
	inputbutton = CreateObject1("images/button.png", 900, 140); showObject(inputbutton);
	hangman = CreateObject1("images/N.png", 275, 236); showObject(hangman);
	
	for (int i = 0; i < 8; i++) {
		line[i] = CreateObject1("images/N.png", 385 + 102 * i, 270);
		Letter[i] = CreateObject1("images/N.png", 394 + 101 * i, 283);
		showObject(line[i]); showObject(Letter[i]);
	}

	//키보드
	keyboard = CreateObject1("images/Key.png", 220, 173);
	EnterB = CreateObject1("images/KN.png", 800, 188);
	
	for (int i = 0; i < 26; i++) {
		char image[20]; //이미지 이름
		sprintf(image, "images/K%d.png", i); //0~25까지 집어넣기
		
		if (i < 8) {
			Key[i] = CreateObject1(image, 259 + 91 * i, 437);
			key_ef[i] = CreateObject1("images/K_selected.png", 255 + 91 * i, 435);
			key_x[i] = CreateObject1("images/K_died.png", 255 + 91 * i, 435);
			
		}
		else if (i < 16) {
			Key[i] = CreateObject1(image, 259 + 91 * (i-8), 354);
			key_ef[i] = CreateObject1("images/K_selected.png", 255 + 91 * (i-8), 350);
			key_x[i] = CreateObject1("images/K_died.png", 255 + 91 * (i - 8), 350);
		}
		else if (i < 24) {
			Key[i] = CreateObject1(image, 259 + 91 * (i-16), 271);
			key_ef[i] = CreateObject1("images/K_selected.png", 255 + 91 * (i-16), 265);
			key_x[i] = CreateObject1("images/K_died.png", 255 + 91 * (i - 16), 265);
		}
		else {
			Key[i] = CreateObject1(image, 259 + 91 * (i-24), 188);
			key_ef[i] = CreateObject1("images/K_selected.png", 255 + 91 * (i-24), 180);
			key_x[i] = CreateObject1("images/K_died.png", 255 + 91 * (i - 24), 180);
		}



	}

	//기본 레이아웃 감추기
	setGameOption(GameOption::GAME_OPTION_INVENTORY_BUTTON, false);
	setGameOption(GameOption::GAME_OPTION_MESSAGE_BOX_BUTTON, false);
	setGameOption(GameOption::GAME_OPTION_ROOM_TITLE, false);

	initgame();

	setMouseCallback(mouseCallack1);

	startGame(gamemain);
   
}
