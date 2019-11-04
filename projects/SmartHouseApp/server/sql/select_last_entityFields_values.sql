SELECT efv.id, efv.date, efv.value, ef.name AS fieldName, ef.read_only,
	e.name AS entityName,
	e.id AS entityId,
	g.name AS groupName,
	d.name AS deviceName   FROM entity_field_value efv
JOIN (SELECT MAX(id) AS id FROM entity_field_value GROUP BY entity_field_id) lastEfv ON efv.id = lastEfv.id
JOIN entity_field ef ON efv.entity_field_id = ef.id
JOIN entities e ON ef.entity_id = e.id
JOIN groups g ON e.group_id = g.id
JOIN devices d ON g.device_id = d.id