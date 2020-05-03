#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>

#include "list.h"


list *add(list *, list *);
list *sub(list *, list *);
list *mtpy(list *, list *);
list *readNum();
int sumDigits(list *);
void printList(list *);
void freeList(list *);


int main() {
  list *numOne = readNum();
  list *numTwo = readNum();
  printList(mtpy(numOne,numTwo));
}

list *readNum() {
  int c;
  list *num=NULL, *tmp=NULL;
  while(isdigit(c = getchar())) {
    if(num == NULL) {
      num = make_node(c-48);
      tmp = num;
    } else {
      tmp->next = make_node(c-48);
      tmp->next->prev = tmp;
      tmp = tmp->next;
    }
  }
  //ungetc(c,stdin);
  return num;
}
list *add(list *numOne, list *numTwo) {
  //finds and sets the tails of each number:
  list *tailOne, *tailTwo, *tmp;
  tmp = numOne;
  while(tmp->next != NULL) {
    tmp = tmp->next;
  }
  tailOne = tmp;

  tmp = numTwo;
  while(tmp->next != NULL) {
    tmp = tmp->next;
  }
  tailTwo = tmp;
  if(list_length(numTwo)>list_length(numOne)) {
    //makes sure the larger number is numOne
    return add(numTwo,numOne);
  } else {
    //adds the two numbers together
    int carry=0, digit=0;
    list *valueTail=NULL, *value;
    while(tailOne != NULL) {
      if(tailTwo != NULL) {
        digit = tailOne->num + tailTwo->num + carry;
        carry = 0;
        if(digit>=10) {
          carry = digit/10;
          digit %=10;
        }
        tailTwo = tailTwo->prev;
      } else {
        digit = tailOne->num + carry;
        carry = 0;
      }
      if(valueTail == NULL) {
        valueTail = make_node(digit);
        value = valueTail;
      } else {
        value->prev = make_node(digit);
        value->prev->next = value;
        value = value->prev;
      }
      tailOne = tailOne->prev;
    }
    if(carry > 0) {
      value->prev = make_node(carry);
      value->prev->next = value;
      value = value->prev;
    }
    return value;
  }
}
list *sub(list *numOne, list *numTwo) {
  //finds and sets the tails of each number:
  list *tailOne, *tailTwo, *tmp;
  tmp = numOne;
  while(tmp->next != NULL) {
    tmp = tmp->next;
  }
  tailOne = tmp;

  tmp = numTwo;
  while(tmp->next != NULL) {
    tmp = tmp->next;
  }
  tailTwo = tmp;
  if(list_length(numTwo)>list_length(numOne)) {
    //makes sure the larger number is numOne
    return sub(numTwo,numOne);
  } else {
    //subs the two numbers together
    int digit=0;
    list *valueTail=NULL, *value, *borrow;
    while(tailOne != NULL) {
      if(tailTwo != NULL) {
        if(tailOne->num < tailTwo->num) {
          borrow = tailOne->prev;
          while(borrow != NULL && borrow->num == 0) {
            borrow->num = 9;
            borrow = borrow->prev;
          }
          if(borrow != NULL && borrow->num != 0) {
            borrow->num -= 1;
            tailOne->num += 10;
          }
        }
        digit = tailOne->num - tailTwo->num;
        tailTwo = tailTwo->prev;
      } else {
        digit = tailOne->num;
      }
      if(valueTail == NULL) {
        valueTail = make_node(digit);
        value = valueTail;
      } else {
        value->prev = make_node(digit);
        value->prev->next = value;
        value = value->prev;
      }
      tailOne = tailOne->prev;
    }
    return value;
  }
}
int sumDigits(list *number) {
  int sum=0;
  while(number != NULL) {
    sum += number->num;
    number = number->next;
  }
  return sum;
}
list *mtpy(list *numOne,list *numTwo) {
  if(list_length(numTwo)>list_length(numOne)) {
    //makes sure the larger number is numOne
    fprintf(stderr, "YERT\n" );
    return mtpy(numTwo,numOne);
  } else {
    list *value = make_node(0), *tmp = make_node(1), *free;
    while(sumDigits(numTwo) != 0){
      printList(numTwo);
      free = add(value, numOne);
      freeList(value);
      value = free;
      free = sub(numTwo, tmp);
      freeList(numTwo);
      numTwo = free;
    }
    return value;
  }
}
void printList(list *number) {
  while(number != NULL) {
    fprintf(stderr, "%d", number->num);
    number = number->next;
  }
  fprintf(stderr, "\n" );
}
void freeList(list *start) {
  list* tmp=NULL;
  while(start != NULL) {
    tmp = start;
    start = start->next;
    free_node(tmp);
  }
  start = NULL;
}
