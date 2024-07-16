from Crypto.PublicKey import RSA
import os
import requests
os.chdir(os.path.dirname(os.path.abspath(__file__)))

key=RSA.importKey(open("pubkey.pem").read())
# print(key.public_key)
public_numbers=key

def int_to_bytes(x: int) -> bytes:
    return x.to_bytes((x.bit_length() + 7) // 8, 'big')

def bytes_to_int(x: bytes) -> int:
    return int(x.hex(), 16)

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

SQL_payload="' OR 1='1"
print("Full Quarry:", f'SELECT username, pass FROM users WHERE username=\'{"username"}\' AND pass=\'{SQL_payload}\'' )

def is_signable(p):
    return (b'`' not in p) and (b'\'' not in p) and (b'\"' not in p) and (b'-' not in p) and (b';' not in p) and (b'%' not in p)
    
def sign(payload):
    return requests.post("http://141.85.224.119:5001/sign", json={
        "payload": payload
    }).json()


# print(sign("11"))


offset=0x12
payload = int_to_bytes(bytes_to_int(SQL_payload.encode()) * offset)
print(payload)
print(is_signable(payload))

# print(payload.hex())
signed_payload = int(sign(payload.hex())["signature"], 16)
signed_offet = int(sign(hex(offset)[2:])["signature"], 16)
print(signed_payload)
print(signed_offet)

signed_offet_inv = pow(signed_offet, -1, key.n)
print(signed_offet_inv)

signed_SQL_payload = int_to_bytes((signed_payload * signed_offet_inv) % key.n).hex()
print("SQL paylaod", signed_SQL_payload)

username="admin"
print(sign(username.encode().hex()))
print(requests.post("http://141.85.224.119:5000/login",
    json={
        "username": username,
        "username_sign": sign(username.encode().hex())["signature"],
        "password": SQL_payload,
        "password_sign": signed_SQL_payload
}).content.split(b"Congratulations! You got the flag: ")[1].split(b"\n")[0])
# CRYPTO_CTF{73x7b00k_r54_n07_600d_f0r_54n171z4710n}
