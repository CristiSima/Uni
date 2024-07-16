from mongoengine import Document, EmbeddedDocument, disconnect, connect
from mongoengine.fields import StringField, FloatField, ReferenceField, DateTimeField, SequenceField
from datetime import datetime, timezone
from os import environ
from functools import partial

from mongoengine import fields

__all__=[
    "connect",
    "disconnect",

    "Tari",
    "Orase",
    "Temperaturi",
]

db_name="sprc_t2"
connect = partial(connect,
    db_name,
    username=environ.get("MONGO_USER", None),
    password=environ.get("MONGO_PASSWORD", None),
    host=environ.get("MONGO_HOST", "192.168.0.210"),
    port=environ.get("MONGO_PORT", 27017)
)

disconnect = partial(disconnect, db_name)

class Tari(Document):
    id = SequenceField(primary_key=True)
    nume_tara = StringField(required=True, unique=True)

    # normaly GeoPointField would be used but the spec wants diferent fields
    # locatie = fields.GeoPointField()
    latitudine = FloatField(required=True)
    longitudine = FloatField(required=True)

    def toJSON(self):
        return {
            "id": self.id,
            "nume": self.nume_tara,
            "lat": self.latitudine,
            "lon": self.longitudine
        }

class Orase(Document):
    id = SequenceField(primary_key=True)
    tara = ReferenceField(Tari, db_field="id_tara", required=True)
    nume_oras = StringField(required=True, unique_with=["tara"])

    latitudine = FloatField(required=True)
    longitudine = FloatField(required=True)

    def toJSON(self):
        return {
            "id": self.id,
            "idTara": self.tara.id,
            "nume": self.nume_oras,
            "lat": self.latitudine,
            "lon": self.longitudine
        }

class Temperaturi(Document):
    id = SequenceField(primary_key=True)
    oras = ReferenceField(Orase, db_field="id_oras", required=True)
    timestamp = DateTimeField(required=True, default=partial(datetime.now, timezone.utc), unique_with=["oras"])

    valoare = FloatField(required=True)
    def toJSON(self):
        return {
            "id": self.id,
            "valoare": self.valoare,
            "timestamp": self.timestamp.__format__("%Y-%m-%d")
        }
