/* IOMngr.h

   Implements a simple interface for obtaining source file characters one at
   a time. It also produces a listing of the source file, together
   with any messages, to stdout. Messages are associated with non-overlapping
   spans of source text. To allow messages to span multiple lines output
   is not produced until the source file is closed.

*/
#ifndef IOMNGR_H
#define IOMNGR_H

#include <stdbool.h>

// A sequence of char in the source
struct Span {
  int first;
  int last;
};

/* OpenSource       Opens aFileName as the source.

   CloseSource      Writes the source file to stdout along with associated
                    messages and closes the source file. Color is used to
                    highlight the span of source to which the message applies.
*/
bool OpenSource(const char * aFileName);
void CloseSource();

/* GetSourceChar    Returns the next character in the source file. The EOF
                    character is returned if the source file is at the end
                    of file when this function is called.
*/
int GetSourceChar();

/* PostMessage      Registers a message to be associated with a span of source
                    text. The message is added to a singly linked list order by
                    position. If the span would overlap an existing message span
                    the new message is dropped. The function returns a bool
                    indicating whether the message was accepted.
*/
bool PostMessage(struct Span span, const char * aMessage);

/* GetLastPosition  Returns the index position of the last returned character.
*/
int GetLastPosition();

/* convenience functions for making span structs
*/
struct Span MakeSpan(int first, int last);
struct Span MakeSpanFromLength(int first, int length);
struct Span MakeEOFSpan();

#endif
