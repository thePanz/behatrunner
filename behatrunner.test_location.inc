<?php

/**
 * @file
 * behatrunner_test_location entity code.
 */

/**
 * Implements hook_entity_info().
 */
function behatrunner_test_location_entity_info() {
  return array(
    'behatrunner_test_location' => array(
      'label' => t('Behat Runner Test location'),
      'entity class' => 'BehatRunnerTestLocation',
      'controller class' => 'BehatRunnerTestLocationController',
      'base table' => 'behatrunner_test_location',
      'uri callback' => 'behatrunner_test_location_uri',
      'fieldable' => TRUE,
      'entity keys' => array(
        'id' => 'btid',
      ),
      'static cache' => TRUE,
      'module' => 'behatrunner_test_location',
      'views controller class' => 'EntityDefaultViewsController',
    ),
  );
}


/**
 * Implements hook_entity_property_info().
 */
function behatrunner_test_location_entity_property_info() {
  $info = array();
  $properties = &$info['behatrunner_test_location']['properties'];

  $properties['btid'] = array(
    'label' => 'Behat Runner Test Location ID',
    'description' => t('The unique ID for the behatrunner_test_location.'),
    'type' => 'integer',
    'schema field' => 'btid',
  );
  $properties['name'] = array(
    'label' => 'Name',
    'description' => t('The name for this test location.'),
    'type' => 'varchar',
    'schema field' => 'module',
  );
  $properties['location'] = array(
    'label' => 'Location',
    'description' => t('The location of the tests, relative to the Drupal base.'),
    'type' => 'varchar',
    'schema field' => 'location',
  );
  $properties['is_module'] = array(
    'label' => 'Is module',
    'description' => t('Whether or not this location is associated with a module.'),
    'type' => 'integer',
    'schema field' => 'is_module',
  );
}


/**
 * Access callback.
 */
function behatrunner_test_location_access($op, $projectqa_phploc_result, $account = NULL, $entity_type = NULL) {
  global $user;
  if (!isset($account)) {
    $account = $user;
  }
  switch ($op) {
    case 'create':
      return user_access('administer behatrunner_test_location entities', $account)
          || user_access('create behatrunner_test_location entities', $account);
    case 'view':
      return user_access('administer behatrunner_test_location entities', $account)
          || user_access('view behatrunner_test_location entities', $account);
  }
}

/**
 * Implements hook_permission().
 */
function behatrunner_test_location_permission() {
  $permissions = array(
    'create behatrunner_test_location entities' => array(
      'title' => t('Create behatrunner_test_location'),
      'description' => t('Allows users to create behatrunner_test_location.'),
      'restrict access' => TRUE,
    ),
    'view behatrunner_test_location entities' => array(
      'title' => t('View behatrunner_test_location'),
      'description' => t('Allows users to view behatrunner_test_location.'),
      'restrict access' => TRUE,
    ),
  );
  return $permissions;
}


/**
 * BehatRunnerTestLocation class.
 */
class BehatRunnerTestLocation extends Entity {

  /**
   * The default label.
   *
   * @return int
   *   behatrunner_test_location btid
   */
  protected function defaultLabel() {
    return $this->btid;
  }

  /**
   * The default URI.
   *
   * @return array
   *   An array containing the path to the behatrunner_test_location
   */
  protected function defaultUri() {
    return array('path' => 'behatrunner_test_location/' . $this->identifier());
  }
}


/**
 * BehatRunnerTestLocationController class
 */
class BehatRunnerTestLocationController extends EntityAPIController {}
