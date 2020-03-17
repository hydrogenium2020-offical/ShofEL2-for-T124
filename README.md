# ShofEL2 for T124

This is a Fusee Gelee / ShofEL2 exploit port for the Nvidia T124 (a.k.a Jetson TK1, Shield K1, etc).

Currently this code allows you to download and execute a payload to the T124. A couple of POC payloads are avaiable on the payloads folder.

Mostly of my code is based on the original ShofEL2 code and Katherine Temkin research, so I cannot take that much credit for this.

See the original fail0verflow blog post: https://fail0verflow.com/blog/2018/shofel2/
See additional info at the original Katherine Temkin github: https://github.com/Qyriad/fusee-launcher/blob/master/report/fusee_gelee.md

## Obligatory disclaimer

This code is provided without any warranty, use under your own resposability.

## Usage

You need an arm-*-eabi toolkit. You can use [Crosstool-ng](https://crosstool-ng.github.io/download/) compile it.

Build the loader and payloads:

    $ cd ShofEL2-for-T124/exploit
    $ make

Run the exploit

    $ cd ShofEL2-for-T124/exploit
    $ ./shofel2_t124 PAYLOAD_NAME.bin

## Interesting facts

* RCM loads the payload to IRAM at 0x4000E000 (described on tegrarcm source code).
* RCM cmd format is sligitly different. RCM cmd header length is 0x284 bytes but the firtst 4 bytes still containing the whole RCM cmd length.
* RCM cmd length restrictions are different to X1 bootrom:
	* Bulk transfers need to be multiply of 0x1000. TBC.
	* RCM cmd min length is 0x404 bytes. Due to the previous condition the minimun length would be 0x1004.
	* RCM cmd length minus 0x284 (header length) must be a multiple of 0x10 (which means RCM CMD length needs to end in 4)
	* RCM cmd length cannot exceed avaiable IRAM from 0x4000E000 till the end of it.
	* With all this in mind max RCM cmd length is 0x32374 bytes.
* A payload can still be loaded using the same path as the one used by the original shofEL2, since no validation is carried out till the whole payload is received.
* Even if the specs says that the JTAG is enabled by default, cold bootrom code disasbles it while is runnig (not as dumb as expected :D).
* RCM runs on an ARM7TDMI core. This CPU is supported by Segger J-LINK EDU and I could run the reset_example (halting on uboot) without any problem with it (see GDB init file).
* When the poisoned get status is executed, 0x30C bytes will be copied before the payload at 0x4000E0000 begins to be copied. These bytes are part of the execution stack, starting by the USB status var.
* Using the original sanity_check function from shofel2, I got from the execution stack that the RCM USB buffers are located at 0x40004000 and 0x40008000.
* Two USB buffers of 0x1000 bytes still in use. They still alternating on each USB txn. And odd number of USB txn will let you on the hight buffer for the next txn.
* Using the original sanity_check function from shofel2, I got from the execution stack that the memcpy return address is located at 0x4000DCD8 (0x4000DCF4 - 0xC - 2 * 4 - 2 * 4).
* The position in the RCM cmd where the entry adress need to be write to smash the memcpy return address is calculated as follow:
	* n_bytes_to_copy = 0x4000DCD8 - 0x40008000 (memcpy_ret_add_loc - usb_buf2_add) -> n_bytes_to_copy = 0x5CD8 bytes
	* pos_in_payload = n_bytes_to_copy - 0x30C (copied from the execution stack) - 0x4 -> pos_in_payload = 0x59C8
	* pos_in_rcm_cmd = pos_in_payload + 0x284 (header length) -> pos_in_rcm_cmd = 0x5C4C 

