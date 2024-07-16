from Crypto.PublicKey import RSA
import os
os.chdir(os.path.dirname(os.path.abspath(__file__)))

key=RSA.importKey(open("private.pem").read())

def xgcd(a, b):
    """
    Performs the extended Euclidean algorithm
    Returns the gcd, coefficient of a, and coefficient of b
    """
    x, old_x = 0, 1
    y, old_y = 1, 0

    while (b != 0):
        quotient = a // b
        a, b = b, a - quotient * b
        old_x, x = x, old_x - quotient * x
        old_y, y = y, old_y - quotient * y

    return a, old_x, old_y


def RSB_dec(ciphertext, key):
    totient = (key.p - 1) * (key.q - 1)
    rsb_e = key.e*101

    _, rsb_d, _ = xgcd(rsb_e, totient)
    rsb_d = (rsb_d + totient) % totient

    return pow(ciphertext, rsb_d, key.n)

print(RSB_dec(int(open("cipher.txt").read()), key).to_bytes(100, "big").strip(b"\x00").decode())
# CRYPTO_CTF{Ce_mare_lucru_ba_o_ecuatie?Faci_18lasuta_dinXegalcuY}