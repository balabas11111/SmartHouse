/*DELETE FROM entity_alarm_entity_field WHERE id=13;*/

SELECT * FROM entity_alarm ea
JOIN entity_alarm_entity_field eaef ON ea.id = eaef.entity_alarm_id
