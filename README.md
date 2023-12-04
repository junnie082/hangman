# Hangman

https://github.com/junnie082/hangman/assets/88719152/1d615066-bf47-450f-8d75-3e9ad74fa4b0

* Presentation: https://docs.google.com/presentation/d/1nRBsFtkSS0PdYuIfni9KqZUkTmKnvFd8ba8aiNflY_0/edit?usp=sharing

## Rule

#### Sender
- The sender sends one specific word to the receiver.
- He can only send one word at a time.  
- The word consists of lower case letters in English, in any length.

#### Receiver
- The Receiver has to find the answer which the sender hides.
- Only 7 chances are given to guess the answer. 
- Only one character is allowed per one input.
- If the sender fails to get the answer in 7 times, he loses. 


## Files 

### sender.c

- The sender sends one specific word to the receiver.   
- Uses `msgget` and `msgsnd` system call and `gets` library.  

```C
#include <stdio.h> 
#include <sys/types.h> 
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <string.h>
#define DEFINED_KEY 0x10101010

int main(int argc, char **argv)
{
    int msg_qid;
    struct {
        long mtype;
        char content[256];
    } msg;

    fprintf(stdout, "=========SENDER==========\n");
    if((msg_qid = msgget(DEFINED_KEY, IPC_CREAT | 0666)) < 0) {
        perror("msgget: "); 
        exit(0); 
    }
    msg.mtype = 1;
        
    while(1) {
        memset(msg.content, 0x0, 256);
        gets(msg.content);
        if(msgsnd(msg_qid, &msg, sizeof(msg.content), 0) < 0) {
            perror("msgsnd: "); 
            exit(0); 
        }
    }
}

```



### receiver.c

- Once the receiver receives the word from the sender, the game starts.   
- Uses `msgget` and `msgrcv` system call. 

```C
#include <stdio.h> 
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <string.h>
#define DEFINED_KEY 0x10101010

int main(int argc, char **argv)
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
    char guess[100], word[100]; 
    
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
        scanf(" %s", guess); 
        while (strlen(guess) != 1) {
            printf("오직 하나의 영어 소문자만 입력하세요.\n"); 
            scanf(" %s", guess); 
        }

        int correct = 0; 
        for (int i = 0; i < size; i++) {
            if (word[i] == guess[0]) {
                answerSheet[i] = guess[0];
                correct = 1;
            }
        }

        if (correct == 0) {
            chance--;
            printf("***** 틀렸습니다! *****기회가 %d 로 줄어듭니다. \n", chance);
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
            printf("------> ***** %d번 만에 정답을 맞추었습니다! ***** <------\n", number); 

            exit(0); 
        }
    }
    
    printf("정답을 맞추지 못했습니다!"); 
    
}

```



### hangman.c and answer.txt

- You can also use hangman.c to play the game, reading the answers from the text file.   
- Uses `fopen` library function.   


#### hangman.c

```C
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
```

#### answer.txt

beautiful  
whale  
dynamic  
computer  
diving  
explore  
confident  
awesome  
urban  
chaos  


