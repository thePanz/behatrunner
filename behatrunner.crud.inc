<?php

/**
 * @param $modules
 * @param bool $idOnly
 * @return array
 */
function behatrunner_scenario_load_filtered($modules = array(), $tags = array(), $idOnly = false) {
    $query = new EntityFieldQuery;
    $query->entityCondition('entity_type', 'behatrunner_scenario');

    $modules = array_filter($modules);
    if (!empty($modules)) {
        $query->propertyCondition('module', $modules, 'IN');
    }

    $tags = array_filter($tags);
    if (!empty($tags)) {
        $query->propertyCondition('tags', current($tags), 'CONTAINS');
    }

    $result = $query->execute();

    if (empty($result) || !array_key_exists('behatrunner_scenario', $result)) {
        return array();
    }

    $ids = array_keys($result['behatrunner_scenario']);
    if ($idOnly) return $ids;

    return entity_load('behatrunner_scenario', $ids);
}


/**
 * @return int
 */
function behatrunner_scenario_count() {
    $query = 'SELECT COUNT(bsid) FROM {behatrunner_scenario}';
    $record = db_query($query)->fetchCol();
    return (int) current($record);
}

/**
 * @param $bsid
 * @return bool
 */
function behatrunner_scenario_delete($bsid) {
    entity_delete('behatrunner_scenario', $bsid);
}

/**
 * @param $bsids
 * @return bool
 */
function behatrunner_scenario_delete_multiple($bsids) {
    if (empty($bsids))
        return true;
    return entity_delete_multiple('behatrunner_scenario', $bsids);
}