SELECT * FROM entity_field_value efv
JOIN entity_field ef ON efv.entity_field_id = ef.id 
JOIN entities e ON e.id = ef.entity_id
JOIN groups g ON g.id = e.group_id
JOIN devices d ON d.id = g.device_id
WHERE d.id = 29
ORDER BY efv.date desc;