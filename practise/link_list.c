#include <link_list.h>
#include <stdio.h>
#include<stdlib.h>

int IsEmpty( List L ){
    return L -> Next == NULL;
}

int IsLast(Position P, List L){
    return P -> Next == NULL;
}

Position Find(ElementType X, List L){
    Position P;
    P = L -> Next;
    while(P != NULL && P -> Element != X){
        P = P -> Next;
    }
    return P;
}
void Delete(ElementType X, List L){
    Position P, TmpCell;
    P = FindPrevious(X, L);
    if(!IsLast(P, L)){
        TmpCell = P -> Next;
        P -> Next = TmpCell -> Next;
        free(TmpCell);
    }
}
Position FindPrevious( ElementType X, List L){
    Position P = L;
    while( P -> Next != NULL && P -> Next -> Element != X){
        P = P -> Next;
    }
    return P;
}
void Insert(ElementType X, List L, Position P){
    Position TmpCell;
    TmpCell = (Position)malloc(sizeof(struct Node));

    TmpCell -> Element = X;
    TmpCell -> Next = P -> Next;
    P -> Next = TmpCell;
};

void DeleteList( List L ){
    Position P = L -> Next;
    L -> Next = NULL;

    while (P != NULL)
    {
        Position TmpCell = P;
        free(P);
        P = TmpCell -> Next;
    }
}


