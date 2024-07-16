from flask import Flask, request, Blueprint
from documents import *


def get_error(error_string):
    return {
        "error": error_string
    }

app = Flask(__name__)

countries = Blueprint("Counteries API", "countries_api", url_prefix="/api/countries")
cities = Blueprint("Cities API", "cities_api", url_prefix="/api/cities")
temperatures = Blueprint("Temperatures API", "temperatures_api", url_prefix="/api/temperatures")

@countries.post("")
def add_countrie():
    if not request.is_json or any([field not in request.json for field in ["nume", "lat", "lon"]]):
        return get_error("Bad Request Format"), 400

    countrie = Tari (
        nume_tara = request.json["nume"],
        latitudine = request.json["lat"],
        longitudine = request.json["lon"],
    )

    try:
        countrie.save()
        return {
            "id": countrie.id
        }, 201
    except:
        return get_error("Country Duplicate"), 409

@countries.get("")
def get_countries():
    return list(map(Tari.toJSON, Tari.objects))

@countries.put("<int:id_tara>")
def edit_countrie(id_tara: int):
    if not request.is_json or any([field not in request.json for field in ["nume", "lat", "lon"]]):
        return get_error("Bad Request Format"), 400

    countrie = list(Tari.objects(id=id_tara))
    if not len(countrie):
        return get_error("Country not found"), 404
    countrie = countrie[0]

    countrie.nume_tara = request.json["nume"]
    countrie.latitudine = request.json["lat"]
    countrie.longitudine = request.json["lon"]

    try:
        countrie.save()
    except:
        return get_error("Country Duplicate"), 409

    return "", 200

@countries.delete("<int:id_tara>")
def delete_countrie(id_tara: int):
    countrie = list(Tari.objects(id=id_tara))
    if not len(countrie):
        return get_error("Country not found"), 404
    countrie = countrie[0]

    countrie.delete()
    return ""

@cities.post("")
def add_city():
    if not request.is_json or any([field not in request.json for field in ["idTara", "nume", "lat", "lon"]]):
        return get_error("Bad Request Format"), 400

    countrie = list(Tari.objects(id=request.json["idTara"]))
    if not len(countrie):
        return get_error("Country not found"), 404
    countrie=countrie[0]

    city = Orase(
        tara=countrie,
        nume_oras=request.json["nume"],
        latitudine=request.json["lat"],
        longitudine=request.json["lon"]
    )
    
    try:
        city.save()
        return {
            "id": city.id
        }, 201
    except:
        return get_error("City Duplicate"), 409

@cities.get("")
def get_cities():
    return list(map(Orase.toJSON, Orase.objects))

@cities.get("/country/<int:id_tara>")
@cities.get("/country/")
def get_cities_from(id_tara=0):
    return list(map(Orase.toJSON, Orase.objects(tara=id_tara)))

@cities.put("/<int:id_oras>")
def edit_city(id_oras):
    if not request.is_json or any([field not in request.json for field in ["idTara", "nume", "lat", "lon"]]):
        return get_error("Bad Request Format"), 400

    city = list(Orase.objects(id=id_oras))
    if not len(city):
        return get_error("City not found"), 404
    city = city[0]

    print(request.json["idTara"], type(request.json["idTara"]))
    countrie = Tari.objects(id=request.json["idTara"])
    if not len(countrie):
        return get_error("Country not found"), 404
    countrie=countrie[0]

    city.tara = countrie
    city.nume_oras = request.json["nume"]
    city.latitudine = request.json["lat"]
    city.longitudine = request.json["lon"]

    try:
        print("save", city.toJSON(), city.save())
    except Exception as e:
        print("Error", e)
        return get_error("City Duplicate"), 409

    return "", 200
    

@cities.delete("/<int:id_oras>")
def delete_city(id_oras):
    city = list(Orase.objects(id=id_oras))
    if not len(city):
        return get_error("City not found"), 404
    city = city[0]

    city.delete()

    return "", 200


@temperatures.post("")
def add_temperature():
    if not request.is_json or any([field not in request.json for field in ["idOras", "valoare"]]):
        return get_error("Bad Request Format"), 400

    oras = list(Orase.objects(id=request.json["idOras"]))
    if not len(oras):
        return get_error("City not found"), 404
    oras=oras[0]

    entry = Temperaturi(
        oras = oras,
        valoare= request.json["valoare"]
    )

    try:
        entry.save()
        return {
            "id": entry.id
        }, 201
    except:
        return get_error("Duplicate entry"), 409

@temperatures.get("")
def get_temperatures():
    orase = Orase.objects(**{
        **({"latitudine": request.args["lat"]} if "lat" in request.args else {}),
        **({"longitudine": request.args["lon"]} if "lon" in request.args else {}),
    })

    return list(map(Temperaturi.toJSON, Temperaturi.objects(**{
        "oras__in": orase,
        **({"timestamp__gt": request.args["from"]} if "from" in request.args else {}),
        **({"timestamp__lt": request.args["until"]} if "until" in request.args else {}),
    })))

@temperatures.get("/cities/<int:id_oras>")
def get_city_temperatures(id_oras):
    city = list(Orase.objects(id=id_oras))
    if not len(city):
        return []
    city=city[0]

    return list(map(Temperaturi.toJSON, Temperaturi.objects(**{
        "oras": city,
        **({"timestamp__gt": request.args["from"]} if "from" in request.args else {}),
        **({"timestamp__lt": request.args["until"]} if "until" in request.args else {}),
    })))

@temperatures.get("/countries/<int:id_oras>")
def get_country_temperatures(id_oras):
    country = list(Tari.objects(id=id_oras))
    if not len(country):
        return []
    country=country[0]

    orase = Orase.objects(tara=country)

    return list(map(Temperaturi.toJSON, Temperaturi.objects(**{
        "oras__in": orase,
        **({"timestamp__gt": request.args["from"]} if "from" in request.args else {}),
        **({"timestamp__lt": request.args["until"]} if "until" in request.args else {}),
    })))

@temperatures.put("/<int:temperature_id>")
def modify_temperature(temperature_id):
    if not request.is_json or any([field not in request.json for field in ["idOras", "valoare"]]):
        return get_error("Bad Request Format"), 400

    temperature = list(Temperaturi.objects(id=temperature_id))
    if not len(temperature):
        return ("Entry not found"), 404
    temperature = temperature[0]

    oras = list(Orase.objects(id=request.json["idOras"]))
    if not len(oras):
        return get_error("City not found"), 400
    oras=oras[0]

    temperature.oras = oras
    temperature.valoare = request.json["valoare"]

    try:
        temperature.save()
    except:
        return get_error("Entry duplicate"), 409

    return "", 200


@temperatures.delete("/<int:temperature_id>")
def delete_temperature(temperature_id):
    temperature = list(Temperaturi.objects(id=temperature_id))
    if not len(temperature):
        return get_error("Entry not found"), 404
    temperature = temperature[0]

    temperature.delete()
    return "", 200


app.register_blueprint(countries)
app.register_blueprint(cities)
app.register_blueprint(temperatures)

if __name__ == '__main__':
    connect()
    app.run(host="0.0.0.0", debug=True)