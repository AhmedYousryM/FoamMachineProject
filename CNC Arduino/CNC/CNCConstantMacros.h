#ifndef CNCCONSTANTMACROS_H
#define CNCCONSTANTMACROS_H

// ########## Constants #############
#define CNC_MAX_X 2800
#define CNC_MIN_X 1600
#define CNC_MAX_Y 1400
#define GANTRY_WIDTH_X 250
#define HEAD_WIDTH_Y 300
#define HEAD_WIDTH_X 300
#define CNC_MidPoint (CNC_MIN_X+CNC_MAX_X)/2.0
#define FIXATION_WIDTH_Y 300
#define SAFE_GAB 20
#define FORK_FIXATION_GAB 20
#define CNC_GATE_X_POS 3100
// ######### Errors ###########
#define CNClimitCross 207
#define WrongFcnCall 208
#define CNCOverDistance 209
#define ERR210 210
#define ERR212 212
#define ERR214 214
#define ERR215 215
#define ERR217 217
#define ERR218 218
#define ERR219 219
#define ERR220 220
#define ERR221 221
#define ERR222 222
#define ERR223 223
#define ERR224 224
#define ERR225 225
#define ERR226 226
#define ERR227 227
#define ERR228 228
#define ERR229 229
#define ERR230 230
#define ERR231 231
// ######## Warning ###########
#define UnexpectedWoodtouch 200
#define WrWrongFcnCall 201
#define Wr202 202
#define Wr203 203
#define Wr204 204
#define Wr205 205
#define Wr206 206
#define Wr211 211
#define WR213 213
// ######### Index ##########
// ******** RC Variables *******
#define ZAXIS_SETUP_DONE 2
#define GCODE_FILE_PATH1 3
#define GCODE_FILE_PATH2 4
#define HEAD1_START_POSITION_X 5
#define HEAD1_START_POSITION_Y 6
#define HEAD2_START_POSITION_X 7
#define HEAD2_START_POSITION_Y 8
#define FOAMYWIDTH 10
#define FOAM_HEIGHT 11
#define FIXATION_POINT_2_X 12
#define CNC_SENSOR_PAGE  13
#define SIDE_PLATE_LM_1_SENSOR 14
#define SIDE_PLATE_LM_2_SENSOR 15
#define SIDE_PLATE_LM_3_SENSOR 16
#define SIDE_PLATE_LM_4_SENSOR 17
#define SIDE_PLATE_LM_5_SENSOR 18 
#define FIXATION_G1_LM 19
#define FIXATION_G2_LM 20
#define HEAD_G1_Z_UP 21
#define HEAD_G1_Z_DOWN 22
#define HEAD_G2_Z_UP 23
#define HEAD_G2_Z_DOWN 24
#define FRAME_G1_X 25
#define G1_G2_X 26
#define G2_FRAME_X 27
#define FRAME_FIX_Y_G1 28
#define FIX_HEAD_Y_G1 29
#define HEAD_FRAME_Y_G1_1 30
#define HEAD_FRAME_Y_G1_2 31
#define FRAME_FIX_Y_G2 32
#define FIX_HEAD_Y_G2 33
#define HEAD_FRAME_Y_G2_1 34
#define HEAD_FRAME_Y_G2_2 35
#define MOVE_DIRECTION 36
#define MOVE_SPEED 37
#define MOVE_G1_X 38
#define MOVE_HEAD_G1_Y 39
#define MOVE_HEAD_G1_Z 40
#define MOVE_FIX_G1_Y 41
#define MOVE_G2_X 42
#define MOVE_HEAD_G2_Y 43
#define MOVE_HEAD_G2_Z 44
#define MOVE_FIX_G2_Y 45
// ******** CF Variables ********
#define G1_FIXATION_DOING 0
#define G2_FIXATION_DOING 1
#define G1_CUTTING_DOING 2
#define G2_CUTTING_DOING 3
#define G1_FOAM_WIDTH 4
#define G2_FOAM_WIDTH 5
#define FORK_FIXATION_POINT_2_X 6
// ******** CM Variable **********
#define MODE 0
#define NOMODE_SUBMODES 1
#define PREPRODUCTION_SUBMODES 2
#define PRODUCTION_SUBMODES 3
#define CNC_OPERATION_DOING 4
#define FORK_START_HOMING 5

#endif 