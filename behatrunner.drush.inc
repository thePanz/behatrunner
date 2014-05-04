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
  $items['behat-register-module'] = array(
    'description' => dt('Register Behat scenarios for a given module.'),
    'arguments' => array(
      'module' => 'Name of the module containing the scenarios. Required',
    ),
    'required-arguments' => TRUE,
    'examples' => array(
      'Standard example' => 'drush brm behatrunner',
    ),
    'aliases' => array('brm'),
  );
  $items['behat-run-tests'] = array(
    'description' => dt('Run the behat tests.'),
    'examples' => array(
      'Standard example' => 'drush behat-run-tests',
    ),
    'aliases' => array('brun'),
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
 * Register a module's scenarios.
 *
 * @param string $module
 *   The name of the module to register.
 */
function drush_behatrunner_behat_register_module($module) {
  module_load_include('inc', 'behatrunner', 'behatrunner');
  behatrunner_register_module($module);
}

/**
 * Run behat tests.
 */
function drush_behatrunner_behat_run_tests() {
  // TODO: Accept parameters.
  $results = behatrunner_execute_tests();
  // TODO: Format output.
  drush_print($results);
}
