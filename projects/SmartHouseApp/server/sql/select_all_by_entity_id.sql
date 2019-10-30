SELECT * FROM entities e
JOIN groups g ON g.id = e.group_id
JOIN devices d ON d.id = g.device_id
WHERE e.id = 11smarthouse_db