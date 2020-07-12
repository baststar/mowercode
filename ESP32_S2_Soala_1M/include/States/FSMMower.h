#pragma once

extern unsigned long currentMillisGlobal;
extern int currentFSMTransition;
extern int lastFSMTransition;
extern int beforeMenuFSMTransition;
extern int currentFSMSequence;
extern int currentState;
extern int lastState;
extern unsigned long sequenceStartTime;

int BuildStateTransitionId(int state1, int state2);
void Loop_FSM();
void TriggerFSM(int fromState, int toState, int sequence);
void StartSequence(int sequence);
