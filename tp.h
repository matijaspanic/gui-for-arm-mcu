#define TP_SCK (1<<31)
#define TP_SI (1<<29)
#define TP_SO (1<<28)
#define TP_IRQ (1<<30)
//#define TP_CS 0x08

#define TP_AXIS_X 0x10
#define TP_AXIS_Y 0x50
#define TP_AXIS_Z1 0x30
#define TP_AXIS_Z2 0x40
#define TP_AXIS_Z 0x01

#define TP_PRECISION 12

int tp_get(int axis);
int tp_cal (int axis, int value);
