SET @groupName = 'settings';

SELECT * FROM groups g WHERE g.name='settings';

DELETE FROM entity_field_value 
WHERE entity_field_id in
	(SELECT ef.id AS id from entity_field ef 
	JOIN entities e ON ef.entity_id = e.id
	JOIN groups g ON g.id = e.group_id
	WHERE g.id IN (SELECT id FROM groups g WHERE g.name='settings'));
	
DELETE FROM entity_field_enabled_value
WHERE entity_field_id IN
	(SELECT ef.id AS id from entity_field ef 
	JOIN entities e ON ef.entity_id = e.id
	JOIN groups g ON g.id = e.group_id
	WHERE g.id IN (SELECT id FROM groups g WHERE g.name='settings'));

DELETE FROM entity_groupped_fields_ids
WHERE entity_id IN
	(SELECT e.id AS id from entities e
	JOIN groups g ON g.id = e.group_id
	WHERE g.id IN (SELECT id FROM groups g WHERE g.name='settings'));
	
DELETE FROM entity_groupped_fields_names
WHERE entity_id IN
	(SELECT e.id AS id from entities e
	JOIN groups g ON g.id = e.group_id
	WHERE g.id IN (SELECT id FROM groups g WHERE g.name='settings'));
	
DELETE FROM entity_alarm_entity_field
WHERE entity_field_id IN
	(SELECT ef.id AS id from entity_field ef 
	JOIN entities e ON ef.entity_id = e.id
	JOIN groups g ON g.id = e.group_id
	WHERE g.id IN (SELECT id FROM groups g WHERE g.name='settings'));

DELETE FROM entity_alarm
WHERE entity_id IN
	(SELECT e.id AS id from entities e
	JOIN groups g ON g.id = e.group_id
	WHERE g.id IN (SELECT id FROM groups g WHERE g.name='settings'));
	
DELETE FROM entity_field
WHERE entity_id IN
	(SELECT e.id AS id from entities e
	JOIN groups g ON g.id = e.group_id
	WHERE g.id IN (SELECT id FROM groups g WHERE g.name='settings'));		

DELETE FROM entities WHERE group_id IN
	(SELECT g.id AS id from  groups g 
	WHERE g.id IN (SELECT id FROM groups g WHERE g.name='settings'));		

DELETE FROM groups WHERE name='settings';

SELECT * FROM groups WHERE NAME='settings';
