#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>

#include "IOMngr.h"

int sourceFD;
char * source;
size_t sourceByteCnt;
char * sourceLastChar;
char * nextChar;        // address of next char to return
int curLine;
int nextMessageNumber;
int nextEOFSpan;
int sourceIndex;

struct Message {
  struct Span span;
  int seqNumber;
  char * message;
  struct Message * next;
};

struct Message * Messages;

struct Message *
MakeMessage(struct Span span, int seq, const char * message) {
  struct Message * msg = malloc(sizeof(struct Message));
  msg->span = span;
  msg->seqNumber = seq;
  msg->message = strdup(message);
  msg->next = NULL;
  printf("New message: %s", message);
  return msg;
}

void printMesages() {
  struct Message * temp = Messages;
  while(temp) {
    printf("Message : %s\n", temp->message);
    printf("\t\tSpan : %d - %d\n",temp->span.first, temp->span.last);
    printf("\t\tseq num : %d\n", temp->seqNumber);
    temp = temp->next;
  }
}

void
FreeHeadMessage() {
  //printf("free");
  if(Messages) {
      struct Message * temp = Messages;
      Messages = temp->next;
      free(temp->message);
      free(Messages);
  }
}

bool
isLineBreak(char * c) {
  return *c == '\n' || *c == '\r' || c == sourceLastChar;
}

// ansi escape sequences for colors
char * colorSeqs[] = {"\033[91m","\033[92m","\033[93m","\033[94m","\033[95m","\033[96m"};

void
OutputMarkStart(struct Message * msg) {
  fprintf(stdout,"%s",colorSeqs[(msg->seqNumber % 6)]);
}

void
OutputMarkStop() {
  fprintf(stdout,"\033[0m");
}

void
OutputInterval(char * start, char * stop) {  // give this span after starting or stopping color
  if (stop < start) return;
  fwrite(start,stop - start + 1,1,stdout);
}

void
OutputMessagesBefore(struct Message * curMsg) {
  //
  //printf("output messages before\n");
  OutputInterval(nextChar, source+curMsg->span.first-1);
  nextChar += curMsg->span.first;
}

void
OutputSource() {
  printf("OutputSource\n");
  // gets things started?
  // march through buffer when it is time to turn color on/off
  //Uses spans being in order
    curLine = 1;
    nextChar = source;
    int index = 0;
    while(Messages) {
      OutputMessagesBefore(Messages);
      OutputMarkStart(Messages);
      OutputInterval(nextChar, source+Messages->span.last);
      nextChar += Messages->span.last + 1;
      OutputMarkStop();
      FreeHeadMessage();
    }
}

bool
OpenSource(const char * aFilename) {
  printf("open source\n");
  sourceFD = open(aFilename,O_RDONLY);
  printf("sourceFD: %d\n",sourceFD); /// what is source FD
  if (sourceFD < 0) return false;
  struct stat buf;
  if (fstat(sourceFD,&buf)) return false;
  sourceByteCnt = buf.st_size;
  source = mmap(NULL,sourceByteCnt,PROT_READ,MAP_PRIVATE,sourceFD,0);
  sourceIndex = 0;
  curLine = 1;
  nextMessageNumber = 0;
  nextChar = source;
  sourceLastChar = source + sourceByteCnt - 1;
  // figure out what to add
  //
  //
  return true;
}

void
CloseSource() {
  // can't display until here
  printMesages();
  OutputSource();
  printf("close source\n" );
}

int
GetSourceChar() {
  // if(source[sourceIndex] == '\0' || source[sourceIndex] == EOF) {
  //   return EOF;
  // }else if(source[sourceIndex] == '\n'){
  //   curLine++;
  // }
  // return source[sourceIndex++];
  if(*nextChar == '\0' || *nextChar == EOF) {
    return EOF;
  }else if(*nextChar == '\n'){
    curLine++;
  }
  char temp = *nextChar;
  nextChar++;
  return temp;
}


bool
PostMessage(struct Span span, const char * aMessage) {
  printf("\nPost Message with span: %d - %d\n",span.first, span.last);
  // look for if you should add msg
  if(Messages) {
    struct Message * temp = Messages;
    if(span.last < temp->span.first) {
      printf("\tnew message head\n");
      struct Message * new = MakeMessage(span, nextMessageNumber++, aMessage);
      new->next = temp;
      // update temps
    }else if((span.first >= temp->span.first && span.first <= temp->span.last)
          || (span.last >= temp->span.first && span.last <= temp->span.last)) {
      printf("\t dup\n");
      return false; // drop overlapping span
    }else{
      struct Message * pre = temp;
      temp = temp->next;
      bool searching = 1;
      while(temp && searching){
        printf("temp span: %d %d", temp->span.first, temp->span.last);
        if (span.first > pre->span.last && span.last < temp->span.first) {
          printf("\tfound span spot\n");
          struct Message * new = MakeMessage(span, nextMessageNumber++, aMessage);
          new->next = pre->next;
          pre->next = new;
          searching = 0;
        }else if((span.first >= temp->span.first && span.first <= temp->span.last)
              || (span.last >= temp->span.first && span.last <= temp->span.last)
              || span.first < temp->span.first) {
          printf("\t dup\n");
          return false; // drop overlapping span
        }else{
          pre = temp;
          temp = temp->next;
        }
      }
      if(searching) {
        printf("add end\n");
        struct Message * new = MakeMessage(span, nextMessageNumber++, aMessage);
        new->next = pre->next;
        pre->next = new;
      }
    }
  }else{
    printf("\tno messages\n");
    Messages = MakeMessage(span, nextMessageNumber++, aMessage);
  }
  return true;
}

int
GetLastPosition() {
  return nextChar - 1 - source;
}

struct Span
MakeSpan(int first, int last) {
  struct Span span = {first, last};
  return span;
}

struct Span
MakeSpanFromLength(int start, int length) {
  struct Span span = {start,start + length - 1};
  return span;
}

struct Span
MakeEOFSpan() {
  struct Span span = {nextEOFSpan,nextEOFSpan};
  nextEOFSpan++;
  return span;
}
