/*
 * ht1632.h
 * defintions for Holtek ht1632 LED driver.
 */


#if !defined(DEBUGPRINT)
#define DEBUGPRINT(fmt, args...)
#endif

/*
 * commands written to the chip consist of a 3 bit "ID", followed by
 * either 9 bits of "Command code" or 7 bits of address + 4 bits of data.
 */
#define HT1632_ID_CMD 4		/* ID = 100 - Commands */
#define HT1632_ID_RD  6		/* ID = 110 - Read RAM */
#define HT1632_ID_WR  5		/* ID = 101 - Write RAM */
#define HT1632_ID_BITS (1<<2)   /* IDs are 3 bits */

#define HT1632_CMD_SYSDIS 0x00	/* CMD= 0000-0000-x Turn off oscil */
#define HT1632_CMD_SYSON  0x01	/* CMD= 0000-0001-x Enable system oscil */
#define HT1632_CMD_LEDOFF 0x02	/* CMD= 0000-0010-x LED duty cycle gen off */
#define HT1632_CMD_LEDON  0x03	/* CMD= 0000-0011-x LEDs ON */
#define HT1632_CMD_BLOFF  0x08	/* CMD= 0000-1000-x Blink ON */
#define HT1632_CMD_BLON   0x09	/* CMD= 0000-1001-x Blink Off */
#define HT1632_CMD_SLVMD  0x10	/* CMD= 0001-00xx-x Slave Mode */
#define HT1632_CMD_MSTMD  0x14	/* CMD= 0001-01xx-x Master Mode */
#define HT1632_CMD_RCCLK  0x18	/* CMD= 0001-10xx-x Use on-chip clock */
#define HT1632_CMD_EXTCLK 0x1C	/* CMD= 0001-11xx-x Use external clock */
#define HT1632_CMD_COMS00 0x20	/* CMD= 0010-ABxx-x commons options */
#define HT1632_CMD_COMS01 0x24	/* CMD= 0010-ABxx-x commons options */
#define HT1632_CMD_COMS10 0x28	/* CMD= 0010-ABxx-x commons options */
#define HT1632_CMD_COMS11 0x2C	/* CMD= 0010-ABxx-x commons options */
#define HT1632_CMD_PWM    0xA0	/* CMD= 101x-PPPP-x PWM duty cycle */
#define HT1632_CMD_BITS (1<<7)


// tomnab

// possible values for a pixel;
#define BLACK  0
#define GREEN  1
#define RED    2
#define ORANGE 3

#define X_MAX 32
#define Y_MAX 16
#define CHIP_MAX 4 //Four HT1632Cs on one board
#define CLK_DELAY


#define plot(x,y,v)  ht1632_plot(x,y,v)
#define cls          ht1632_clear


// our own copy of the "video" memory; 64 bytes for each of the 4 screen quarters;
// each 64-element array maps 2 planes:
// indexes from 0 to 31 are allocated for green plane;
// indexes from 32 to 63 are allocated for red plane;
// when a bit is 1 in both planes, it is displayed as orange (green + red);
extern byte ht1632_shadowram[64][4];
extern unsigned char Tab7Segts[];


/*
 * Set these constants to the values of the pins connected to the SureElectronics Module
 */
static const byte ht1632_data = 6;  // Data pin (pin 7 of display connector)
static const byte ht1632_wrclk = 7; // Write clock pin (pin 5 of display connector)
static const byte ht1632_cs = 8;    // Chip Select (pin 1 of display connnector)
static const byte ht1632_clk = 9; // clock pin (pin 2 of display connector)


#define DEMOTIME 10000  // 30 seconds max on each demo is enough.
#define DISPDELAY 40    // Each "display" lasts this long
#define LONGDELAY 1000  // This delay BETWEEN demos

void OutputCLK_Pulse(void);
void OutputA_74164(unsigned char x);
void ChipSelect(int select);
void ht1632_writebits (byte bits, byte firstbit);
static void ht1632_sendcmd (byte chipNo, byte command);
static void ht1632_senddata (byte chipNo, byte address, byte data);
void ht1632_setup();
void ht1632_plot (byte x, byte y, byte color);
void ht1632_clear();
void setup7Seg(void);
