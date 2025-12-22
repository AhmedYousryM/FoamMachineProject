/*
################## GRBL Control ########################
in GRBL_Control.Cpp ->> in constructor _jogDir isn't initialized
** In processResopnse >> Error Response
** Is Ok Recieved??
**  In getZaxisMachinePos>> String response = readGrblStatus(); () was added
    also status.length replaced by response.length
** In line 210 GRBL replaced with _grblSerial-> also line 244
also line 245 and readGRBLstatus added to class
################### CNC sensors ###############
is it all digital??
################### CnCMotors ###############
add semicolon to line 2

################### Fixation Main ###############
1.  add puplic
2.  change name of Homing and Fixating bool function to avoid conflict.
3.  add GoingIn sequance
4.  add move2mm & fixationEnd bool functions to .h
5.  add array size to sideplatesensor
6.  there's two functions ResetFixating & FixatingReset what the difference!!
7.  there's no function called Reset in noBlockingSequence so being replaced by Restart
8.  add Goingout to sequence objects
9.  add goingIn bool function and change GoingOut bool function to goingOut
10. undefined CNC_MAX_Y variable - solved by include CNCconstantmacros
11. note that file name ConstantMacros changed to CNCConstantMacros
12. Send Error added to Homing_MoveLongRun & Homing_MoveShortRun also
    Error Excel and macros Excel updated but ErrorVariable need to be updated

13. In GoingInSafe ->> - FOAMYWIDTH added to macros and to sharedvariable excel 
                       - Askvar Added to code
                       - CRVariable updated
                       - timing ??????
14. FIRST_SAFE_POS name changed
15. before_Fixation fcn modified
16. in beforFix->> -FIXATION_POINT_2_X added to macros and sharedvariables excel and in Macro code
                   - AskVar added
                   - timing???
                   - CRVariable updated
                   
                   - FORK_FIXATION_GAB added to macros but with undefined value!!!
                   - G1_FOAM_WIDTH added to macros and sharedvariables excel and to code
                   - CFVariables Updated
                   
                   - CNC_GATE_X_POS added to macros excel and code but with uundefined value
                   - G2_FOAM_WIDTH added to macros and sharedvariables excel and to code
                   - CFVariables Updated  
                   - Should i ask for variable or use it !!!!!!
17. in approachNearFoam - >> - fcn modified and WrongFcnCall error added
                             - what to do in warning
18. Acceptable_FoamYWidth modified                              
19. add error to FixationLimitSensorRead
20. change First_Time_Exc... to first_time_exexuting_step
21. DoingLubrication ->> modify Lubricating.Finish();
22. comment first goingOut for conflict
23. add safe_pos_before(float pos) to class members

################ to do #############
1. revise and make any change for hardware pins excel
2. add sections to Macros --- Done
3. add Constants to excel

#################### 13/12/2025 ###############
************* CNCSensorTestMode ***********
1. in loop ->>  - CNC_SENSOR_PAGE added to macros and sharedvariable and to code
                - CRVariable Updated
2. Modify ReadyToSendStep according to task 1
3. In SPchange ->> - SIDE_Plate_sensors added to macros and sharedvariables
                   - CRVariable Updated
                   - Fcn Modified
4. In FoamFixChange ->> - Fixation_G1&G2_LM added added to macros and sharedvariables
                   - CRVariable Updated
                   - Fcn Modified
5. In MotionHead1Change ->>
     - HEAD_G1_Z_UP & HEAD_G1_Z_DOWN & FRAME_G1_X & G1_G2_X & FIX_HEAD_Y_G1 & HEAD_FRAME_Y_G1_1 & HEAD_FRAME_Y_G1_2 added to macros and sharedvariables and to code
     - CRVariable updated
     - Fcn Modified
     - note _Sensor removed
6. In MotionHead2Change ->>
     - HEAD_G2_Z_UP & HEAD_G2_Z_DOWN & G2_FRAME_X & G1_G2_X & FIX_HEAD_Y_G2 & HEAD_FRAME_Y_G2_1 & HEAD_FRAME_Y_G2_2 added to macros and sharedvariables and to code
     - CRVariable updated
     - Fcn Modified
     - note _Sensor removed
7. MotionFix1Change & MotionFix2Change modified like 6 & 5
8. move Declare Step Macro to header file
9. add all fcns decleration to header file 
10. remove delete constructor as it's used in CNCMoveMotorsMode
################### 14/12/2025 ##################
****************** CNCMoveMotorsMode **********
1. remove static from begin and void 
2. add SensorTest as an object of SensorTestMode
3. In loop fcn add SensorTestMode 
4. MoveHead 2 G1 variables were used !! so replaced by G2
5. MoveDir remove else if and put else
6. add fcns declerations
7. in loop what to do in case 0;
8. Move speed type changed from char to int
9. remove delete and add puplic
################# 15/12/2025 ####################
****************** GRBL_Sender ***************
1. in openFile() Warning and Error added
2. what is _gcodeFile.seek(_filePosition);
3. first part of start()  is redundunt the error part i mean
4. in Sender_run() ->> - the nested if for ending file with no pend is useless it always valid
5.  sendNextLine() could case infinte loop for recursion
6. processResponse has fewer case than that in GRBL Control
7. Modify fileIsOpen()
8. in SetSpindleSpeed(float s) add GRBL_Sender::
*************** GRBL_main ****************
1. add warning to Grbl_main()
2. change Grbl_main() to GRBL_Main()
3. in begin() why Sender_begin Commented!!
4. add HEAD_WIDTH_X macro but with uncertain value
5. remove Control.Ismoving() and put only Ismoving() in StartPos1_2
6. add CNC_MidPoint to macros
7. modify fcn of StartPos1_2
8. Lubrication1() modified
9. in Lubrication2() in case of num 2 why finalX??
10. Replace Reset with Restart 
11. in StartPos2_1() ->> - Add HEAD1_START_POSITION_X/HEAD1_START_POSITION_Y/HEAD2_START_POSITION_Y/HEAD2_START_POSITION_X/FOAM_HEIGHT to macros and shared variables
                         - CRvariable updated
                         - Fcn Modified
12. in StartPos2_2() ->> - Add Set_StartPos2 !!!!!
13. modify Set_StartPos2() in start_z 
14. in ZAxis_Adjustment_Loop() ->> - Add ZAXIS_SETUP_DONE to macros and sharedvariables
                                   - CRVariable Updated 
                                   - Fcn Modified
                                   - replace EndZAdjust with IsZAxisAdjustmentFinished() as no code modify EndZAdjust so fcn always return false
15. add inputs definition to ZAxis in .h
16. add #include "CNCsensor.h"
17. remove startPressTime & flag_char init from ZAxis to class puplic
18. Modify AdjustmentEnd
19. Modify IsZAxisAdjustmentFinished()
20. Add Puplic
21. comment void SetWorkpieceCoordinates();
22. in Start_Cutting() ->> WorkingSequence.isFinished(); instead of Working.isFinished();
23. in NewCuttingProgram() replace reset with restart
24. in moving1() ->> comment setmachinecoordinates()
25. beforeFixating() use Head_num
26. FORK_FIXATION_POINT_2_X added to sharedvariables in CF but it is in CM !! and added to macro and CFvariable updated
****************** CNC *********************
1. add MODE / NOMODE_SUBMODES /PREPRODUCTION_SUBMODES/PRODUCTION_SUBMODES/CNC_OPERATION_DOING/FORK_START_HOMING
2. add G1_FIXATION_DOING/G2_FIXATION_DOING to macros and sharedvar and update cfvariable
3. add G1_CUTTING_DOING/G2_CUTTING_DOING to macros and sharedvar and update cfvariable
4. remove SensorTestMode.begin()
5. add erropin to sensors but with uncertian value
6. use SensorTest and MoveMotors objetcs
************ to do *********
Search for static in Grbl_main Startpresstime
2. move FORK_FIXATION_POINT_2_X to CM variables it's in GRBL_main
3. Reorgnize Task1 - use classes
*/