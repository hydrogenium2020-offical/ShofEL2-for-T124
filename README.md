# ShofEL2 for T124

This is a Fusee Gelee / ShofEL2 exploit port for the Nvidia T124. Hopefully this will allow me to take full control of my Shield Tablet.

Currently I can load a payload without problems, but still required to write a usefull payload. A couple of examples are aviable on the payloads folder.

Mostly of my code is based on the original ShofEL2 code and Katherine Temkin research, so I cannot take that much credit for this.

See the original fail0verflow blog post: https://fail0verflow.com/blog/2018/shofel2/
See additional info at the original Katherine Temkin github: https://github.com/Qyriad/fusee-launcher/blob/master/report/fusee_gelee.md

## Obligatory disclaimer

This code is provided without any warranty, use under your own resposability, it could made your house on fire or worse....

## Usage

You need arm-unknown-eabi
[Crosstool-ng](https://crosstool-ng.github.io/download/)

Build the loader and payloads:

    $ cd ShofEL2-for-T124/exploit
    $ make

Run the exploit

    $ cd ShofEL2-for-T124/exploit
    $ ./shofel2_t124 PAYLOAD_NAME.bin

## Interesting facts

* RCM loads the payload to 0x4000E000 (described on tegrarcm source code).
* RCM CMD size restrictions are different to X1.	
* RCM CMD format is also sligitly different.
* A payload can still be loaded using the same technic as the one used by the original shofEL2, since no validation is performed till the whole payload is received.
* Even if the specs says that the JTAG is enabled by default, bootrom code disasbles it while is runnig (not as dumb as expected :D).
* RCM is run on an ARM7TDMI. This CPU is supported by Segger J-LINK EDU and I could run the reset_example (halting on uboot) without any problem with it (see GDB init file).
* Using the original sanity_check function from shofel2, I would say that RCM USB buffers are located at 0x40004000 and 0x40008000.

