-- @mounted/check_results_sets_error

DROP VIEW Results_Sets_Consistency_Errors;

CREATE VIEW Results_Sets_Consistency_Errors AS SELECT
    rs.recipe_id ID,
    recip.name Recipe,
    recip.description description,
    station.name Made_at,
    sum(rs.probability) probability_sum
FROM 
    Results_sets rs
    INNER JOIN Recipes recip ON rs.recipe_id = recip.id
    INNER JOIN Crafting_Stations station ON station.id = recip.crafting_station_id
GROUP BY recip.name, recip.description, rs.recipe_id, station.name
HAVING sum(rs.probability) != 1;


DECLARE
    consistency_errors NUMBER;
    rc sys_refcursor;
BEGIN
    SELECT count(Recipe) INTO consistency_errors FROM Results_Sets_Consistency_Errors;

    IF consistency_errors != 0 THEN
        dbms_output.Put_line('Results_Sets has '||consistency_errors||' set(s) with incorect probability distribution(s)');
        OPEN rc FOR SELECT * FROM Results_Sets_Consistency_Errors;
        dbms_sql.return_result(rc);
    END IF;
END;
/

CREATE OR REPLACE FUNCTION Results_Sets_Consistency_check RETURN sys_refcursor
IS
    rc sys_refcursor;
BEGIN
    OPEN rc FOR SELECT * FROM Results_Sets_Consistency_Errors;
    return rc;
END Results_Sets_Consistency_check;
/
show errors;
