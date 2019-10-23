SELECT g.* FROM devices d
	JOIN groups g ON g.device_id = d.id
WHERE d.id = 1;

SELECT e.name, ef.* FROM entities e
JOIN entity_field ef ON ef.entity_id = e.id
WHERE e.group_id = 2;

SELECT e.name, ef.description, ef.name, efev.* FROM entities e
JOIN entity_field ef ON ef.entity_id = e.id
JOIN entity_field_enabled_value efev ON efev.entity_field_id = ef.id
WHERE e.group_id = 2;