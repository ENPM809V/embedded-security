#!/bin/sh

# Authors:
# - Brian Sidebotham <brian.sidebotham@gmail.com>

#scriptdir=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)

if [ $# -lt 1 ]; then
    #echo "usage: make_card.sh <pi-model> [model] [kernel_file]" >&2
    #echo "       pi-model options: rpi0, rpi1, rpi1bp, rpi2, rpi3, rpi4, rpibp" >&2
    echo "usage: make_card.sh [kernel_file]" >&2
    exit 1
fi

# The raspberry pi model we're targetting
model=rpi0  #"${1}"

kernel_file=kernel.img
if [ $# -ge 1 ]; then
    kernel_file=${1}
fi

diskspace=16

requires="sfdisk mcopy"
for required in ${requires}; do
    which ${required} > /dev/null 2>&1
    if [ $? -ne 0 ]; then
        echo "ERROR: Cannot find ${required} which is required" >&2
        exit 1
    fi
done

disk_image=card.${model}.img

tmpcardimg=$(mktemp)
tmpcardpart=$(mktemp)

# We generate a dummy file the same size as the disk size. We could just generate a file that has the same size as
# the parition table only, but sfdisk bombs out when you doing, reporting that the partition start position is past
# the end of the image. Perfectly acceptable failure mode I guess.
echo "Creating temporary partition ${tmpcardimg} to create the partition table"
dd bs=1024 count=$((1024*${diskspace})) if=/dev/zero of="${tmpcardimg}" > /dev/null

if [ $? -ne 0 ]; then
  echo "Failed to create the parititon table image" >&2
  exit 1
fi

# Create the partition table putting the FAT32 partition at 1MiB offset (2048 sector offset with 512 byte sector size)
# to allow for the partition table. We pipe a heredoc to sfdisk

# start=2048 -- The start sector for the partition
# size=      -- The size of the partition (so sfdisk can calculate the amount of sectors consumed by the partition)
#               This must be 1MiB less than the total disk size. which is taken up by the boot record space.
#               We've allowed 1MiB for that overhead (2048 sectors at 512bytes each ).
#               sfdisk 2.36+ will not allow us to create a partition larger than the space available on the disk
# type=c     -- 0xc is the WIN95 FAT partition type
# bootable   -- Set the bootable flag - don't think the RPi takes any notice of this to be fair
echo "Creating the master boot record for a FAT partition on ${tmpcardimg}"
cat << EOF | sfdisk "${tmpcardimg}"
label: dos

start=2048,size=$(( ${diskspace} - 1 ))M,type=c,bootable
EOF
if [ $? -ne 0 ]; then
  echo "Failed to create the Master Boot Record on ${tmpcardimg}" >&2
  exit 1
fi

# These are the normal files required to boot a Raspberry Pi - which come from the official Raspberry Pi github
# firmware repository. For the RPI4 we require a different startup file because it uses a different version of
# VideoCore
startfile=start.elf
fixupfile=fixup.dat
configfile=config.txt

#if [ "${model}X" = "rpi4X" ]; then
#    startfile=start4.elf
#    fixupfile=fixup4.dat
#fi

# See https://www.raspberrypi.org/documentation/configuration/boot_folder.md for the kernel filename
# options and what they mean
kernel_filename=kernel.img
#if [ "${model}X" = "rpi2X" ] || [ "${model}X" = "rpi2bp" ]; then
#    kernel_filename=kernel7.img
#elif [ "${model}X" = "rpi4X" ]; then
#    kernel_filename=kernel7l.img
#fi

# Now we create a separate partition file which we can write the filesystem on. This can then be concatentated to the
# parititon table we generated above
echo "Creating temporary card image ${tmpcardpart} partition for the filesystem"
dd bs=1024 count=$(($((${diskspace} - 1)) * 1024)) if=/dev/zero of="${tmpcardpart}" > /dev/null

if [ $? -ne 0 ]; then
  echo "Failed to create temporary card image" >&2
  exit 1
fi

# Generate a FAT file system (Leave mkfs.fat to work out which FAT to use)
# -I use the entire space of the partition (We've got the partition table elsewhere)
# -S 512 - make sure the geometry is 512 sector size
echo "Creating FAT filesystem on temporary card image ${tmpcardpart}"
mkfs.fat -S 512 -I "${tmpcardpart}"

if [ $? -ne 0 ]; then
  echo "Failed to create filesystem on temporary card image" >&2
  exit 1
fi

# mcopy is cool - it works with fat file systems in image files which saves us doing these things as root on a
# mounted loop device which is always flaky as hell.

# See the source code for the -i option which sets a disk disk_image instead of a device.
# https://github.com/Distrotech/mtools/blob/master/mcopy.c
# It's also mentioned in "man mtools" too
# Copy the kernel image file to the image's FAT file system and name the target file kernel.img
echo ${kernel_file}
mcopy -v -i ${tmpcardpart} ${kernel_file} ::${kernel_filename}

# Copy the rest of the files required in the same way
# bootcode.bin is no longer used by the rpi4 - we could leave it in place though and the rpi4 will ignore it.
if [ "${model}X" != "rpi4X" ]; then
  mcopy -v -i ${tmpcardpart} ../boot/bootcode.bin  ::bootcode.bin
fi
mcopy -v -i ${tmpcardpart} ../boot/${fixupfile}  ::${fixupfile}
mcopy -v -i ${tmpcardpart} ../boot/${startfile}  ::${startfile}
mcopy -v -i ${tmpcardpart} ../boot/${configfile} ::${configfile}

# Stich the disk image together by copying the partition table from the fake disk image and then concatentate the FAT
# file system partition on the end
dd bs=512 if="${tmpcardimg}" of="${disk_image}" count=2048
cat ${tmpcardpart} >> "${disk_image}"

rm -f ${tmpcardpart}
rm -f ${tmpcardimg}
