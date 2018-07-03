//#ifndef _GLOBAL_H_
//#define _GLOBAL_H_

#define 	_BV(bit)   (1 << (bit))
#define 	bit_is_set(sfr, bit)   (sfr & _BV(bit))
#define 	bit_is_clear(sfr, bit)   (!(sfr & _BV(bit)))
#define 	loop_until_bit_is_set(sfr, bit)   do { } while (bit_is_clear(sfr, bit))
#define 	loop_until_bit_is_clear(sfr, bit)   do { } while (bit_is_set(sfr, bit))

#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#define Dprintf if (Debug) printf("%s ", __FILENAME__); if (Debug) printf

// ----------------------------------------------------------------------------
#define	true	1
#define	false	0

#ifndef TRUE
typedef enum boolean2 {FALSE=0,TRUE=1} boolean;
#define	TRUE 1
#define	FALSE 0
#endif

#ifndef int8_t
typedef signed char int8_t;
#endif
#ifndef uint8_t
typedef unsigned char uint8_t;
#endif
#ifndef uint16_t
typedef unsigned short uint16_t;
#endif
#ifndef Uint32_t
typedef unsigned long int Uint32_t;
#endif

#ifndef byte
typedef unsigned char byte;
#endif

typedef unsigned int   UINT;
typedef unsigned char  BOOL;
typedef unsigned char  BYTE;
typedef unsigned short WORD;
typedef unsigned long  DWORD;
typedef unsigned short word;

#define sqr(num) (num)*(num)


//typedef	_Bool bool;
//typedef	boolean Bool;
// ----------------------------------------------------------------------------

/*#define	RESET(x)		_XRS(x)
#define	SET(x)			_XS(x)
#define	TOGGLE(x)		_XT(x)
#define	SET_OUTPUT(x)	_XSO(x)
#define	SET_INPUT(x)	_XSI(x)
#define	IS_SET(x)		_XR(x)

#define	PORT(x)			_port2(x)
#define	DDR(x)			_ddr2(x)
#define	PIN(x)			_pin2(x)

#define	_XRS(x,y)	PORT(x) &= ~(1<<y)
#define	_XS(x,y)	PORT(x) |= (1<<y)
#define	_XT(x,y)	PORT(x) ^= (1<<y)

#define	_XSO(x,y)	DDR(x) |= (1<<y)
#define	_XSI(x,y)	DDR(x) &= ~(1<<y)

#define	_XR(x,y)	((PIN(x) & (1<<y)) != 0)

#define	_port2(x)	PORT ## x
#define	_ddr2(x)	DDR ## x
#define	_pin2(x)	PIN ## x
*/

//#endif	// GLOBAL_H
