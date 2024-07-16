

CREATE OR REPLACE FUNCTION Items_set_price(target_id number, new_price number) RETURN boolean
IS
BEGIN
    UPDATE Items
    SET average_price=new_price
    WHERE id = target_id;

    return sql%rowcount = 1;
END Items_set_price;
/
show errors;

-- python driver doesn't have row type
CREATE OR REPLACE FUNCTION Items_get_by_id(target_id number) RETURN sys_refcursor
IS
    rc sys_refcursor;
BEGIN
    -- SELECT * INTO rc FROM Items WHERE id = target_id;
    OPEN rc FOR SELECT * FROM Items WHERE id = target_id;
    return rc;
END Items_get_by_id;
/
show errors;

CREATE OR REPLACE FUNCTION Items_get_all RETURN sys_refcursor
IS
    rc sys_refcursor;
BEGIN
    OPEN rc FOR SELECT * FROM Items;
    return rc;
END Items_get_all;
/
show errors;


CREATE OR REPLACE FUNCTION Stations_set_tax(target_id number, new_tax number) RETURN boolean
IS
BEGIN
    UPDATE Crafting_Stations
    SET use_tax=new_tax
    WHERE id = target_id;

    return sql%rowcount = 1;
END Stations_set_tax;
/
show errors;

CREATE OR REPLACE FUNCTION Stations_get_by_id(target_id number) RETURN sys_refcursor
IS
    rc sys_refcursor;
    -- rc Crafting_Stations%ROWTYPE;
BEGIN
    -- SELECT * INTO rc FROM Crafting_Stations WHERE id = target_id;
    OPEN rc FOR SELECT * FROM Crafting_Stations WHERE id = target_id;
    return rc;
END Stations_get_by_id;
/
show errors;

CREATE OR REPLACE FUNCTION Stations_get_all RETURN sys_refcursor
IS
    rc sys_refcursor;
BEGIN
    OPEN rc FOR SELECT * FROM Crafting_Stations;
    return rc;
END Stations_get_all;
/
show errors;



CREATE OR REPLACE FUNCTION get_results_sets_for_recipe(target_id number) RETURN sys_refcursor
IS
    rc sys_refcursor;
    -- rc Crafting_Stations%ROWTYPE;
BEGIN
    OPEN rc FOR SELECT
        rs.id, rs.probability
    FROM 
        Results_sets rs
    WHERE rs.recipe_id = target_id;

    return rc;
END get_results_sets_for_recipe;
/
show errors;

CREATE OR REPLACE FUNCTION get_results_in_set(target_id number) RETURN sys_refcursor
IS
    rc sys_refcursor;
    -- rc Crafting_Stations%ROWTYPE;
BEGIN
    OPEN rc FOR SELECT
        item.id, item.name, item.tier, item.enchant, item.quality, r.count
    FROM 
        Results r
        INNER JOIN Items item ON item.id = r.item_id
    WHERE r.results_sets_id = target_id;

    return rc;
END get_results_in_set;
/
show errors;

CREATE OR REPLACE FUNCTION get_ingredients_for_recipe(target_id number) RETURN sys_refcursor
IS
    rc sys_refcursor;
    -- rc Crafting_Stations%ROWTYPE;
BEGIN
    OPEN rc FOR SELECT
        item.id, item.name, item.tier, item.enchant, item.quality, ing.count
    FROM 
        Ingredients ing
        INNER JOIN Items item ON item.id = ing.item_id
    WHERE ing.recipe_id = target_id;

    return rc;
END get_ingredients_for_recipe;
/
show errors;


CREATE OR REPLACE FUNCTION Recipes_get_by_id(target_id number) RETURN sys_refcursor
IS
    rc sys_refcursor;
    -- rc Crafting_Stations%ROWTYPE;
BEGIN
    OPEN rc FOR SELECT
        r.id, r.name, r.description, cs.name, cs.use_tax
    FROM 
        Recipes r
        INNER JOIN Crafting_Stations cs ON cs.id = r.crafting_station_id
    WHERE r.id = target_id;

    return rc;
END Recipes_get_by_id;
/
show errors;

CREATE OR REPLACE FUNCTION Recipes_get_all RETURN sys_refcursor
IS
    rc sys_refcursor;
BEGIN
    OPEN rc FOR SELECT
        r.id, r.name, r.description, cs.name, cs.use_tax
    FROM 
        Recipes r
        INNER JOIN Crafting_Stations cs ON cs.id = r.crafting_station_id;

    return rc;
END Recipes_get_all;
/
show errors;

-- @mounted/interaction
