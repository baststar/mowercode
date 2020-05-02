#pragma once

extern int currentFSMEvent;
extern int lastFSMEvent;
extern int beforeMenuFSMEvent;
extern int currentFSMSequence;

void Setup_FSM();
void Loop_FSM();
void Trigger_FSM(int event, int sequence);
