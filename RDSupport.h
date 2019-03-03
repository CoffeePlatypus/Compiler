/* Recursive Descent Support Functions */
#ifndef RDSUPPORT_H
#define RDSUPPORT_H

#include <stdbool.h>

// Initialize by obtaining first token
void RDInit();

// Return the current input token
int CurrentToken();

// test for token match
bool Match(int ExpectedToken);

// Calls on function entry and exit
// that maintain breadcrumb trail
#define ENTER       Enter(__func__)
#define LEAVE_FAIL  LeaveFail()
#define LEAVE_SUCC  LeaveSucc()
void Enter(const char *Name);
bool LeaveFail();
bool LeaveSucc();

// report parse error
void ParseError(char *Message);

#endif
