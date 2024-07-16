## RSB  
<b>c = m<sup>(e*101)</sup></b>  
<b>e<sub>RSB</sub> = e\*101</b>  
<b>c = m<sup>e<sub>RSB</sub></sup></b>  
Se calculeaza
<b>d<sub>RSB</sub></b>,
ca inversul modular lui
<b>e<sub>RSB</sub></b>  
<b>c<sup>d<sub>RSB</sub></sup> = m<sup>e<sub>RSB</sub>*d<sub>RSB</sub></sup> = m</b>  
`CRYPTO_CTF{Ce_mare_lucru_ba_o_ecuatie?Faci_18lasuta_dinXegalcuY}`

## Mining Primes
Cheile au un prim comun care se poate extrage prin cmmdc.  
Odata cunoscut re poate calcula celalalt si rederiva cheia privata si decripta.  
`CRYPTO_CTF{Ho ho ho! And a bottle of gcd!}`

## One second
Verificarea cheii se face caracter cu caracter si este suficient de lenta pentru a se putea afla caracter cu caracter.  
`CRYPTO_CTF{Cesiu133InRepaus}`

## 123
Counter-ul folosit este pe 8 biti si va genera acelasi keystream la fiecare 256 blocuri.  
Criptarea este xor-ul mesajului cu keystream-ul, care se poate afla la criparea cu un mesaj cunoscut.  
Odata aflat keystream-ul se poate decripta flag-ul.  
`CRYPTO-CTF{Ch4n93 th3 k3y m0r3 0ft3n!}`

## Caramelia
Autentificarea este vunerabila unui SQL injection, dar username-ul si parola trebuie semnate.
Dar servarul nu va semna mesaje care contin ``'"`-%;``.  
Semnarea consta in o cripare simpla cu RSA  
<b>password<sub>signed</sub>c = password<sup>d</sup></b>  
Iar verificare este decriptarea si compararea cu valoarea primita  
<b>password == password<sub>signed</sub><sup>e</sup></b>  
Se poate folosi
<b>a<sup>c</sup> = (a\*b)<sup>c</sup> * inv(b<sup>c</sup>)</b>
pentru a semna orice mesaj  
<b>password<sub>signed</sub> = password<sup>d</sup> = (password\*v)<sup>d</sup> * inv(v<sup>d</sup>)</b>  
Astfel se pot semna mesaje cu caractere invalide  
`CRYPTO_CTF{73x7b00k_r54_n07_600d_f0r_54n171z4710n}`