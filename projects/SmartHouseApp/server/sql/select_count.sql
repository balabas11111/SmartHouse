/*
correct values 
1 device,
3 groups,
7 entities,
54 fields,
3 enabledValue,
3 grouppedId,
2 groupped name
*/

SELECT
	(SELECT COUNT(*) FROM devices) AS devicesCount,
	(SELECT COUNT(*) FROM groups) AS groupCount,
	(SELECT COUNT(*) FROM entities) AS entitiesCount,
	(SELECT COUNT(*) FROM entity_field) AS entityFieldCount,
	(SELECT COUNT(*) FROM entity_field_enabled_value) AS entityFieldEnabledValueCount,
	(SELECT COUNT(*) FROM entity_groupped_fields_ids) AS entity_groupped_id_Count,
	(SELECT COUNT(*) FROM entity_groupped_fields_names) AS entity_groupped_names_count