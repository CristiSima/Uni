# kernel
Folosind veriunea 6.1 nu puteam da input din qemu asa ca am trecut la 6.6.
Setari aditionale:
- USB_SERIAL_FTDI_SIO: y
- BLUETOTH: n
    kernel-ul dadea eroare la boot legata de bluetoth


# Root FS
Facut cu buildroot.
Avahi nu functioneaza, porneste dar domain-ul nu este cunoscut nici de pe host nici din qemu. Nu afecteaza rezolvarea deoarece conexiunea se face prin portforwarding.
User-ul student adaugat cu usertable.
Pentru openssh am utilizat un script post-build, patch_sshd.sh, ce adauga "PermitRootLogin yes" in sshd_config.
Pentru server-ul web si driver-ul UART am instalat python si modulele Flask si pyserial.
In overlay sunt definite:
- servicile pentru web si driver in /etc/init.d/
- scriptul de achiziti in /bin
- fisierele pentru web in /opt/webserver
Pentru crearea unei imagini similare cu cea oficiala creez un rootfs.img in output/images cu 2 partiti, scriptul post image update_sd extrage continutul arhivei pe imaginea montata.
Pentru initializarea imagini:
dd if=/dev/zero of=output/images/rootfs.img bs=1M  count=64
parted output/images/rootfs.img
# !! in parted !!
mklabel msdos
mkpart primary fat32 1MiB 5MiB
mkpart primary ext4 5MiB 100%
p
q
#



# Daemon-ul de achiziție a datelor
Implementat in python cu pyserial.
Mentine starea curenta intr-o variabila globala si mentine cea mai actuala stare in /var/run/sensor_data sub format json.
Starile eronate sunt ignorate iar daca nu a esistat o stare valida se stockeaza null in loc de [<timestamp_last_update>, <value>]
Scriptul este pus in /bin si configurat cu shebang.
Serviciul pune log-urile in /var/log/sensor_service.log


# Web UI
Implementat in python cu flask si bootstrap.
La fiecare randare se citesc datele din fisier, iar pentru actialitate pagina face frecvent refresh
Toate fisierele sunt in /opt/webserver
Serviciul pune log-urile in /var/log/webserver.log 

