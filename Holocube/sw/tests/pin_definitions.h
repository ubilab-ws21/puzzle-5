/*
 * This file contains the pin definitions/interfaces for the Holocube.
 * It also contains addresses for I2C slaves. Include this file in all tests.
 *      Author: fedy0
*/

// Pin Interfaces
#define RGB_PIN                GPIO_NUM_4
#define IMU_INTERRUPT          GPIO_NUM_20
#define SYSTEM_I2C_SDA_PIN     GPIO_NUM_21
#define SYSTEM_I2C_SCL_PIN     GPIO_NUM_22
#define SYSTEM_UART_RX_PIN     GPIO_NUM_1
#define SYSTEM_UART_TX_PIN     GPIO_NUM_3
#define SDCARD_SPI_CS_PIN      GPIO_NUM_15
#define SDCARD_SPI_CLK_PIN     GPIO_NUM_14
#define SDCARD_SPI_MOSI_PIN    GPIO_NUM_13
#define SDCARD_SPI_MISO_PIN    GPIO_NUM_26
#define LCD_RESET_PIN          GPIO_NUM_23
#define LCD_BACKLIGHT          GPIO_NUM_19
#define LCD_SPI_DC_PIN         GPIO_NUM_0
#define LCD_SPI_CLK_PIN        GPIO_NUM_5
#define LCD_SPI_MOSI_PIN       GPIO_NUM_18


// I2C Slave Addresses
#define LIGHT_SENSOR_ADDRESS   0x23
#define IMU_SENSOR_ADDRESS     0x68

