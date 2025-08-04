#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

#define EPS 256
#define ALL 257
#define MATCH 258


#define MAX_LINE_LENGTH 1024


struct state {
   int c;
   struct state *out1;
   struct state *out2;
   int last_set;
};


typedef struct state state_t;


struct nfa {
   state_t *start;
   state_t *final;
   int n;
};


typedef struct nfa nfa_t;


struct stack {
   int length;
   int capacity;
   nfa_t *data;
};


typedef struct stack stack;


struct set {
   int length;
   int id;
   state_t **states;
};


typedef struct set set_t;


state_t *new_state(int c, state_t *out1, state_t *out2) {
   state_t* s = (state_t*)malloc(sizeof(struct state));
   s->c = c;
   s->out1 = out1;
   s->out2 = out2;
   s->last_set = -1;
   return s;
}


nfa_t character(int c) {
   nfa_t a;
   state_t* s;
   state_t* f;
   s = new_state(c,f,NULL);
   f= new_state(c,NULL,NULL);
   a.start = s;
   a.final = f;
   a.n = 2;
   return a;
}




nfa_t all(void) {
   nfa_t a;
   nfa_t a = character(ALL);
   return a;
}




nfa_t concat(nfa_t a, nfa_t b) {
   nfa_t c;
   c.start = a->start;
   a.final->out1 = b->start;
   a.final->c = EPS;
   c.final = b->final;
   c.n = a.n + b.n;
   return c;
}


nfa_t alternative(nfa_t a, nfa_t b) {
   nfa_t c;
   c.start = new_state(EPS,a.start,b.start);
   c.final = new_state(MATCH,NULL,NULL);
   b.final->out1 = c.final;
   a.final->out1 = c.final;
   c.n = a.n + b.n + 2;
   return c;
}


nfa_t star(nfa_t a) {
   nfa_t astar;
   astar.final = new_state(MATCH,NULL,NULL);
   a.final->out1 = astar.final;
   astar.start = new_state(EPS,a.start,astar.final);
   astar.final->out1 = astar.start;
   nfa_t astar = a.n + 2;
   return astar;
}


nfa_t maybe(nfa_t a) {
   nfa_t amaybe;
   amaybe.final = new_state(EPS,NULL,NULL);
   a.final->out1 = amaybe.final;
   amaybe.start = new_state(MATCH,a.start,amaybe.final);
   amaybe.n = 2 + a.n;
   return amaybe;
}


stack *stack_new(int capacity) {
   stack *s = malloc(sizeof(stack));
   s->data = malloc(capacity * sizeof(nfa_t));
   s->capacity = capacity;
   s->length = 0;
   return s;
}


void stack_free(stack *s) {
   free(s->data);
   free(s);
}


nfa_t pop(stack *s) {
   assert(s->length > 0);
   nfa_t result = s->data[s->length - 1];
   s->length--;
   return result;
}


void push(stack *s, nfa_t a) {
   assert(s->capacity > s->length);
   s->data[s->length] = a;
   s->length++;
}


nfa_t build(char *regex) {
   int n = strlen(regex);
   stack *s = stack_new(2 * n);
   for (int i = 0; i < n; i++) {
       char c = regex[i];
       if (c == '@') {
           nfa_t b = pop(s);
           nfa_t a = pop(s);
           push(s, concat(a, b));
       } else if (c == '*') {
           nfa_t a = pop(s);
           push(s, star(a));
       } else if (c == '|') {
           nfa_t b = pop(s);
           nfa_t a = pop(s);
           push(s, alternative(a, b));
       } else if (c == '?') {
           nfa_t a = pop(s);
           push(s, maybe(a));
       } else if (c == '.') {
           push(s, all());
       } else {
           push(s, character(c));
       }
   }
   assert(s->length == 1);
   nfa_t result = pop(s);
   stack_free(s);
   return result;
}




bool backtrack(state_t *state, char *s) {
   if (state == NULL) return false;
   if (state->c == EPS) {
       return backtrack(state->out1, s) || backtrack(state->out2, s);
   }
   if (s[0] == '\0' || s[0] == '\n') return  state->c == MATCH;
   if (s[0] == state->c || state->c == ALL) return backtrack(state->out1, &s[1]);
   return false;
}


bool accept_backtrack(nfa_t a, char *s) {
   return backtrack(a.start, s);
}


void match_stream_backtrack(nfa_t a, FILE *in) {
   char *line = malloc((MAX_LINE_LENGTH + 1) * sizeof(char));
   while (true) {
       if (fgets(line, MAX_LINE_LENGTH, in) == NULL) break;
       if (accept_backtrack(a, line)) {
           printf("%s", line);
       }
   }
   free(line);
}


int main(int argc, char* argv[]) {
   assert(argc >= 2);
   FILE *in = stdin;
   if (argc >= 3) in = fopen(argv[2], "r");
   nfa_t a = build(argv[1]);
   match_stream_backtrack(a, in);
   if (argc >= 3) fclose(in);
   return 0;
}





