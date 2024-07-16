import ctypes
import os
from math import ceil

from cryptography.hazmat.primitives.ciphers import Cipher, algorithms, modes


class AESCounterMode:
    # Sizes in bytes
    KEY_SIZE = 16
    BLOCK_SIZE = 16
    NONCE_SIZE = 15

    def __init__(self, key: bytes, nonce: bytes) -> None:
        self._key = key
        self._cipher = Cipher(algorithms.AES(self._key), modes.ECB())

        # IV = nonce + counter
        self._counter = ctypes.c_ubyte(0)
        self._nonce = nonce

    def _generate_stream_key(self, num_bytes: int) -> bytes:
        bef = self._counter.value
        nb_blocks = ceil(num_bytes / AESCounterMode.BLOCK_SIZE)

        # Generates the counter blocks which will be used in generating the
        # stream key. Counter blocks have the following form:
        # [NONCE | COUNTER + 0] [NONCE | COUNTER + 1] [NONCE | COUNTER + 2] ...
        counter_blocks = b"".join(
            self._nonce + ctypes.c_ubyte(i).value.to_bytes(1, "big")
            for i in range(self._counter.value, self._counter.value + nb_blocks)
        )
        self._counter.value += nb_blocks

        print(bef, "->", self._counter.value)
        # Generate the streaming key
        encryptor = self._cipher.encryptor()
        stream_key = encryptor.update(counter_blocks) + encryptor.finalize()
        return stream_key[:num_bytes]

    def __call__(self, data: bytes) -> bytes:
        """Encrypts/decrypts some data using AES in counter mode.

        The decryption scheme is the same as encryption, but plaintext
        and ciphertext are reversed (see https://tinyurl.com/9phyjk4x).
        The user must make sure to use the right AES encryption key and IV.

        Args:
            data (bytes): Some data to encrypt/decrypt

        Returns:
            bytes: The result of the encryption/decryption.
        """
        stream_key = self._generate_stream_key(len(data))
        return bytes(a ^ b for a, b in zip(stream_key, data))

    @staticmethod
    def generate_key() -> bytes:
        """Generates a random encryption key"""
        return os.urandom(AESCounterMode.KEY_SIZE)

    @staticmethod
    def generate_nonce() -> bytes:
        """Generates a random nonce"""
        return os.urandom(AESCounterMode.NONCE_SIZE)


def test() -> None:
    import timeit

    def test_case(num_reps: int, plain_size: int) -> None:
        key = AESCounterMode.generate_key()
        nonce = AESCounterMode.generate_nonce()
        encrypt = AESCounterMode(key, nonce)
        decrypt = AESCounterMode(key, nonce)

        for _ in range(num_reps):
            plain = os.urandom(plain_size)
            assert decrypt(encrypt(plain)) == plain

    print(timeit.timeit(lambda: test_case(100, 1000), number=1))

def increase_block_count(encryptor, count):
    encryptor(b"a"*16*((count+256)%256))

def attack_test():
    key = AESCounterMode.generate_key()
    nonce = AESCounterMode.generate_nonce()
    encrypt = AESCounterMode(key, nonce)
    decrypt = AESCounterMode(key, nonce)

    payload = b"123456789abcdef"
    a = encrypt(payload)
    increase_block_count(encrypt, -1)
    b = encrypt(a)
    print(a)
    print(b)
    print(a == b)


if __name__ == "__main__":
    attack_test()
    exit()
    test()
