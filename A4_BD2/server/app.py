from flask import Flask, render_template, request, Blueprint
import oracledb
import time

while True:
    try:
        connection = oracledb.connect(
            user="system",
            password="sys",
            host="database"
        )
    except Exception as e:
        print("DB Connection failed with", e, flush=True)
        time.sleep(5)
        continue
    break

app = Flask(__name__)

@app.context_processor
def renderers():
    return {
        "round": round,
        "zip": zip,
        "list": list,
        "enumerate": enumerate,
        "print": print,
        "len": len,
    }

@app.get("/rs_consistency_check")
def rs_consistency_check():
    with connection.cursor() as cur:
        return render_template("consistency.html.j2",
            rows=list(cur.callfunc("Results_Sets_Consistency_check", oracledb.DB_TYPE_CURSOR))
        )

@app.get("/cycles_in_recipes")
def get_cycles_in_recipes():
    with connection.cursor() as cur:
        return render_template("cycles_in_recipes.html.j2",
            rows=list(cur.callfunc("get_cycles_in_recipes", oracledb.DB_TYPE_CURSOR))
        )


@app.get("/profitable_recipes")
def get_profitable_recipes():
    with connection.cursor() as cur:
        return render_template("profitable_recipes.html.j2", rows=cur.callfunc("get_profitable_recipes", oracledb.DB_TYPE_CURSOR))


@app.get("/items")
def get_items():
    with connection.cursor() as cur:
        return render_template("items.html.j2", rows=cur.callfunc("Items_get_all", oracledb.DB_TYPE_CURSOR))

@app.get("/stations")
def get_stations():
    with connection.cursor() as cur:
        return render_template("stations.html.j2", rows=cur.callfunc("Stations_get_all", oracledb.DB_TYPE_CURSOR))

@app.get("/item/<int:item_id>")
def iteminfo(item_id):
    with connection.cursor() as cur:
        items = list(cur.callfunc("Items_get_by_id", oracledb.DB_TYPE_CURSOR, [item_id,]))
        if len(items) != 1:
            return "", 404
        item=items[0]
        return render_template("item.html.j2",
            item=item,
            made_by_rows=cur.callfunc("get_recipes_for_item", oracledb.DB_TYPE_CURSOR, [item_id,]),
            used_in_rows=cur.callfunc("get_recipes_using_item", oracledb.DB_TYPE_CURSOR, [item_id,]),
        )


@app.get("/recipe/<int:recipe_id>")
def recipeinfo(recipe_id):
    with connection.cursor() as cur:
        recipes=list(cur.callfunc("Recipes_get_by_id", oracledb.DB_TYPE_CURSOR, [recipe_id,]))
        if len(recipes) != 1:
            return "", 404
        recipe=recipes[0]

        return render_template("recipe.html.j2",
            recipe=recipe,
            ingredients=cur.callfunc("get_ingredients_for_recipe", oracledb.DB_TYPE_CURSOR, [recipe_id,]),
            result_sets=[
                (
                    result_set_probability,
                    cur.callfunc("get_results_in_set", oracledb.DB_TYPE_CURSOR, [result_set_id,])
                )
                for result_set_id, result_set_probability
                    in cur.callfunc("get_results_sets_for_recipe", oracledb.DB_TYPE_CURSOR, [recipe_id,])
            ]
        )


@app.post("/item_price/<int:item_id>")
def set_item_price(item_id):
    print(item_id, request.data.decode(), flush=True)
    try:
        new_price = int(request.data.decode())
    except:
        return "Wrong Format", 400

    with connection.cursor() as cur:
        cur.callfunc("Items_set_price", bool, [item_id, new_price])

    return ""

@app.post("/station_tax/<int:station_id>")
def set_station_tax(station_id):
    print(station_id, request.data.decode(), flush=True)
    try:
        new_tax = int(request.data.decode())
    except:
        return "Wrong Format", 400

    with connection.cursor() as cur:
        cur.callfunc("Stations_set_tax", bool, [station_id, new_tax])

    return ""


@app.get("/")
def test():
    return render_template("home.html.j2")

if __name__ == '__main__':
    app.run(host="0.0.0.0", debug=True)