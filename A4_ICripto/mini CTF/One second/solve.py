import requests
import string
import itertools
from tqdm import tqdm
import time

url="http://141.85.224.119:7777/"

s=requests.session()

def get_length() -> int:
    for i in range(512+3):
        r = s.post(url, json={
            "password":"1"*i
        })
        if r.content == b"Wrong password":
            return i

length = get_length()
print(length)

def request(password):
    return s.post(url, json={
        "password": password
    })

def time_attack(before, after):
    times = {}
    for password in string.ascii_letters+string.digits:
        t=time.time_ns()
        r = request(before + password + after)
        times[password] = time.time_ns()-t

    max_time=max(e for _,e in times.items())
    # print(times)
    for letter, letter_time in times.items():
        if max_time == letter_time:
            return letter


password = ""
for _ in range(length-1-len(password)):
    password+=time_attack(password, "!"*(length-1-len(password)))
    print(password)

for password_c in string.ascii_letters+string.digits:
    r = request(password+password_c)
    if r.content == b"Wrong password":
        continue
    print(password+password_c)
    print(r.content.decode())

# gmABy
# CRYPTO_CTF{Cesiu133InRepaus}