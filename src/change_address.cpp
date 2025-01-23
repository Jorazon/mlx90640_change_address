#include <cstdlib>
#include <functional>
#include <string>
#include <stdint.h>
#include <iostream>
#include <cstring>
#include <fstream>
#include <chrono>
#include <thread>
#include "MLX90640_API.h"
#include "MLX90640_I2C_Driver.h"

#define FMT_STRING "\u2588\u2588"

#define MLX_I2C_ADDR 0x33
#define MLX_ZERO 0x0000
#define MLX_EEPROM_ADDR_I2C_ADDR 0x240F
#define MLX_REGISTER_ADDR_I2C_ADDR 0x8010

int msleep(long msec)
{
    struct timespec ts;
    int res;

    if (msec < 0)
    {
        errno = EINVAL;
        return -1;
    }

    ts.tv_sec = msec / 1000;
    ts.tv_nsec = (msec % 1000) * 1000000;

    do
    {
        res = nanosleep(&ts, &ts);
    } while (res && errno == EINTR);

    return res;
}

/*void change_address(std::string addr, std::string old_addr = "0x33")
{
    unsigned int new_address = std::stoul(addr, nullptr, 16);
    int old_address = MLX_I2C_ADDR;
    if(old_addr != "0x33"){
    old_addr = std::stoi(old_addr, nullptr, 16);
    std::cout << "Old address " << old_addr << std::endl;
    }
    if(!(new_address > 48642 && new_address < 48760)){
        std::cout << "Faulty new address: " << new_address << std::endl;
        return;
    }
    if(!(old_address > 0x02 && old_address < 0x78)){
    std::cout << "Faulty old_address: " << old_address << std::endl;
    return;
    }
    uint16_t data;
    MLX90640_I2CRead(old_address, 0x240F, 1, &data);
    std::cout << "Current address: " << std::hex << data << std::endl;
    MLX90640_I2CWrite(old_address, 0x240F, 0);
    msleep(1000);
    MLX90640_I2CWrite(old_address, 0x240F, new_address);
}*/

int change_address(uint8_t slaveAddr, uint8_t newAddr)
{
    uint16_t result_orig;
    uint16_t result;
    uint16_t value;
    int error = 1;

    error = MLX90640_I2CRead(slaveAddr, MLX_EEPROM_ADDR_I2C_ADDR, 1, &result_orig);
    if (error == 0)
    {
        printf("Value of I2C device 0x%02x at EEPROM address 0x%04x: 0x%04x\n", slaveAddr, MLX_EEPROM_ADDR_I2C_ADDR, result_orig);
        printf("Writing value 0x%04x to EEPROM address 0x%04x.\n", MLX_ZERO, MLX_EEPROM_ADDR_I2C_ADDR);
        error = MLX90640_I2CWrite(slaveAddr, MLX_EEPROM_ADDR_I2C_ADDR, MLX_ZERO);
        msleep(50);
        if (error == 0)
        {
            error = MLX90640_I2CRead(slaveAddr, MLX_EEPROM_ADDR_I2C_ADDR, 1, &result);
            printf("Value of I2C device 0x%02x at EEPROM address 0x%04x: 0x%04x\n", slaveAddr, MLX_EEPROM_ADDR_I2C_ADDR, result);
            value = (result_orig & 0xFF00) | newAddr;
            printf("Writing value 0x%04x to EEPROM address 0x%04x.\n", value, MLX_EEPROM_ADDR_I2C_ADDR);
            error = MLX90640_I2CWrite(slaveAddr, MLX_EEPROM_ADDR_I2C_ADDR, value);
            msleep(50);
            if (error == 0)
            {
                error = MLX90640_I2CRead(slaveAddr, MLX_EEPROM_ADDR_I2C_ADDR, 1, &result);
                printf("Value of I2C device 0x%02x at EEPROM address 0x%04x: 0x%04x\n", slaveAddr, MLX_EEPROM_ADDR_I2C_ADDR, result);
                printf("Please reboot the device (disconnect from power and connect to power).\n");
            }
            else
            {
                printf("Error (error=%i) writing value 0x%04x to I2C device 0x%02x to EEPROM address 0x%04x.\n", error, value, slaveAddr, MLX_EEPROM_ADDR_I2C_ADDR);
            }
        }
        else
        {
            printf("Error (error=%i) writing value 0x%04x to I2C device 0x%02x to EEPROM address 0x%04x.\n", error, MLX_ZERO, slaveAddr, MLX_EEPROM_ADDR_I2C_ADDR);
        }
    }
    else
    {
        printf("Error (error=%i) reading from I2C device 0x%02x at EEPROM address 0x%04x.\n", error, slaveAddr, MLX_EEPROM_ADDR_I2C_ADDR);
    }
    return error;
}

int main(int argc, char *argv[])
{
    uint8_t addr_new;
    uint8_t addr_old;
    if (argc < 2)
    {
        std::cout << "Usage ./change_address *new_address* as hex string between 0x30 and 0x35 and *optional old address* as hex string between 0x30 and 0x35" << std::endl;
        return 1;
    }
    else if (argc == 2)
    {
        addr_new = (uint8_t)strtol(argv[1], NULL, 0);
        addr_old = (uint8_t)strtol("0x33", NULL, 0);
        change_address(addr_old, addr_new);
        // change_address(argv[1]);
    }
    else if (argc == 3)
    {
        for (int i = 0; i < 3; i++)
        {
            std::cout << argv[i] << std::endl;
        }
        addr_new = (uint8_t)strtol(argv[1], NULL, 0);
        addr_old = (uint8_t)strtol(argv[2], NULL, 0);
        change_address(addr_old, addr_new);
        // change_address(argv[1], argv[2]);
    }
}