SET @deviceId=3;

DELETE FROM entity_field_value 
WHERE entity_field_id in
	(SELECT ef.id AS id from entity_field ef 
	JOIN entities e ON ef.entity_id = e.id
	JOIN groups g ON g.id = e.group_id
	JOIN devices d ON d.id = g.device_id
	WHERE d.id = @deviceId);
	
DELETE FROM entity_field_enabled_value
WHERE entity_field_id IN
	(SELECT ef.id AS id from entity_field ef 
	JOIN entities e ON ef.entity_id = e.id
	JOIN groups g ON g.id = e.group_id
	JOIN devices d ON d.id = g.device_id
	WHERE d.id = @deviceId);

DELETE FROM entity_groupped_fields_ids
WHERE entity_id IN
	(SELECT e.id AS id from entities e
	JOIN groups g ON g.id = e.group_id
	JOIN devices d ON d.id = g.device_id
	WHERE d.id = @deviceId);
	
DELETE FROM entity_groupped_fields_names
WHERE entity_id IN
	(SELECT e.id AS id from entities e
	JOIN groups g ON g.id = e.group_id
	JOIN devices d ON d.id = g.device_id
	WHERE d.id = @deviceId);
	
DELETE FROM entity_alarm_entity_field
WHERE entity_field_id IN
	(SELECT ef.id AS id from entity_field ef 
	JOIN entities e ON ef.entity_id = e.id
	JOIN groups g ON g.id = e.group_id
	JOIN devices d ON d.id = g.device_id
	WHERE d.id = @deviceId);

DELETE FROM entity_alarm
WHERE entity_id IN
	(SELECT e.id AS id from entities e
	JOIN groups g ON g.id = e.group_id
	JOIN devices d ON d.id = g.device_id
	WHERE d.id = @deviceId);
	
DELETE FROM entity_field
WHERE entity_id IN
	(SELECT e.id AS id from entities e
	JOIN groups g ON g.id = e.group_id
	JOIN devices d ON d.id = g.device_id
	WHERE d.id = @deviceId);		

DELETE FROM entities WHERE group_id IN
	(SELECT g.id AS id from  groups g 
	JOIN devices d ON d.id = g.device_id
	WHERE d.id = @deviceId);		

DELETE FROM groups WHERE device_id = @deviceId;

DELETE FROM devices WHERE id = @deviceId;

