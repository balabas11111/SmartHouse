DELETE FROM entity_field WHERE id = 1239;

SELECT * FROM entity_field ef 
JOIN entities e ON e.id = ef.entity_id
JOIN groups g ON g.id = e.group_id
JOIN devices d ON d.id = g.device_id
WHERE ef.id = 1239;