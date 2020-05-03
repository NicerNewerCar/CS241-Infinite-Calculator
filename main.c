#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>

#include "list.h"

// constants
#define EOS	257
#define NUM	258

// prototypes
list* expr();
list* term();
list* factor();
list* diff();
void error( char * );
int get_token();
void match( int );
list *add(list *, list *);
list *sub(list *, list *);
int sumDigits(list *);
list *mtpy(list *, list *);
void printList(list *);
void freeList(list *);

// global communication variables
int current_token;
list * current_attribute;
FILE *ifp, *ofp;

/* bounded memory calculator */
int main(int argc, char const *argv[]) {

	if (argc < 3) {
		fprintf(stderr,"Not enough arguments\n");
		exit(1);
	}
	if (!(ifp = fopen(argv[1],"r"))) {
		fprintf(stderr,"Cannot open file %s\n",argv[1]);
		exit(1);
	}
	if (!(ofp = fopen(argv[2],"w"))) {
		fprintf(stderr,"Cannot open file %s\n",argv[2]);
		exit(1);
	}

	while(!feof(ifp)) {
		list *value;
		current_token = get_token();
		while ( current_token != EOS ) {
			value = expr();
			printList(value);
			freeList(value);
		}
	}
}

/* calculator */

// handles addition
list *expr() {
	list *value = diff(), *free;
	while (1) {
		if ( current_token == '+' ) {
			match( '+' );
			//value += term();
			free = add(value, diff());
			freeList(value);
			value = free;
		}
		else break;
	}
	return value;
}

list *diff() {
	list *value = term(), *free;
	while (1) {
		if ( current_token == '-' ) {
			match( '-' );
			free = sub(value, term());
			freeList(value);
			value = free;
		}
		else break;
	}
	return value;
}

// handles multiplication
list *term() {
	list *value = factor(), *free;
	while (1) {
		if ( current_token == '*' ) {
			match( '*' );
			//value *= factor();
			value = mtpy(value, factor());
		}
		else break;
	}
	return value;
}

// handles brackets and numbers
list *factor() {
	list *value;

	if ( current_token == '(' ) {
		match( '(' );
		value = expr();
		match( ')' );
		return value;
	}
	else if ( current_token == NUM ) {
		value = current_attribute;
		match( NUM );
		return value;
	}
	else error( "Unexpected token in factor()" );
}

/* match expected token */
void match( int expected_token ) {
	if ( current_token == expected_token ) {
		current_token = get_token();
	}
	else {
		error("Unexpected token in match" );
	}
}


/* get next token */
int get_token() {
	int c;		// current character from the stream
	list *value=NULL, *tmp;	//linked list for reading in nums

	while (1) {
		switch ( c = fgetc(ifp) ) {
		case '+': case '*': case '(': case ')': case '-':
			return c;	// return operators and brackets as is
		case ' ': case '\t':
			continue;	// ignore spaces and tabs
		default:
			if ( isdigit(c) ) {
				value = make_node(c-48);
				tmp = value;
				while ( isdigit( c = fgetc(ifp) )) {
					tmp->next = make_node(c-48);
		      tmp->next->prev = tmp;
		      tmp = tmp->next;
				}
				ungetc( c, ifp );
				current_attribute = value;
				return NUM;
			}
			else if ( c == '\n' ||  c==-1) {
				return EOS;
			}
			else {
				fprintf( stderr, "{%c}", c );
				error( "Unknown token" );
			}
		}
	}
}

// error reporting function
void error( char *message ){
	fprintf( stderr, "Error: %s\n", message );
	exit(1);
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
    return mtpy(numTwo,numOne);
  } else {
    list *value = make_node(0), *tmp = make_node(1), *free;
    while(sumDigits(numTwo) != 0){
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
    fprintf(ofp, "%d", number->num);
    number = number->next;
  }
  fprintf(ofp, "\n" );
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
