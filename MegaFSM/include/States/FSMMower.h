#pragma once

extern int currentFSMEvent;
extern int lastFSMEvent;
extern int beforeMenuFSMEvent;
extern int currentFSMSequence;

int BuildStateTransitionId(int state1, int state2);
void Setup_FSM();
void Loop_FSM();
void Trigger_FSM(int event, int sequence);
