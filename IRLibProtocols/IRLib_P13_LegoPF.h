/* IRLib_P99_Additional.h
 * Part of IRLib Library for Arduino receiving, decoding, and sending
 * infrared signals. See COPYRIGHT.txt and LICENSE.txt for more information.
 */
/*
 * This is dummy code that you can copy and rename and modify when implementing new protocols.
 */

#ifndef IRLIB_PROTOCOL_13_H
#define IRLIB_PROTOCOL_13_H
#define IR_SEND_PROTOCOL_13		case 13: IRsendLegoPF::send(data); break;
#define IR_DECODE_PROTOCOL_13	if(IRdecodeLegoPF::decode()) return true;
#ifdef IRLIB_HAVE_COMBO
	#define PV_IR_DECODE_PROTOCOL_13 ,public virtual IRdecodeLegoPF
	#define PV_IR_SEND_PROTOCOL_13   ,public virtual IRsendLegoPF
#else
	#define PV_IR_DECODE_PROTOCOL_13  public virtual IRdecodeLegoPF
	#define PV_IR_SEND_PROTOCOL_13    public virtual IRsendLegoPF
#endif

#define LEGO_MARK            158    // 6 * 1/38 KHz
#define LEGO_SPACE_HEADER    1026   // 39 * 1/38 KHz
#define LEGO_SPACE_ZERO      263    // 10 * 1/38 KHz
#define LEGO_SPACE_ONE       553    // 21 * 1/38 KHz

/*
Low bit range 316 - 526 us
High bit range 526 – 947 us
Start/stop bit range 947 – 1579 us
#define LEGO_ZERO_MAX        
*/

typedef struct {
  bool toggle; // 0
  bool escape; // 1
  int8_t channel; // 2-3
  bool address; // 4
  int8_t mode; // 5-7
  int8_t data; // 8-11
  int8_t lrc; // 12-15
} LegoPF;

#ifdef IRLIBSENDBASE_H
class IRsendLegoPF: public virtual IRsendBase {
  public:
    void IRsendLegoPF::send(uint32_t data) {
    //void IRsendAdditional::send(uint32_t data, uint32_t data2)//optional form
      /*********
       *  Insert your code here.
       *********/
    };
};
#endif  //IRLIBSENDBASE_H

#ifdef IRLIBDECODEBASE_H
class IRdecodeLegoPF: public virtual IRdecodeBase {
  public:
    bool IRdecodeLegoPF::decode(void) {
      uint16_t data;
      uint16_t tsum,mark,space;
      resetDecoder();
      IRLIB_ATTEMPT_MESSAGE(F("LegoPF"));

      if ( (recvGlobal.decodeLength!=(32+4)) ) return RAW_COUNT_ERROR;

      if (!ignoreHeader) {
        mark = recvGlobal.decodeBuffer[1];
        space = recvGlobal.decodeBuffer[2];
        if (!(mark >= 70 && mark <= 200))
          return HEADER_MARK_ERROR(LEGO_MARK);
        if (!MATCH(space,LEGO_SPACE_HEADER))
          return HEADER_SPACE_ERROR(LEGO_SPACE_HEADER);
      }

      offset=3; //data=0;

      while (offset < (recvGlobal.decodeLength-1)) {

         if((offset & 1) != 0) {
           mark = recvGlobal.decodeBuffer[offset];
           if( !(mark >= 120 && mark <= 200) )
             return DATA_MARK_ERROR(LEGO_MARK);
         }
         else {
           space = recvGlobal.decodeBuffer[offset];
           tsum =  mark + space; 

           if( space >= 200 && space <= 330 && tsum >= 320 && tsum <= 530)
              // '0'
              data = data << 1;
           else
           if( space >= 410 && space <= 750 && tsum >= 530 && tsum <= 950)
              // '1'
              data = (data << 1) | 1;
           else
             return DATA_SPACE_ERROR(LEGO_SPACE_ZERO);
         }
         
         
         offset += 1;
/*
Low bit range 316 - 526 us
High bit range 526 – 947 us
Start/stop bit range 947 – 1579 us
#define LEGO_ZERO_MAX        
*/

      }

      bits = recvGlobal.decodeLength-4;
      value = data; 
      protocolNum=LEGOPF;
      return true;
    }
    

};

#endif //IRLIBDECODEBASE_H

#define IRLIB_HAVE_COMBO

#endif //IRLIB_PROTOCOL_99_H
