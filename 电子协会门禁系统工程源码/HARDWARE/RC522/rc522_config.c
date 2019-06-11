
#include "rc522_config.h"
#include "stm32f10x.h"

/***************************************************************************
引脚接线：
				STM32F103ZET6核心板   -------------------  RC522射频读卡器
				接PA4-----------------------------SDA
				接PA5-----------------------------SCK
				接PA7-----------------------------MOSI
				接PA6-----------------------------MISO
				空 脚------------------------------ IRQ
				接GND-----------------------------GND
				接PB0------------------------------ RST
				接3.3V------------------------------ 3.3V
***************************************************************************/
static void             RC522_SPI_Config             ( void );

/**
  * @brief  RC522_Init
  * @param  无
  * @retval 无
  */
void RC522_Init ( void )
{
	RC522_SPI_Config ();	
	RC522_Reset_Disable();	
	RC522_CS_Disable();
}

/**
  * @brief  R522 SPI配置
  * @param  无
  * @retval 无
  */
static void RC522_SPI_Config ( void )
{
  /* SPI_InitTypeDef  SPI_InitStructure */
  GPIO_InitTypeDef GPIO_InitStructure;
  
	/*!< Configure SPI_RC522_SPI pins: CS */
	RC522_GPIO_CS_CLK_FUN ( RC522_GPIO_CS_CLK, ENABLE );
  GPIO_InitStructure.GPIO_Pin = RC522_GPIO_CS_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = RC522_GPIO_CS_Mode;
  GPIO_Init(RC522_GPIO_CS_PORT, &GPIO_InitStructure);
	
  /*!< Configure SPI_RC522_SPI pins: SCK */
	RC522_GPIO_SCK_CLK_FUN ( RC522_GPIO_SCK_CLK, ENABLE );
  GPIO_InitStructure.GPIO_Pin = RC522_GPIO_SCK_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = RC522_GPIO_SCK_Mode;
  GPIO_Init(RC522_GPIO_SCK_PORT, &GPIO_InitStructure);
	
  /*!< Configure SPI_RC522_SPI pins: MOSI */
	RC522_GPIO_MOSI_CLK_FUN ( RC522_GPIO_MOSI_CLK, ENABLE );
  GPIO_InitStructure.GPIO_Pin = RC522_GPIO_MOSI_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = RC522_GPIO_MOSI_Mode;
  GPIO_Init(RC522_GPIO_MOSI_PORT, &GPIO_InitStructure);

  /*!< Configure SPI_RC522_SPI pins: MISO */
	RC522_GPIO_MISO_CLK_FUN ( RC522_GPIO_MISO_CLK, ENABLE );
  GPIO_InitStructure.GPIO_Pin = RC522_GPIO_MISO_PIN;
  GPIO_InitStructure.GPIO_Mode = RC522_GPIO_MISO_Mode;
  GPIO_Init(RC522_GPIO_MISO_PORT, &GPIO_InitStructure);	
		
  /*!< Configure SPI_RC522_SPI pins: RST */
	RC522_GPIO_RST_CLK_FUN ( RC522_GPIO_RST_CLK, ENABLE );
  GPIO_InitStructure.GPIO_Pin = RC522_GPIO_RST_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = RC522_GPIO_RST_Mode;
  GPIO_Init(RC522_GPIO_RST_PORT, &GPIO_InitStructure);
	
}


