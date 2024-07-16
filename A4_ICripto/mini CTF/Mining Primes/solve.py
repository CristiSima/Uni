from Crypto.PublicKey import RSA
from Crypto.Cipher import PKCS1_OAEP
from Crypto.Util import number
import base64
import os
import math
os.chdir(os.path.dirname(os.path.abspath(__file__)))

alice_key=RSA.importKey(open("alice.pubkey").read())
bob_key=RSA.importKey(open("bob.pubkey").read())

print(alice_key.public_key)
print(bob_key.public_key)

def make_private(key: RSA.RsaKey, p: int):
    q = key.n // p
    totient = math.lcm(p-1, q-1)

    key._p = p
    key._q = q
    key._d = pow(key.e, -1, totient)

    key._dp = key._d % (key._p - 1)  # = (e⁻¹) mod (p-1)
    key._dq = key._d % (key._q - 1)  # = (e⁻¹) mod (q-1)
    key._invq = None                   # will be computed on demand

    key._u = pow(p, -1, q)

p=(math.gcd(alice_key.n, bob_key.n))

make_private(alice_key, p)
make_private(bob_key, p)


alice_decryptor = PKCS1_OAEP.new(alice_key)
bob_decryptor = PKCS1_OAEP.new(bob_key)

print(alice_decryptor.decrypt(base64.b64decode(open("for_alice.enc").read()))
        + bob_decryptor.decrypt(base64.b64decode(open("for_bob.enc").read())))

        
# CRYPTO_CTF{Ho ho ho! And a bottle of gcd!}