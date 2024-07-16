


CREATE OR REPLACE FUNCTION get_recipes_for_item(target_item_id number) RETURN sys_refcursor
IS
    rc sys_refcursor;
BEGIN
    OPEN rc FOR SELECT
        recip.id, recip.name, recip.description,
        sum(res.count * rs.probability) average_crafted
    FROM
        Recipes recip
        INNER JOIN Results_sets rs ON rs.recipe_id = recip.id
        INNER JOIN Results res ON res.results_sets_id = rs.id
    WHERE res.item_id = target_item_id
    GROUP BY recip.id, recip.name, recip.description;

    return rc;
END get_recipes_for_item;
/
show errors;

DECLARE
    rc sys_refcursor;
BEGIN
    rc := get_recipes_for_item(44);
    -- dbms_sql.return_result(rc);
END;
/
show errors;

-- @mounted/get_recipes_for_item
