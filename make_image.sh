#!/bin/zsh

set -euo pipefail

IMAGE=pi_os.img
SIZE_MB=512

# FAT partition starts at sector 2048 (1 MiB alignment)
START_SECTOR=2048
SECTOR_SIZE=512
PARTITION_SIZE=$((SIZE_MB * 1024 * 1024 - START_SECTOR * SECTOR_SIZE))

echo "[*] Creating empty SD image"
rm -f "$IMAGE"
truncate -s "${SIZE_MB}m" "$IMAGE"


echo "[*] Creating MBR partition table"

# Create MBR:
# partition 1:
#   type 0x0c FAT32 LBA
#   start sector 2048
#   length = remaining sectors

python3 <<EOF
import struct

image="$IMAGE"
size_mb=$SIZE_MB
start=$START_SECTOR

sectors = size_mb * 1024 * 1024 // 512
length = sectors - start

mbr = bytearray(512)

# partition entry
entry = 446

mbr[entry + 0] = 0x00       # boot flag
mbr[entry + 4] = 0x0c       # FAT32 LBA

# starting LBA
mbr[entry+8:entry+12] = struct.pack("<I", start)

# number of sectors
mbr[entry+12:entry+16] = struct.pack("<I", length)

# signature
mbr[510:512] = b"\\x55\\xaa"

with open(image,"r+b") as f:
    f.write(mbr)
EOF


echo "[*] Creating FAT32 filesystem"

mkfs.fat \
    -F 32 \
    -S 512 \
    -n BOOT \
    -C boot.fat \
    $((PARTITION_SIZE / 1024))


echo "[*] Copying FAT filesystem into image"

dd \
    if=boot.fat \
    of="$IMAGE" \
    bs=512 \
    seek=$START_SECTOR \
    conv=notrunc


rm boot.fat


echo "[*] Adding boot files"

mcopy -i "$IMAGE@@$((START_SECTOR * 512))" ../firmware/bootcode.bin ::
mcopy -i "$IMAGE@@$((START_SECTOR * 512))" ../firmware/start.elf ::
mcopy -i "$IMAGE@@$((START_SECTOR * 512))" ../firmware/fixup.dat ::
mcopy -i "$IMAGE@@$((START_SECTOR * 512))" ../firmware/bcm2710-rpi-3-b.dtb ::

if [ -d firmware/overlays ]; then
    mmd -i "$IMAGE@@$((START_SECTOR * 512))" ::/overlays
    mcopy -i "$IMAGE@@$((START_SECTOR * 512))" ../firmware/overlays/* ::/overlays/
fi

mcopy -i "$IMAGE@@$((START_SECTOR * 512))" kernel8.img ::

cat >/tmp/config.txt <<EOF
arm_64bit=1
kernel=kernel8.img
enable_uart=1
EOF

mcopy -i "$IMAGE@@$((START_SECTOR * 512))" /tmp/config.txt ::
rm /tmp/config.txt


echo
echo "[+] Created $IMAGE"