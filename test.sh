# 宣告使用 /bin/bash
#!/bin/bash

# MAKE_CMD_example="cd /code/riscv-example && make clean && make && cd $CUR_DIR"
# echo $MAKE_CMD_example
# ( cd /code/riscv-example && make clean && make && cd $CUR_DIR )

# MAKE_CMD_cart="cd /code/riscv-cart && make clean && make && cd $CUR_DIR"
# echo $MAKE_CMD_cart
# ( cd /code/riscv-cart && make clean && make && cd $CUR_DIR )

MAKE_CMD_firmware="cd /code/riscv-firmware && make clean && make && cd $CUR_DIR"
echo $MAKE_CMD_firmware
( cd /code/riscv-firmware && make clean && make && cd $CUR_DIR )

#MAKE_CMD_firmware＿context="cd /code/riscv-firmware-context && make clean && make && cd $CUR_DIR"
#echo $MAKE_CMD_firmware＿context
#( cd /code/riscv-firmware-context && make clean && make && cd $CUR_DIR )

MAKE_CMD_cartridge="cd /code/riscv-cartridge && make clean && make && cd $CUR_DIR"
echo $MAKE_CMD_cartridge
( cd /code/riscv-cartridge && make clean && make && cd $CUR_DIR )

RUNSIM='/code/runsim.sh -d'
echo $VIDEO_MEMORY
( /code/runsim.sh -d )