-- @mounted/get_cycles_in_recipes

CREATE OR REPLACE FUNCTION get_cycles_in_recipes RETURN sys_refcursor
IS
    rc sys_refcursor;
BEGIN
    OPEN rc FOR SELECT
        item.id item_id,
        item.name item_name,
        recip.id recipe_id,
        recip.name recipe_name,
        sum(ing.count)/count(item.id) consumed,
        -- is div for every entry is res
        sum(res.count*rs.probability) produced,
        sum(res.count*rs.probability) - sum(ing.count)/count(item.id) Delta
    FROM
        Recipes recip
        INNER JOIN Crafting_Stations station ON station.id = recip.crafting_station_id
        INNER JOIN Ingredients ing ON ing.recipe_id = recip.id
        INNER JOIN Items item ON item.id = ing.item_id
        INNER JOIN Results_sets rs ON rs.recipe_id = recip.id
        INNER JOIN Results res ON res.results_sets_id = rs.id
    WHERE res.item_id = ing.item_id
    GROUP BY item.id, item.name, recip.id, recip.name;

    return rc;
END get_cycles_in_recipes;
/
show errors;

