// This is an implementation of a FSM based on code of Gary Sims.
// gcc -O3 -o fsm-lexer fsm-lexer.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
  START,

  //infix operator
  INFIX_RI1,
  INFIX_LIEQ1,

  internal_END_STATES,

  //identifier
  IDENTIFIER_START,
  IDENTIFIER_REST,
  
  //literals
  __LITERAL,

  //delimiter
  __DELIMITER,

  //prefix operator
  __PREFIX_OPERATOR,

  //infix operator 
  INFIX_SINGLE,
  INFIX_LIEQ2,
  INFIX_EQ,
  INFIX_RI2,

  internal_LAST_STATE,
  TRAP,
} FSM_STATES;

typedef struct {
  FSM_STATES current_state;
  char *tokens;
  FSM_STATES next_state;
} FSM_TRANSITION_LOGIC;



FSM_STATES fsmTransition(FSM_STATES current_state, char *token, FSM_TRANSITION_LOGIC *state_machine) {
  FSM_TRANSITION_LOGIC *transition = state_machine;

  while (transition->current_state != internal_LAST_STATE) {
    if (transition->current_state == current_state) {
      if (strchr(transition->tokens, *token) != NULL) {
        return transition->next_state;
      }
    }
    transition++;
  }

  return TRAP;
}

int isAcceptingState(FSM_STATES state) {
  return (state > internal_END_STATES && state != TRAP);
}

// alpha = "abcdefghijklmnopqrstuvwxyz";
// number = "0123456789";
// alphanum = "abcdefghijklmnopqrstuvwxyz0123456789";

static FSM_TRANSITION_LOGIC fsm_lexer[] = {
  //Identifier
  {START, "abcdefghijklmnopqrstuvwxyz", IDENTIFIER_START},
  {IDENTIFIER_START, "abcdefghijklmnopqrstuvwxyz0123456789", IDENTIFIER_REST},
  {IDENTIFIER_REST, "abcdefghijklmnopqrstuvwxyz0123456789", IDENTIFIER_REST},
  
  //literals 
  {START, "01", __LITERAL},

  //dilimiter
  {START, "()", __DELIMITER},

  //prefix operator
  {START, "!", __PREFIX_OPERATOR},

  //infix operator
  {START, "&|", INFIX_SINGLE},
  {START, "<", INFIX_LIEQ1},
  {INFIX_LIEQ1, "=", INFIX_LIEQ2},
  {INFIX_LIEQ2, ">", INFIX_EQ},
  {START, "=", INFIX_RI1},
  {INFIX_RI1, ">", INFIX_RI2},

  {internal_LAST_STATE, "", internal_LAST_STATE}
};

typedef enum {
  IDENTIFIER,
  LITERAL,
  DELIMITER,
  PREFIX_OPERATOR,
  INFIX_OPRATOR,
  ERROR,
} TOKEN_TYPE;

typedef struct {
  TOKEN_TYPE tokenType;
  char* token;
} TOKEN;

static TOKEN errorToken = {ERROR, ""};

TOKEN_TYPE getTokenTypeOf(FSM_STATES state) {
  switch (state) {
    case(IDENTIFIER_START):
    case(IDENTIFIER_REST):
      return IDENTIFIER;

    case(__LITERAL):
      return LITERAL;

    case(__DELIMITER):
      return DELIMITER;

    case(__PREFIX_OPERATOR):
      return PREFIX_OPERATOR;

    case(INFIX_LIEQ2):
    case(INFIX_RI2):
    case(INFIX_EQ):
      return INFIX_OPRATOR;

    default: return ERROR;
  }
}

struct Tuple {
  FSM_STATES state;
  int depth;
};

struct Tuple findLongestToken(FSM_STATES current_state, char* string) {
  char* c = string;
  char* nextc = c + 1;

  struct Tuple trap = {TRAP, 0};
  if(strlen(c) == 0) return trap;

  FSM_STATES next_state = fsmTransition(current_state, c, fsm_lexer);

  if(next_state == TRAP) return trap;

  struct Tuple result = findLongestToken(next_state, c + 1);
  //when an accepting state came before came current state then the depth is 
  //incremented by one and the result gets returned
  if(isAcceptingState(result.state)) {
    result.depth++;
    return result;
  } 

  //when the current state is the last accepting state for the given string, 
  //then the state is returned and the depth is set to 0
  if(isAcceptingState(current_state)) {
    result.state = current_state;
    result.depth = 1;
    return result;
  }

  //this returns, if neighther this state is an accepting state nor the once 
  //coming after
  return result;
}

char* tokenTypeToString(TOKEN_TYPE tokenType) {
  switch (tokenType) {
    case(IDENTIFIER): return "Identifier";
    case(LITERAL): return "Literal";
    case(DELIMITER):  return "Delimiter";
    case(PREFIX_OPERATOR): return "Prefix Operator";
    case(INFIX_OPRATOR): return "Infix Operator";
    default: return "Error";
  }
}

char* splitSringAfter(char* string, int length) {
  int i = 0;
  char* result = (char *)malloc(length + 1);
  while (i < length && string + i) {
    *(result + i) = *(string + i);
    i++;
  }

  result += '\0';
  return result;
}

TOKEN nextToken(char* string) {
  struct Tuple token_delim = findLongestToken(START, string);
  TOKEN_TYPE type = getTokenTypeOf(token_delim.state);
  char* token_string = splitSringAfter(string, token_delim.depth);

  TOKEN token = {type, token_string};
  return token; 
  // return errorToken;
}

char* tokenToStr(TOKEN token) {
  return strcat(tokenTypeToString(token.tokenType), 
      strcat(": ", token.token));
}

int main(void) {
  struct Tuple r = findLongestToken(START, "!)rsth()ttst");
  printf("%d\n", r.depth);
  printf("%s\n", tokenTypeToString(getTokenTypeOf(r.state)));
  // printf("%s", (char)r.state);
  TOKEN nt = nextToken("abc");
  //printf("%s", tokenToStr(nt));

}