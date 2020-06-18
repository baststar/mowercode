#pragma once

enum NodeMCUMessageTopics { NewState = 1 };

void Setup_NodeMCU();
void SendData(NodeMCUMessageTopics topic, String value);
