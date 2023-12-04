#include <stdio.h> // receiver.c #include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <string.h>
#define DEFINED_KEY 0x10101010
main(int argc, char **argv)
{
    int msg_qid;
    struct {
        long mtype;
        char content[256];
    } msg;

    fprintf(stdout, "=========RECEIVER==========\n");
    if((msg_qid = msgget(DEFINED_KEY, IPC_CREAT | 0666)) < 0) {
        perror("msgget: "); 
        exit(-1); 
    }
    
    memset(msg.content, 0x0, 256); 
    if(msgrcv(msg_qid, &msg, 256, 0, 0) < 0) {
        perror("msgrcv: "); exit(-1);
    } 

    int chance = 7; 
    char guess, word[100]; 
    
    strcpy(word, msg.content);

    // hangman game start   

    printf("게임 시작합니다!\n"); 

 
    int size = strlen(word); // 마지막의 \n 은 빼기 위해 -1을 해줌 

    char answerSheet[100]; 
    for (int i = 0; i < size; i++) {
       answerSheet[i] = '_'; 
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
    
}