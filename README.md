# ShofEL2 for T124

This is an atempt to run the Fusee Gelee / ShofEL2 exploit on the T124 and maybe get full control of my Shield Tablet.

Mostly of my code is based on the original ShofEL2 code and Katherine Temkin research, so I cannot take that much credit for it.

At this moment it doesn't work, since the run stack smashing step doesn't look to do its job.

The USB stack allows to send poisonious GetStatus and it replies to those ones with lenghts smaler than 1000 bytes,
however it doesn't look to write out of the USB buffer (or maybe the run stack is not located after the USB buffer).

See the original fail0verflow blog post: https://fail0verflow.com/blog/2018/shofel2/
See additional info ati the original Katherine Temkin github: https://github.com/Qyriad/fusee-launcher/blob/master/report/fusee_gelee.md

## Obligatory disclaimer

This code is a non functional alpha, use under your own resposability, it could made your house on fire or worse....

## Usage

You need arm-linux-gnueabi
[Linaro](https://releases.linaro.org/components/toolchain/binaries/latest-7/)

Build the reset_example:

    $ cd shofel2/exploit
    $ make

Run the exploit

    $ cd shofel2/exploit
    $ python3 ./shofel2.py

## Interesting ideas

* RCM loads the payload to 0x4000E000 (described on tegrarcm source code).
* RCM CMD size restrictions are different to X1.	
* RCM CMD format is also sligitly different.
* A payload can still be loaded using the same technic as the one used by the original shofEL2, since no validation is performed till the whole payload is received.
* Even if the specs says that the JTAG is enabled by default, bootrom code disasbles it while is runnig (not as dumb as expected :D).
* RCM is run on an ARM7TDMI. This CPU is supported by Segger J-LINK EDU and I could run the reset_example without any problem with it (see GDB init file).
* TBC: Following the original shofEL2 code (sanity_check function), I would say that USB buffers are located at 0x40004000 and 0x40008000.
* TBC: I would say that memcpy is limiting the amount of data to 0x1000 or the destination adress is after the stack, wharever it is, the stack doesn't look
to get smashed (even using the biggest GetStatus size 0xFFFF), because the T124 keep repliying non poisonous GetStatus and allowing you to complete sedding the RCM CMD payload.
* Other vectors could be explored but at this time, I think that I am on a dead end.

