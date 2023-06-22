"""
" Return the CRC32 of a given file
"
" Mostly used to keep the filename conventions used by Jagex
" when publishing their own browsercontrol JAR.
"
" This should not be used to verify the file as CRC32 is not
" a secure method of file verification.
"
" A potential method which could be used would be to provide
" a SHA256 hash within the jav_config.ws file along-side the
" reference to the browsercontrol filename.
"
" In the future we might also use `jarsigner` - but I don't
" have the money for a code signing certificate from Thawte.
"""
import sys
import zlib
from typing import BinaryIO

CHUNK_SIZE = 65536


def checksum_to_signed_string(checksum: int) -> str:
    # The result is an unsigned 32-bit integer.
    #
    # https://docs.python.org/3/library/zlib.html#zlib.crc32
    signed = int.from_bytes(checksum.to_bytes(4, sys.byteorder, signed=False), sys.byteorder, signed=True)

    return signed.__str__()


def calculate_crc32(fp: BinaryIO) -> str:
    checksum = 0

    while chunk := fp.read(CHUNK_SIZE):
        checksum = zlib.crc32(chunk, checksum)

    """
    " When crafting their filenames Jagex used a
    " signed representation of the file's CRC32
    " hash.
    """
    return checksum_to_signed_string(checksum)


def main():
    if len(sys.argv) != 2:
        print('Usage: {0} <FILE_PATH>', format(sys.argv[0]), file=sys.stderr)
        sys.exit(1)

    file_path = sys.argv[1]
    with open(file_path, 'rb') as fp:
        print(calculate_crc32(fp))

    sys.exit(0)


if __name__ == '__main__':
    main()
