# init
# docker run -it \
# --platform linux/amd64 \
# -v ".:/csapp" \
# --name=csapp \
# ubuntu /bin/bash

docker run -it \
--platform linux/amd64 \
-v ".:/csapp" \
--name=csapp \
csapp_image /bin/bash

# # in vm
# apt-get install qemu-user-static

# # debug one x86 binary
# cd /root/bomb
# qemu-x86_64-static -g 1234 ./bomb


# # in another tty
# gdb ./bomb
# (gdb) target remote :1234
# (gdb) b *0x400ee4
# Breakpoint 1 at 0x400ee4
# (gdb) continue
# Continuing.

# Breakpoint 1, 0x0000000000400ee4 in phase_1 ()
# (gdb) x/s 0x402400
# 0x402400:	"Border relations with Canada have never been better."
# (gdb) quit