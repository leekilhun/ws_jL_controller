/*
 * flash.c
 *
 *  Created on: 2021. 7. 11.
 *      Author: gns2l
 */



#include "flash.h"
#include "cli.h"

#ifdef _USE_HW_FLASH


#define FLASH_SECTOR_MAX          64

typedef struct
{
  uint32_t addr;
  uint16_t length;
} flash_tbl_t;

static flash_tbl_t flash_tbl[FLASH_SECTOR_MAX] = {
   /* {0x08000000, 2*1024},
    {0x08000800, 2*1024},
    {0x08001000, 2*1024},
    {0x08001800, 2*1024},
    {0x08002000, 2*1024},
    {0x08002800, 2*1024},
    {0x08003000, 2*1024},
    {0x08003800, 2*1024},
    {0x08004000, 2*1024},
    {0x08004800, 2*1024},

    {0x08005000, 2*1024},
    {0x08005800, 2*1024},
    {0x08006000, 2*1024},
    {0x08006800, 2*1024},
    {0x08007000, 2*1024},
    {0x08007800, 2*1024},
    {0x08008000, 2*1024},
    {0x08008800, 2*1024},
    {0x08009000, 2*1024},
    {0x08009800, 2*1024},

    {0x0800A000, 2*1024},
    {0x0800A800, 2*1024},
    {0x0800B000, 2*1024},
    {0x0800B800, 2*1024},
    {0x0800C000, 2*1024},
    {0x0800C800, 2*1024},
    {0x0800D000, 2*1024},
    {0x0800D800, 2*1024},
    {0x0800E000, 2*1024},
    {0x0800E800, 2*1024},

    {0x0800F000, 2*1024},
    {0x0800F800, 2*1024},
    {0x08010000, 2*1024},
    {0x08010800, 2*1024},
    {0x08011000, 2*1024},
    {0x08011800, 2*1024},
    {0x08012000, 2*1024},
    {0x08012800, 2*1024},
    {0x08013000, 2*1024},
    {0x08013800, 2*1024},

    {0x08014000, 2*1024},
    {0x08014800, 2*1024},
    {0x08015000, 2*1024},
    {0x08015800, 2*1024},
    {0x08016000, 2*1024},
    {0x08016800, 2*1024},
    {0x08017000, 2*1024},
    {0x08017800, 2*1024},
    {0x08018000, 2*1024},
    {0x08018800, 2*1024},

    {0x08019000, 2*1024},
    {0x08019800, 2*1024},
    {0x0801A000, 2*1024},
    {0x0801A800, 2*1024},
    {0x0801B000, 2*1024},
    {0x0801B800, 2*1024},
    {0x0801C000, 2*1024},
    {0x0801C800, 2*1024},
    {0x0801D000, 2*1024},
    {0x0801D800, 2*1024},

    {0x0801E000, 2*1024},
    {0x0801E800, 2*1024},
    {0x0801F000, 2*1024},
    {0x0801F800, 2*1024},*/

};


#define FLASH_USER_START_ADDR   (FLASH_BASE + (55 * FLASH_PAGE_SIZE))   /* Start @ of user Flash area */
#define FLASH_USER_END_ADDR     (FLASH_BASE + FLASH_SIZE - 1)   /* End @ of user Flash area */

#define DATA_64                 ((uint64_t)0x1234567812345678)
#define DATA_32                 ((uint32_t)0x12345678)


static bool flashInSector(uint16_t sector_num, uint32_t addr, uint32_t length);
static uint32_t flashGetPage(uint32_t Addr);

#ifdef _USE_HW_CLI
static void cliFlash(cli_args_t *args);
#endif


bool flashInit(void)
{
  for (int i=0; i<FLASH_SECTOR_MAX; i++)
  {
    flash_tbl[i].addr   = FLASH_BASE + i*FLASH_PAGE_SIZE;
    flash_tbl[i].length = FLASH_PAGE_SIZE;
  }

#ifdef _USE_HW_CLI
  cliAdd("flash", cliFlash);
#endif

  return true;
}

bool flashErase(uint32_t addr, uint32_t length)
{
  bool ret = false;
  HAL_StatusTypeDef status;
  FLASH_EraseInitTypeDef init;
  uint32_t page_error;


  uint32_t FirstPage = 0, NbOfPages = 0;
  /* Get the 1st page to erase */
  FirstPage = flashGetPage(FLASH_USER_START_ADDR);

  /* Get the number of pages to erase from 1st page */
  NbOfPages = flashGetPage(FLASH_USER_END_ADDR) - FirstPage + 1;


  HAL_FLASH_Unlock();
  //HAL_FLASH_OB_Unlock();
  init.TypeErase   = FLASH_TYPEERASE_PAGES;
  init.Banks       = FLASH_BANK_1;
  init.Page 			 = FirstPage;//flash_tbl[start_sector_num].addr;
  init.NbPages     = NbOfPages;//sector_count;
  status = HAL_FLASHEx_Erase(&init, &page_error);
  if (status == HAL_OK)
  {
  	ret = true;

  }
  HAL_FLASH_Lock();



#if 0

  int16_t  start_sector_num = -1;
  uint32_t sector_count = 0;

  for (int i=0; i<FLASH_SECTOR_MAX; i++)
  {
    if (flashInSector(i, addr, length) == true)
    {
      if (start_sector_num < 0)
      {
        start_sector_num = i;
      }
      sector_count++;
    }
  }


  if (sector_count > 0)
  {
    HAL_FLASH_Unlock();

    init.TypeErase   = FLASH_TYPEERASE_PAGES;
    init.Banks       = FLASH_BANK_1;
    init.Page 			 = flash_tbl[start_sector_num].addr;
    init.NbPages     = sector_count;

    status = HAL_FLASHEx_Erase(&init, &page_error);
    if (status == HAL_OK)
    {
      ret = true;
    }

    HAL_FLASH_Lock();
  }

#endif

  return ret;
}

bool flashWrite(uint32_t addr, uint8_t *p_data, uint32_t length)
{
  bool ret = true;
  HAL_StatusTypeDef status;


  if (addr%2 != 0)
  {
    return false;
  }

  HAL_FLASH_Unlock();

  for (int i=0; i<length; i+=2)
  {
    uint16_t data;

    data  = p_data[i+0] << 0;
    data |= p_data[i+1] << 8;

    status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, addr + i, (uint64_t)data);
    if (status != HAL_OK)
    {
      ret = false;
      break;
    }
  }

  HAL_FLASH_Lock();

  return ret;
}

bool flashRead(uint32_t addr, uint8_t *p_data, uint32_t length)
{
  bool ret = true;
  uint8_t *p_byte = (uint8_t *)addr;


  for (int i=0; i<length; i++)
  {
    p_data[i] = p_byte[i];
  }

  return ret;
}

bool flashInSector(uint16_t sector_num, uint32_t addr, uint32_t length)
{
  bool ret = false;

  uint32_t sector_start;
  uint32_t sector_end;
  uint32_t flash_start;
  uint32_t flash_end;


  sector_start = flash_tbl[sector_num].addr;
  sector_end   = flash_tbl[sector_num].addr + flash_tbl[sector_num].length - 1;
  flash_start  = addr;
  flash_end    = addr + length - 1;


  if (sector_start >= flash_start && sector_start <= flash_end)
  {
    ret = true;
  }

  if (sector_end >= flash_start && sector_end <= flash_end)
  {
    ret = true;
  }

  if (flash_start >= sector_start && flash_start <= sector_end)
  {
    ret = true;
  }

  if (flash_end >= sector_start && flash_end <= sector_end)
  {
    ret = true;
  }

  return ret;
}


uint32_t flashGetPage(uint32_t Addr)
{
  return (Addr - FLASH_BASE) / FLASH_PAGE_SIZE;
}


#ifdef _USE_HW_CLI
void cliFlash(cli_args_t *args)
{
  bool ret = true;
  uint32_t i;
  uint32_t addr;
  uint32_t length;
  uint8_t  data;
  uint32_t pre_time;
  bool flash_ret;


  if (args->argc == 1)
  {
  	if(args->isStr(0, "info") == true)
  	{
  		for (int i=0; i<FLASH_SECTOR_MAX; i++)
  		{
  			cliPrintf("0x%X  : %dKB\n", flash_tbl[i].addr, flash_tbl[i].length/1024);
  		}
  	}
  	else
    {
      ret = false;
    }
  }
  else if (args->argc == 3)
  {
    if(args->isStr(0, "read") == true)
    {
      addr   = (uint32_t)args->getData(1);
      length = (uint32_t)args->getData(2);

      for (i=0; i<length; i++)
      {
        flash_ret = flashRead(addr+i, &data, 1);

        if (flash_ret == true)
        {
          cliPrintf( "addr : 0x%X\t 0x%02X\n", addr+i, data);
        }
        else
        {
          cliPrintf( "addr : 0x%X\t Fail\n", addr+i);
        }
      }
    }
    else if(args->isStr(0, "erase") == true)
    {
      addr   = (uint32_t)args->getData(1);
      length = (uint32_t)args->getData(2);

      pre_time = millis();
      flash_ret = flashErase(addr, length);

      cliPrintf( "addr : 0x%X\t len : %d %d ms\n", addr, length, (millis()-pre_time));
      if (flash_ret)
      {
        cliPrintf("OK\n");
      }
      else
      {
        cliPrintf("FAIL\n");
      }
    }
    else if(args->isStr(0, "write") == true)
    {
      addr = (uint32_t)args->getData(1);
      data = (uint8_t )args->getData(2);

      pre_time = millis();
      flash_ret = flashWrite(addr, &data, 1);

      cliPrintf( "addr : 0x%X\t 0x%02X %dms\n", addr, data, millis()-pre_time);
      if (flash_ret)
      {
        cliPrintf("OK\n");
      }
      else
      {
        cliPrintf("FAIL\n");
      }
    }
    else
    {
      ret = false;
    }
  }
  else
  {
    ret = false;
  }


  if (ret == false)
  {
    cliPrintf( "flash info\n");
    cliPrintf( "flash read  [addr] [length]\n");
    cliPrintf( "flash erase [addr] [length]\n");
    cliPrintf( "flash write [addr] [data]\n");
  }

}
#endif



#endif
