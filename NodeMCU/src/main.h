#pragma once

enum NodeMCUMessageTopics { NewState = 1 };

void myTimerEvent();
void Blynk_Connect();
void Update_Blynk_App_With_Status();
void Clear_APP();
void Transmit_Blynk_Data_to_Mega();
void Receive_All_From_MEGA();
void Print_RX_Values();
void Calculate_Filtered_Mower_Status();
void Pause_Mower();
void Going_Home();
void StartMower();
void Exit_Dock();
void Set_To_Manuel_Mode();
void Set_To_Automatic_Mode_Spiral();
void Set_To_Automatic_Mode_Random();
void Set_To_Automatic_Mode_Parallel();
void Setup_Wifi();
void ConnectMQTT();
void Receive_Mega_Topic();
void ProcessData(int topic, int value);
void UpdateVoltAmpCharge();
void UpdateVoltAmpRain();
