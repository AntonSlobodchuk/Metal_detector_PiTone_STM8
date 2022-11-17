#include <iostm8s105s6.h>

#include "eeprom.h"

#define OPT_BaseAddress         0x4800
#define	BootLoader_RESET		0x6000

#define  OPERATION_TIMEOUT  ((uint)0x1000)

//--------------------------------------------------------------------------------------------------
/**
* @brief FLASH status of the last operation
*/
typedef enum {
  FLASH_STATUS_END_HIGH_VOLTAGE			= (uchar)0x40,	/*!< End of high voltage */
  FLASH_STATUS_SUCCESSFUL_OPERATION		= (uchar)0x04,	/*!< End of operation flag */
  FLASH_STATUS_TIMEOUT					= (uchar)0x02,	/*!< Time out error */
  FLASH_STATUS_WRITE_PROTECTION_ERROR	= (uchar)0x01	/*!< Write attempted to protected page */
} FLASH_Status_TypeDef;
//--------------------------------------------------------------------------------------------------
/**
  * @brief FLASH Memory types
  */
typedef enum {
    FLASH_MEMTYPE_PROG      = (uchar)0x00, /*!< Program memory */
    FLASH_MEMTYPE_DATA      = (uchar)0x01 /*!< Data EEPROM memory */
} FLASH_MemType_TypeDef;
//--------------------------------------------------------------------------------------------------
/**
* @brief Wait for a Flash operation to complete.
* @par Parameters:
* @param[in] MemType Memory type
* @retval FLASH_Status_TypeDef State of the last operation
* @par Required preconditions:
* The FLASH_WaitForLastOperation function should be executed from RAM.
* This function return the FLASH status and clears the EOP, WR_PG_DIS flags in the IAPSR register.
*/
__ramfunc FLASH_Status_TypeDef FLASH_WaitForLastOperation(FLASH_MemType_TypeDef MemType)
{
  uchar flagstatus = 0x00;
  uint timeout = OPERATION_TIMEOUT;
  /* Wait until operation completion or write protected page occured */
  if (MemType == FLASH_MEMTYPE_PROG)
  {
	while ((flagstatus == 0x00) && (timeout != 0x00))
	{
	  flagstatus = (uchar)(FLASH_IAPSR & (MASK_FLASH_IAPSR_EOP | MASK_FLASH_IAPSR_WR_PG_DIS));
	  timeout--;
	}
  }
  else
  {
	while ((flagstatus == 0x00) && (timeout != 0x00))
	{
	  flagstatus = (uchar)(FLASH_IAPSR & (MASK_FLASH_IAPSR_HVOFF | MASK_FLASH_IAPSR_WR_PG_DIS));
	  timeout--;
	}
  }
  if (timeout == 0x00 )
  {
	flagstatus = FLASH_STATUS_TIMEOUT;
  }

  return((FLASH_Status_TypeDef)flagstatus);
}
//--------------------------------------------------------------------------------------------------
void unlock_eeprom()
{
  FLASH_DUKR = 0xAE;	//разблокируем еепром для записи
  FLASH_DUKR = 0x56;
}
//--------------------------------------------------------------------------------------------------
/**
* @brief Programs an option byte
* @param[in] Address  option byte address  to program
* @param[in] Data Value to write
* @retval
* None
*/
__ramfunc void ProgramOptionByte(uint Address, uchar Data)
{
  /* Enable write access to option bytes */
  FLASH_CR2_bit.OPT = 1;
  FLASH_NCR2_bit.NOPT = 0;

  /* Program option byte and his complement */
  *((__near uchar*)Address) = Data;
  *((__near uchar*)(Address + 1)) = (uchar)(~Data);

  FLASH_WaitForLastOperation(FLASH_MEMTYPE_DATA);

  /* Disable write access to option bytes */
  FLASH_CR2_bit.OPT = 0;
  FLASH_NCR2_bit.NOPT = 1;
}
//--------------------------------------------------------------------------------------------------
__ramfunc void save_word_eeprom(uint *addres, uint data)
{
  *((__near uchar*)addres) = HIGH(data);
  *(((__near uchar*)addres) + 1) = LOW(data);
}
//--------------------------------------------------------------------------------------------------
__ramfunc void save_byte_eeprom(uchar *addres, uchar data)
{
  *((__near uchar*)addres) = data;
}
