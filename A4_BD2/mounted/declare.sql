-- // Use DBML to define your database structure
-- // Docs: https://dbml.dbdiagram.io/docs

DROP TABLE Ingredients;
DROP TABLE Results;
DROP TABLE Results_sets;
DROP TABLE Items;
DROP TABLE Recipes;
DROP TABLE Crafting_Stations;


CREATE TABLE Crafting_Stations (
      id NUMBER(4) PRIMARY KEY,
      name VARCHAR2(50) NOT NULL,
      use_tax NUMBER(9) NOT NULL
);
CREATE TABLE Recipes (
    id NUMBER(4) PRIMARY KEY,
    crafting_station_id NUMBER(4) NOT NULL,
    name VARCHAR2(50) NOT NULL,
    description VARCHAR2(200),
    CONSTRAINT FK_Recipes_crafting_station FOREIGN KEY(crafting_station_id) REFERENCES Crafting_Stations(id) ON DELETE CASCADE
);

CREATE TABLE Items (
    id NUMBER(4) PRIMARY KEY,
    name VARCHAR2(50) NOT NULL,
    tier NUMBER(1) NOT NULL,
    enchant NUMBER(1) NOT NULL,
    quality NUMBER(1) NOT NULL,
    average_price NUMBER(9) NOT NULL
);


CREATE TABLE Ingredients (
    recipe_id NUMBER(4) NOT NULL,
    item_id NUMBER(4) NOT NULL,
    count NUMBER(3) NOT NULL,
    CONSTRAINT PK_Ingredients PRIMARY KEY(recipe_id, item_id),  
    CONSTRAINT FK_Ingredients_item FOREIGN KEY(item_id) REFERENCES Items(id) ON DELETE CASCADE,
    CONSTRAINT FK_Ingredients_recipe FOREIGN KEY(recipe_id) REFERENCES Recipes(id) ON DELETE CASCADE
);
-- Ref: Ingredients.item_id > Items.id // many-to-one

CREATE TABLE Results_sets (
    id NUMBER(4) PRIMARY KEY,
    recipe_id NUMBER(4) NOT NULL,
    probability NUMBER(4,3) NOT NULL,
    CONSTRAINT FK_Results_sets_recipe FOREIGN KEY(recipe_id) REFERENCES Recipes(id) ON DELETE CASCADE
);
-- CONSTRAINT check_sum_constraint 
-- 	CHECK (probability + COALESCE((SELECT SUM(probability) FROM Results_sets r WHERE r.recipe_id = recipe_id), 0) = 1)

CREATE TABLE Results (
    results_sets_id NUMBER(4) NOT NULL,
    item_id NUMBER(4) NOT NULL,
    count NUMBER(3) NOT NULL,
	CONSTRAINT PK_Results PRIMARY KEY(results_sets_id, item_id),  
    CONSTRAINT FK_Results_item FOREIGN KEY(item_id) REFERENCES Items(id) ON DELETE CASCADE,
    CONSTRAINT FK_Results_results_sets FOREIGN KEY(results_sets_id) REFERENCES Results_Sets(id) ON DELETE CASCADE
);
-- Ref: Results.item_id > Items.id // many-to-one







-- SELECT TABLE_NAME FROM ALL_TABLES WHERE  TABLESPACE_NAME = 'USERS';
-- SELECT * FROM ALL_TABLES WHERE  TABLE_NAME = 'ITEMS';