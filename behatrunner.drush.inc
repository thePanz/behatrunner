<?php
/**
 * @file
 * Drush integration implementation
 */

/**
 * Implements hook_drush_help().
 */
function behatrunner_drush_help($command) {
  switch ($command) {
    case 'drush:br':
      return dt('Run Behat Runner');
  }
}

/**
 * Implements hook_drush_command().
 */
function behatrunner_drush_command() {
  $items = array();
  $items['behat-run'] = array(
    'description' => dt('Run Behat tests.'),
    'examples' => array(
      'Standard example' => 'drush behat-run',
    ),
    'aliases' => array('br'),
  );
  $items['behat-list-locations'] = array(
    'description' => dt('List registered Behat test locations.'),
    'examples' => array(
      'Standard example' => 'drush behat-list-locations',
    ),
    'aliases' => array('bll'),
  );
  $items['behat-register-location'] = array(
    'description' => dt('Register a Behat test location.'),
    'arguments' => array(
      'name' => 'Name of the location. Required.',
      'module' => 'Name of the module containing these tests. Optional.',
      'location' => 'The location of the tests, relative to the Drupal base. Required.',
    ),
    'examples' => array(
      'Standard example' => 'drush behat-register-location',
    ),
    'aliases' => array('brl'),
  );
  return $items;
}


/**
 * Run Behat tests.
 */
function drush_behatrunner_behatrun($arg1 = NULL) {
  // TODO:
}


/**
 * List known test locations.
 */
function drush_behatrunner_behat_list_locations($arg1 = NULL) {
  $registrations = behatrunner_get_test_locations();

  if ($registrations) {
    $table = array();
    $table[] = array(t('Name'), t('Module'), t('Location'));
    $table[] = array('-----------', '-----------', '-----------');
    foreach ($registrations as $registration) {
      $table[] = array(
        $registration->name,
        $registration->module,
        $registration->location);
    }
    drush_print_table($table);
  }
  else {
    drush_print(t('There are no test locations registered.'));
  }
}

/**
 * Register a test location.
 *
 * @param string $name
 *   The name of the location.
 *
 * @param string $module
 *   The module (if any) containing these tests.
 *
 * @param string $location
 *   The location of the tests.
 */
function drush_behatrunner_behat_register_location($name, $module, $location) {

  behatrunner_register_location($name, $module, $location);
}
