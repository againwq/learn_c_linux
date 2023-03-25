#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define NAME_SIZE 10
#define NUMBER_SIZE 11
struct Person;
typedef struct Person * Position;
typedef struct Person * List;

struct Person{
    char name[NAME_SIZE];
    char number[NUMBER_SIZE];
    Position next;
};

void addPerson(List, char *name, char *number);
void delPerson(List, char *name);
Position findPerson(List contacts, char *name);
void changePerson(List, char *name, char *number);
void displayPerson(List);
void exit_contacts(List);
void clear_tmp_char();
Position find_pervious(List, Position);


int main(int argc, char const *argv[])
{
    List contacts = malloc(sizeof(struct Person));
    int control;
    char name[NAME_SIZE];
    char number[NUMBER_SIZE];
    while(1){
        printf("添加联系人: 1\n");
        printf("删除联系人: 2\n");
        printf("查找联系人: 3\n");
        printf("修改联系人: 4\n");
        printf("显示所有联系人: 5\n");
        printf("退出通讯录: 6\n");
        printf("请输入命令:");
        scanf("%d", &control);
        switch (control)
        {
            case 1: printf("请输入联系人姓名(回车结束)"); scanf("%s", name);
                    printf("请输入联系人号码(回车结束)"); scanf("%s", number);
                    addPerson(contacts, name, number); break;
            case 2: printf("请输入要删除的联系人姓名(回车结束)"); scanf("%s", name);
                    delPerson(contacts, name); break;
            case 3: printf("请输入要查找的联系人姓名(回车结束)"); scanf("%s", name);
                    findPerson(contacts, name); break;
            case 4: printf("请输入要修改的联系人姓名(回车结束)"); scanf("%s", name);
                    printf("请输入要修改的联系人号码(回车结束)"); scanf("%s", number);
                    changePerson(contacts, name, number); break;
            case 5: displayPerson(contacts); break;
            case 6: exit_contacts(contacts); break;
        }
    } 
    return 0;
}

void clear_tmp_char(){
    char tmp_char;
    while ((tmp_char = getchar() != '\n') && tmp_char != EOF);
}
void addPerson(List contacts, char *name, char *number){
    Position Tmp, p;
    Tmp = findPerson(contacts, name);
    if(Tmp != NULL){
        printf("该用户已经存在");
    }else{
        p = contacts;
        while(p -> next != NULL){
            p = p -> next;
        }
        Position person = malloc(sizeof(struct Person));
        strcpy(person -> name, name);
        strcpy(person -> number, number);
        person -> next = NULL;
        p -> next = person;
    }
}

Position findPerson(List contacts, char *name){
    Position p = contacts;
    if(strcmp(name, p -> name) == 0){
        return p;
    }else if(p -> next == NULL){
        return NULL;
    }else{
        return findPerson(p -> next, name);
    }
}

Position find_pervious(List contacts, Position p){
    Position L = contacts;
    while(L -> next != p){
        L = L -> next;
    }
    return L;
}

void delPerson(List contacts, char *name){
    Position aimP = findPerson(contacts, name);
    Position preP = find_pervious(contacts, aimP);
    preP -> next = aimP -> next;
    free(aimP);
}

void changePerson(List contacts, char *name, char *number){
    Position aimP = findPerson(contacts, name);
    if(aimP == NULL){
        printf("该用户不存在");
    }else{
        strcpy(aimP -> name, name);
        strcpy(aimP -> number, number);
    }
}

void displayPerson(List contacts){
    Position TmpL = contacts;
    while(TmpL != NULL){
        printf("姓名：%s\n", TmpL -> name);
        printf("号码：%s\n", TmpL -> number);
        TmpL = TmpL -> next;
    }
}

void exit_contacts(List contacts){
    while (contacts != NULL){
        Position Tmp = contacts;
        free(contacts);
        contacts = Tmp -> next;
    }
    exit(1);
}