Pentru deploy este necesar pornirea containerelor si rularea scriptului de init din mounted
porneste compose stack-ul
    sudo docker-compose up --build

pentru obtinerea unuei sesiuni
    sudo docker exec -it bd2_database_1 sqlplus system/sys
    * numele containerului difera in functie de numele serviciunui care este cel al directorului curent
    * bd2 trebuie schimbat cu numele directorului curent
pentru rulare
    @mounted/init

sau dintr-o singura comanda
    sudo docker exec -it bd2_database_1 bash -c "echo @mounted/init | sqlplus system/sys"
    * cu aceiasi mentiune ca nueme trebuie schimbat