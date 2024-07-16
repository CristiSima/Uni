FROM python:3.11

COPY requierments.txt /worky/
WORKDIR /worky

RUN pip install -r requierments.txt 

COPY . .

CMD python app.py