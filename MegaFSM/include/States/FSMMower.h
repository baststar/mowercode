#pragma once

extern int currentFSMEvent;
extern int lastFSMEvent;
extern int beforeMenuFSMEvent;
extern int currentFSMSequence;

int BuildStateTransitionId(int state1, int state2);
void Loop_FSM();
void TriggerFSM(int fromState, int toState, int sequence);
