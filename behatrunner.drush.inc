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

  $items['behat-list-scenarios'] = array(
    'description' => dt('List registered Behat scenarios.'),
    'examples' => array(
      'Standard example' => 'drush behat-list-scenarios',
    ),
    'aliases' => array('bls'),
  );
  $items['behat-scan-scenarios'] = array(
    'description' => dt('Scan modules and register Behat scenarios. If no argument is specified, all discoverable modules will be registered.'),
    'arguments' => array(
      'module' => '(optional) Name of the module containing the scenarios to be registered.',
    ),
    'examples' => array(
      'Discover and register' => 'drush bscan',
      'Register specific module' => 'drush bscan behatrunner',
    ),
    'aliases' => array('bscan', 'brm'),
    );
  $items['behat-clear-scenarios'] = array(
      'description' => dt('Clear register Behat scenarios. If no argument is specified, all scenarios will be removed.'),
      'arguments' => array(
          'module' => '(optional) Name of the module containing the scenarios to be removed.',
      ),
      'examples' => array(
          'Clear all scenarios' => 'drush bclear',
          'Clear scenarios from specific module' => 'drush blcear behatrunner',
      ),
      'aliases' => array('bclear'),
  );
  $items['behat-run-tests'] = array(
    'description' => dt('Run the behat tests.'),
    'options' => array(
      'formats' => array(
        'description' => 'The output formats',
        'example_value' => 'junit',
      ),
    ),
    'examples' => array(
      'Standard example' => 'drush behat-run-tests',
    ),
    'aliases' => array('brun'),
  );
  return $items;
}

/**
 * List known test locations.
 */
function drush_behatrunner_behat_list_scenarios() {
  $scenarios = entity_load('behatrunner_scenario');

  if ($scenarios) {
    $table = array();
    $table[] = array(t('Title'), t('Module'), t('Location'));
    $table[] = array('-----------', '-----------', '-----------');
    foreach ($scenarios as $scenario) {
      $table[] = array(
        $scenario->title,
        $scenario->module,
        $scenario->location);
    }
    drush_print_table($table);
  }
  else {
    drush_print(t('There are no test locations registered.'));
  }
}

/**
 * @param string $module
 */
function drush_behatrunner_behat_clear_scenarios($module = null) {
    module_load_include('inc', 'behatrunner', 'includes/behatrunner');
    $result = behatrunner_deregister_module_scenarios($module);
    if ($result === false) {
        drush_log('Error while removing scenarios!', 'error');
        return;
    }

    if (!empty($module)) {
        drush_log(dt('All scenarios of module "@module" removed', array('@module' => $module)), 'success');
    }
    else {
        drush_log('All scenarios removed', 'success');
    }
}

/**
 * Register the scenarios of the given module name, if empty all enabled modules
 *  will be scanned for scenarios.
 *
 * @param string $moduleName The name of the module to register.
 */
function drush_behatrunner_behat_scan_scenarios($moduleName = NULL) {
  module_load_include('inc', 'behatrunner', 'includes/behatrunner');

  $modules = array();
  $noticeNoScenarios = false;

  if (!empty($moduleName)) {
      $modules[] = $moduleName;
      $noticeNoScenarios = true;
  }
  else {
      $modules = module_list();
  }

  foreach($modules as $module) {
      $registered = behatrunner_features_scan_module($module);
      if ($noticeNoScenarios && !$registered) {
          drush_log(dt('No scenarios found in module "@module"', array('@module' => $module)), 'warning');
      }
      else if ($registered) {
          drush_log(dt('Added scenarios for module "@module"', array('@module' => $module)), 'success');
      }
  }
}


/**
 * Run behat tests.
 */
function drush_behatrunner_behat_run_tests() {
  module_load_include('inc', 'behatrunner', 'includes/behatrunner');

  // If there are no registrations, run auto discovery.
  if (!behatrunner_scenarios_exist()) {
    behatrunner_discover_modules();
  }

  $formats = array();
  if ($formats_option = drush_get_option('formats', 'pretty')) {
    $formats = explode(',', $formats_option);
  }

  // TODO: Accept parameters.
  module_load_include('inc', 'behatrunner', 'includes/behatrunner');
  $results = behatrunner_execute_tests(array(), array(), $formats);
}
