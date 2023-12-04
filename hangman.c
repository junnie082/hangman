#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#define MAX 1000

int main() {

    FILE *fp = NULL; 
    char str[100]; 
    char words[11][100]; 

    fp = fopen("answer.txt", "r"); 

    if (fp == NULL) {
        printf("Failed to open the file."); 
    } 

    int index = 1;
    while (fgets(str, sizeof(str), fp) != NULL) {
        strcpy(words[index], str); 
        index++;  
    }

    int round; 
    printf("라운드를 선택하세요: 1부터 10\n"); 
    scanf("%d", &round);

    printf("게임 시작합니다!\n"); 

    int chance = 7; 
    char guess, word[100]; 
    strcpy(word, words[round]); 

    int size = strlen(word) - 1; // 마지막의 \n 은 빼기 위해 -1을 해줌 

    char answerSheet[100]; 
    for (int i = 0; i < 100; i++) {
       answerSheet[i] = '_'; 
    }

    // 글자수 출력하기. 
    for (int i = 0; i < size; i++) {
        printf("_ "); 
    }
    
    int number = 0; 
    while (chance != 0) {
        number++;
        printf("\n영어 알파벳(소문자만 입력하세요)을 고르세요. 기회는 %d 번 주어집니다.\n", chance);
        scanf(" %c", &guess); 
        
        int correct = 0; 
        for (int i = 0; i < size; i++) {
            if (word[i] == guess) {
                answerSheet[i] = guess;
                correct = 1;
            }
        }

        if (correct == 0) {
            chance--;
            printf("틀렸습니다! 기회가 %d 로 줄어듭니다. \n", chance);
        }

        // 출력하기. 
        for (int i = 0; i < size; i++) {
            printf("%c ", answerSheet[i]);
            if (answerSheet[i] == '_') {
                correct = 0;
            }
        }

        // 글자를 단 하나도 맞추지 못했을 때. 
        if (correct == 1) {
            printf("%d번 만에 정답을 맞추었습니다!\n", number); 

            exit(0); 
        }
    }
    
    printf("정답을 맞추지 못했습니다!"); 
    // fclose(fp); 
}
