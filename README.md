# LTDC to HDMI with DCMIPP Preview Example

> [!IMPORTANT]
> This repository has been created using the `git submodule` command. Please
> refer to the ["How to clone"](README.md#how-to-clone) section for more
> details.

This example demonstrates how to use the ADV7513 LTDC to HDMI adapter board
(MB1227) to connect an HDMI display to the STM32N6570-DK board, replacing the
MB1860B LCD board.

By default, the firmware is configured with an output resolution of **800x480
(WVGA)** to get an identical display and layout between the LCD board and when
using the HDMI adpater. For enhanced visualization or compatibility issues with
different HDMI display, the output resolution can be configured to **720p
(1280x720)** or **VGA (640x480)** in `main.c`.

## Hardware Support

- [STM32N6570-DK](https://www.st.com/en/evaluation-tools/stm32n6570-dk.html) discovery board (MB1939-N6570-C02 Rev CR1).
  - *Note:* OTP for XSPI IOs must already be configured.
- LTDC to HDMI board (MB1227B) connected to an HDMI display.
- One of the following camera modules:
  - IMX335 camera module (provided with the STM32N6570-DK board).
  - [STEVAL-55G1MBI](https://www.st.com/en/evaluation-tools/steval-55g1mbi.html)
  - [STEVAL-66GYMAI1](https://www.st.com/en/evaluation-tools/steval-66gymai.html)

## Software Environment

- [STM32CubeIDE](https://www.st.com/content/st_com/en/products/development-tools/software-development-tools/stm32-software-development-tools/stm32-ides/stm32cubeide.html) (**STM32CubeIDE 1.17.0**)
- IAR Embedded Workbench for Arm (**EWARM 9.40.1**) + N6 patch ([**EWARMv9_STM32N6xx_V1.0.0**](STM32Cube_FW_N6/Utilities/PC_Software/EWARMv9_STM32N6xx_V1.0.0.zip))

## How to clone

This repository has been created using the `git submodule` command. Please check
the instructions below for proper use.

1. To **clone** this repository along with the linked submodules, option
   `--recursive` has to be specified as shown below.

    git clone --recursive
    https://github.com/STMicroelectronics/STM32N6570-DK-LTDC-to-HDMI.git

2. To get the **latest updates**, in case this repository is **already** on your
   local machine, issue the following **two** commands (with this repository as
   the **current working directory**).

    git pull git submodule update --init --recursive

## Quickstart

- Set the boot mode in development mode (both boot switches BOOT0 & BOOT1 to the
  right).
- Ensure HDMI cable is connected before running the firmware.
- Open your preferred toolchain.
- Rebuild all files and load your image into target memory. Code can be executed
  in this mode for debugging purposes.

Next, this program can be run in boot from flash mode. This is done by following
the instructions below:

    STM32_SigningTool_CLI -bin build/Project.bin -nk -t ssbl -hv 2.3 -o build/Project-signed.bin

    export DKEL="<STM32CubeProgrammer_N6 Install Folder>/bin/ExternalLoader/MX66UW1G45G_STM32N6570-DK.stldr"
    STM32_Programmer_CLI -c port=SWD mode=HOTPLUG -el $DKEL -hardRst -w bin/ai_fsbl.hex
    STM32_Programmer_CLI -c port=SWD mode=HOTPLUG -el $DKEL -hardRst -w build/Project-signed.bin 0x70100000

*Note:* `build/Project.bin` path is for Makefile build, adapt to your environment.

- Set the boot mode in boot from external Flash (both boot switches BOOT0 & BOOT1 to the right).
- Press the reset button. The code then executes in boot from external Flash mode.

## Limitations

- No support for hot plug & cable reconnect.
- Limited resolution options (WVGA, 720p, VGA).
- No double buffering implemented in this example.

## Tips for display compatibility issues

- Experiment with different resolutions, HDMI pixel clock and timings.
- Test using different HDMI displays.

## Tips for bandwidth issues

- Reduce the LTDC pixel clock (PCLK) frequency.
- Use only one layer (disable foreground layer).
- Play with DCMIPP IP-Plug and/or camera timings.
- Avoid simultaneous PSRAM access with other hardware resources.

## Resources

- https://www.analog.com/en/products/adv7513.html
- https://www.analog.com/media/en/technical-documentation/user-guides/ADV7513_Programming_Guide.pdf
- [AN4861](https://www.st.com/resource/en/application_note/an4861-introduction-to-lcdtft-display-controller-ltdc-on-stm32-mcus-stmicroelectronics.pdf) Introduction to LCD-TFT display controller (LTDC) on STM32 MCUs
