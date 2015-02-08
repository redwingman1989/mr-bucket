#pragma once
enum RobotState{LoadRings,ScoreRings
};
class RunableModule
{
public:
    RunableModule(void);
    ~RunableModule(void);
    //the main entry point for work each cycle
    int RunTick(float time,RobotState state);
};

