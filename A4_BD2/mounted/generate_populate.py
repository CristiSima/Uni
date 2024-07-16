def sword_recipe(recipe_id, results_set_id, swrod_q0_id, resource_offset):
    return f'''
INSERT INTO Recipes VALUES({recipe_id}, 5, 'Smith Sword', null);
INSERT INTO Results_sets VALUES({results_set_id}, {recipe_id}, 0.5);
INSERT INTO Results VALUES({results_set_id}, {swrod_q0_id+0}, 1);
INSERT INTO Results_sets VALUES({results_set_id+1}, {recipe_id}, 0.3);
INSERT INTO Results VALUES({results_set_id+1}, {swrod_q0_id+1}, 1);
INSERT INTO Results_sets VALUES({results_set_id+2}, {recipe_id}, 0.1);
INSERT INTO Results VALUES({results_set_id+2}, {swrod_q0_id+2}, 1);
INSERT INTO Results_sets VALUES({results_set_id+3}, {recipe_id}, 0.08);
INSERT INTO Results VALUES({results_set_id+3}, {swrod_q0_id+3}, 1);
INSERT INTO Results_sets VALUES({results_set_id+4}, {recipe_id}, 0.02);
INSERT INTO Results VALUES({results_set_id+4}, {swrod_q0_id+4}, 1);
INSERT INTO Ingredients VALUES({recipe_id}, {20+resource_offset}, 1);
INSERT INTO Ingredients VALUES({recipe_id}, {40+resource_offset}, 1);
    '''


recipe_id=30
results_set_id=50
print(sword_recipe(recipe_id, results_set_id, 50, 0))
recipe_id+=1
results_set_id+=5
print(sword_recipe(recipe_id, results_set_id, 55, 1))
recipe_id+=1
results_set_id+=5
print(sword_recipe(recipe_id, results_set_id, 60, 2))
recipe_id+=1
results_set_id+=5
print(sword_recipe(recipe_id, results_set_id, 65, 3))
recipe_id+=1
results_set_id+=5
print(sword_recipe(recipe_id, results_set_id, 70, 4))
recipe_id+=1
results_set_id+=5
print(sword_recipe(recipe_id, results_set_id, 75, 5))