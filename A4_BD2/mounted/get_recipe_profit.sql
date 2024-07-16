-- @mounted/get_recipe_profit

-- column ID format 999
-- column TIER format 9
-- column enchant format 9
-- column quality format 9
-- column count format 99
-- column NAME format a20
-- column DESCRIPTION format a30


CREATE OR REPLACE FUNCTION get_profitable_recipes RETURN sys_refcursor
IS
    rc sys_refcursor;
BEGIN
    OPEN rc FOR SELECT recip.id, recip.name, recip.description, station.name, station.use_tax,
        sum(ing.count*item_ing.average_price) AS Meterials_Price,
        round(sum(res.count*item_res.average_price*rs.probability)) AS Average_Result_Value,
        round(sum(res.count*item_res.average_price*rs.probability) - sum(ing.count*item_ing.average_price) - station.use_tax) AS Average_Profit
    FROM
        Recipes recip
        INNER JOIN Crafting_Stations station ON station.id = recip.crafting_station_id
        INNER JOIN Ingredients ing ON ing.recipe_id = recip.id
        INNER JOIN Items item_ing ON item_ing.id = ing.item_id
        INNER JOIN Results_sets rs ON rs.recipe_id = recip.id
        INNER JOIN Results res ON res.results_sets_id = rs.id
        INNER JOIN Items item_res ON item_res.id = res.item_id
    GROUP BY recip.id, recip.name, recip.description, station.name, station.use_tax
    HAVING round(sum(res.count*item_res.average_price*rs.probability) - sum(ing.count*item_ing.average_price) - station.use_tax) > 0
    ORDER BY 8 desc;

    return rc;
END get_profitable_recipes;
/
show errors;

DECLARE
    rc sys_refcursor;
BEGIN
    rc := get_profitable_recipes();
    -- dbms_sql.return_result(rc);
END;
/
show errors;
