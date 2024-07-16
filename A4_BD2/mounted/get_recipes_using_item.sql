


CREATE OR REPLACE FUNCTION get_recipes_using_item(target_item_id number) RETURN sys_refcursor
IS
    rc sys_refcursor;
BEGIN
    OPEN rc FOR SELECT recip.id, recip.name, recip.description, sum(ing.count)
    FROM
        Recipes recip
        INNER JOIN Ingredients ing ON ing.recipe_id = recip.id
    WHERE ing.item_id = target_item_id
    GROUP BY recip.id, recip.name, recip.description;
    -- HAVING res.item_id = target_item_id

    return rc;
END get_recipes_using_item;
/
show errors;

DECLARE
    rc sys_refcursor;
BEGIN
    rc := get_recipes_using_item(44);
    -- dbms_sql.return_result(rc);
END;
/
show errors;

-- @mounted/get_recipes_using_item
