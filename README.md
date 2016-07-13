# Hypervisor

The project contains code on writing a simple hypervisor by entering the VMM mode on intel chips.

## Steps required to achieve this
1. First go through this tutorial series (http://www.brokenthorn.com/Resources/OSDevIndex.html) for understanding the basics of Bootloaders and Operating Systems. 
2. All the code in those tutorials is based on 32 bit processors. So for entering VMM mode we have to enter long mode. For this follow the instructions in this site (http://wiki.osdev.org/Setting_Up_Long_Mode).
3. Now that we have entered Long mode go through the instructions in the Intel Manual (http://www.intel.com/content/dam/www/public/us/en/documents/manuals/64-ia-32-architectures-software-developer-system-programming-manual-325384.pdf) and the code. Chapters 24 and Appendix B are especially helpful. Also i would like to credit Micahel Courdakis(http://www.turboirc.com/asm/z6.htm) for some reference on how to achieve this.

## Setup
Use the bochs emulator for running the OS like mentioned in the tutorials at brokenthorn. Also it's very handy to have the bochs `Visual Studio Build (https://sourceforge.net/projects/bochs/files/bochs/2.6.8/bochs-2.6.8-msvc-src.zip/download)` with debugging enabled so that you can step through bochs code to figure the exact reason for the triple fault.

If anything is unclear raise an issue and hopefully I'll address it :).