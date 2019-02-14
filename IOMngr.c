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

void
FreeHeadMessage() {
  // does this free all messages?
  printf("free");
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
  printf("output messages before\n");
}

void
OutputSource() {
  printf("OutputSource\n");
  // gets things started?
  // march through buffer when it is time to turn color on/off
  //Uses spans being in order
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

  // figure out what to add
  //
  //
}

void
CloseSource() {
  // can't display until here
  printf("close source\n" );
}

int
GetSourceChar() {
  return source[sourceIndex++];
  //what does this do?
}

// ASK ABOUT MESSAGE NUMBERS!!!!!!
bool
PostMessage(struct Span span, const char * aMessage) {
  // look for if you should add msg
  if(Messages) {
    struct Message * temp = Messages;
    if(span.last < temp->span.first) {
      struct Message * new = MakeMessage(span, 0, aMessage);
      new->next = temp;
      // update temps
    }else if((span.first >= temp->span.first && span.first <= temp->span.last)
          || (span.last >= temp->span.first && span.last <= temp->span.last)) {
      return false; // drop overlapping span
    }else{
      struct Message * pre = temp;
      temp = temp->next;
      bool searching = 1;
      while(temp && searching){
        if (span.first > pre->span.last && span.last < temp->span.first) {
          //add here
          struct Message * new = MakeMessage(span, temp->seqNumber, aMessage);
        }else if((span.first >= temp->span.first && span.first <= temp->span.last)
              || (span.last >= temp->span.first && span.last <= temp->span.last)) {
          return false; // drop overlapping span
        }else{
          pre = temp;
          temp = temp->next;
        }
      }
      while(temp) {
        // increment seqNumber because added a span before
        temp->seqNumber++;
        temp = temp->next;
      }
    }
  }else{
    Messages = MakeMessage(span, 0, aMessage);
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
