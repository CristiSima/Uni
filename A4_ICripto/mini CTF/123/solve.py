from math import ceil
from ctf_counter_mode_2023.client import Client

client = Client("141.85.224.119", 9776)

(client.readline())
client.print_menu()
print("#")
def get_flag():
    client.send(b"1")
    ret = client.readline()
    client.readline()
    return ret.strip()[len("Encrypted FLAG: "):].decode()

    
def encrypt(msg: bytes):
    client.send(b"2")
    client.send(msg)
    (client.readline())
    ret = client.readline()
    (client.readline())
    (ret)
    return ret.strip()[len("Encrypted plaintext: "):].decode()

def increase_block_count(encryptor, count):
    (encryptor(b"a"*16*((count+256)%256)))

flag = get_flag()
print(flag)
flag = bytes.fromhex(flag)
flag_blocs = ceil(len(flag) / 16)
print(flag_blocs)
# state reset
increase_block_count(encrypt, -flag_blocs)

A_xor_seq=bytes.fromhex(encrypt(b"A"*len(flag)))
# get stream_key
seq = bytes(a ^ ord("A") for a in A_xor_seq)
print(seq)

flag = bytes(a ^ b for a, b in zip(flag, seq))
print(flag.decode())
# CRYPTO-CTF{Ch4n93 th3 k3y m0r3 0ft3n!}