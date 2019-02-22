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
int nextEOFSpan = -1;
int sourceIndex;
bool debug = 0;
bool containsNewLine = false;
bool marked = false;

struct Message {
  struct Span span;
  int seqNumber;
  char * message;
  struct Message * next;
};

struct Message * Messages;
struct Message * Removed;
struct Message * EOFMessages;

struct Message *
MakeMessage(struct Span span, int seq, const char * message) {
  struct Message * msg = malloc(sizeof(struct Message));
  msg->span = span;
  msg->seqNumber = seq;
  msg->message = strdup(message);
  msg->next = NULL;
  // printf("New message: %s", message);
  return msg;
}

void printMesages() {
  struct Message * temp = Messages;
  if(!temp) {
    printf("No Messages\n");
  }
  while(temp) {
    printf("Message : %s\n", temp->message);
    printf("\t\tSpan : %d - %d\n",temp->span.first, temp->span.last);
    printf("\t\tseq num : %d\n", temp->seqNumber);
    temp = temp->next;
  }
}

void printEOFs() {
  struct Message * temp = EOFMessages;
  if(!temp) {
    printf("no eofs\n");
  }
  while(temp) {
    printf("Message : %s\n", temp->message);
    temp = temp->next;
  }
}

void addRemoved(struct Message * temp) {
  if(Removed) {
    struct Message * pre = Removed;
    while(pre->next) {
      pre = pre->next;
    }
    pre->next = temp;
    temp->next = NULL;
  }else{
    Removed = temp;
    temp->next = NULL;
  }
}

void clearRemoved() {
  while(Removed) {
    struct Message * temp = Removed;
    Removed = temp->next;
    free(temp->message);
    free(temp);
  }
  Removed = NULL;
}

void
FreeHeadMessage() {
  //printf("free");
  if(Messages) {
      struct Message * temp = Messages;
      Messages = temp->next;
      addRemoved(temp);
      // free(temp->message);
      // free(Messages);
  }
}

void FreeHeadEOF() {
  if(EOFMessages) {
    struct Message * temp = EOFMessages;
    EOFMessages = temp->next;
    free(temp->message);
    free(EOFMessages);
  }
}

bool
isLineBreak(char * c) {
  return *c == '\n' || *c == '\r' || c == sourceLastChar;
}

bool isEOFSpan(struct Span span) {
  return span.first == span.last && span.first < 0;
}

// ansi escape sequences for colors
char * colorSeqs[] = {"\033[91m","\033[92m","\033[93m","\033[94m","\033[95m","\033[96m"};

void
OutputMarkStart(struct Message * msg) {
  fprintf(stdout,"%s",colorSeqs[(msg->seqNumber % 6)]);
  marked = true;
}

void
OutputMarkStop() {
  fprintf(stdout,"\033[0m");
  marked = false;
}

void printRemoved() {
  struct Message * temp = Removed;
  while(temp) {
    printf("        ");
    OutputMarkStart(temp);
    printf("**");
    OutputMarkStop();
    printf(" %s\n",temp->message);
    temp = temp->next;
  }
}

int endColorFlag = false;

void
OutputInterval(char * start, char * stop) {  // give this span after starting or stopping color
  if (stop < start) return;
  char * temp = start;
  if (containsNewLine) {
    bool mark = marked;
    if(marked) {
      OutputMarkStop();
    }
    printRemoved();
    clearRemoved();
    printf("    %2d: ", curLine);
    containsNewLine = false;
    if(mark || endColorFlag){
      endColorFlag = false;
      OutputMarkStart(Messages);
    }
  }
  while (temp < stop && !isLineBreak(temp)) {
    temp++;
  }
  // if(temp == sourceLastChar) {
  //   printf("%c",temp);
  //   while(Messages){
  //     FreeHeadMessage();
  //   }
  //   printRemoved();
  // }else
  if (isLineBreak(temp)) {
    if (marked) {
      fwrite(start,temp - 1 - start + 1,1,stdout);
      OutputMarkStop();
      printf("%c", *temp);
      // if(!Removed) {
      //   OutputMarkStart(Messages);
      // }
      endColorFlag = true;
    }else{
      fwrite(start,temp - start + 1,1,stdout);
    }
    containsNewLine = true;
    curLine++;
    OutputInterval(temp+1, stop);
  }else{
      fwrite(start,stop - start + 1,1,stdout);
  }
}

void
OutputMessagesBefore(struct Message * curMsg) {
  OutputInterval(nextChar, source+curMsg->span.first-1);
  nextChar = source + curMsg->span.first;
}

void
OutputSource() {
  // printf("OutputSource\n");
  // gets things started?
  // march through buffer when it is time to turn color on/off
  //Uses spans being in order
    curLine = 1;
    nextChar = source;
    int index = 0;
    containsNewLine = false;
    printf("    %2d: ", curLine);
    while(Messages) {
      OutputMessagesBefore(Messages);
      OutputMarkStart(Messages);
      OutputInterval(nextChar, source+Messages->span.last);
      OutputMarkStop();
      nextChar = source + Messages->span.last +1;
      FreeHeadMessage();
    }
    OutputInterval(nextChar, sourceLastChar);
    printRemoved();
    while(EOFMessages) {
      printf("        ");
      OutputMarkStart(EOFMessages);
      printf("**");
      OutputMarkStop();
      printf(" %s\n", EOFMessages->message);
      FreeHeadEOF();
    }
}

bool
OpenSource(const char * aFilename) {
  if(debug) printf("open source\n");
  sourceFD = open(aFilename,O_RDONLY);
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
  return true;
}

void
CloseSource() {
  // can't display until here
  if(debug) printMesages();
  if(debug) printEOFs();
  OutputSource();
  // printf("close source\n" );
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
  if (debug) printf("\nPost Message with span: %d - %d\n",span.first, span.last);

  if(isEOFSpan(span)) {
    struct Message * temp = EOFMessages;
    if(temp) {
      while(temp->next) {
        temp = temp->next;
      }
      temp->next = MakeMessage(span, nextMessageNumber++, aMessage);
    }else{
        EOFMessages = MakeMessage(span, nextMessageNumber++, aMessage);
    }
  }else if(Messages) {
    struct Message * temp = Messages;
    if(span.last < temp->span.first) {
      if (debug) printf("\tnew message head\n");
      struct Message * new = MakeMessage(span, nextMessageNumber++, aMessage);
      new->next = temp;
      // update temps
    }else if((span.first >= temp->span.first && span.first <= temp->span.last)
          || (span.last >= temp->span.first && span.last <= temp->span.last)) {
      if (debug) printf("\t dup\n");
      nextMessageNumber++;
      return false; // drop overlapping span
    }else{
      struct Message * pre = temp;
      temp = temp->next;
      bool searching = 1;
      while(temp && searching){
        if (debug) printf("temp span: %d %d", temp->span.first, temp->span.last);
        if (span.first > pre->span.last && span.last < temp->span.first) {
          if(debug) printf("\tfound span spot\n");
          struct Message * new = MakeMessage(span, nextMessageNumber++, aMessage);
          new->next = pre->next;
          pre->next = new;
          searching = 0;
        }else if((span.first >= temp->span.first && span.first <= temp->span.last)
              || (span.last >= temp->span.first && span.last <= temp->span.last)
              || span.first < temp->span.first) {
          if(debug) printf("\t dup\n");
          nextMessageNumber++;
          return false; // drop overlapping span
        }else{
          pre = temp;
          temp = temp->next;
        }
      }
      if(searching) {
        if (debug) printf("add end\n");
        struct Message * new = MakeMessage(span, nextMessageNumber++, aMessage);
        new->next = pre->next;
        pre->next = new;
      }
    }
  }else{
    if (debug) printf("\tno messages\n");
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
  nextEOFSpan--;
  return span;
}
