<?php

/**
 * @file
 * Utility functions for Behat Runner.
 */

/*
 * Registers the locations of all scenarios for a given module.
 *
 * @param string $name
 *   The machine name of a module.
 */
function behatrunner_register_module($module) {
  $registered = behatrunner_module_is_registered($module);
  // If this module has been registered, remove its scenario registrations.
  if ($registered) {
    behatrunner_deregister_module_scenarios($module);
  }

  // Discover all features for the given module.
  $features = behatrunner_discover_module_features($module);
  foreach ($features as $feature) {
    // Register all scenarios for each feature.
    $status = behatrunner_register_feature_scenarios($feature);
  }
}

/**
 * Discovers all features for a given module.
 *
 * @param string $module
 *   The machine name of a module.
 *
 * @return array
 *   An associative array of features, keyed by filename.
 */
function behatrunner_discover_module_features($module) {
  $features_path = 'features';

  // Allow modules to specify a custom path for their features directory.
  $behat_info = module_invoke_all('behat_info');
  if (!empty($behat_info[$module]['paths']['features'])) {
    $features_path = $behat_info[$module]['']['features'];
  }

  $system_path = DRUPAL_ROOT . '/' . drupal_get_path('module', $module) . '/' . $features_path;
  $feature_files = file_scan_directory($system_path, '/.*\.feature$/', array('recurse' => TRUE));
  $parser = behatrunner_get_parser();

  $features = array();
  foreach ($feature_files as $feature_file) {
    $feature = $parser->parse(file_get_contents($feature_file->uri));
    $feature->setFile($feature_file->uri);
    $feature->drupalModule = $module;
    $features[$feature_file->filename] = $feature;
  }

  return $features;
}

/**
 * [behatrunner_register_feature_scenarios description]
 * @param  [type] $feature [description]
 * @return [type]          [description]
 */
function behatrunner_register_feature_scenarios($feature) {
  $feature_location = behatrunner_convert_absolute_to_relative_path($feature->getFile());
  $scenarios = $feature->getScenarios();

  foreach ($scenarios as $scenario) {
    $data['title'] = $scenario->getTitle();
    $data['location'] = $feature_location . ':' . $scenario->getLine();
    $data['module'] = $feature->drupalModule;
    $entity = entity_create('behatrunner_scenario', $data);
    $entity->save();
  }
}

/**
 * Returns a Behat Parser object for parsing Gherkin.
 *
 * @return object
 *   An Behat\Gherkin\Parser object initialized with default English keywords.
 */
function behatrunner_get_parser() {
  $keywords = new Behat\Gherkin\Keywords\ArrayKeywords(array(
    'en' => array(
        'feature'          => 'Feature',
        'background'       => 'Background',
        'scenario'         => 'Scenario',
        'scenario_outline' => 'Scenario Outline|Scenario Template',
        'examples'         => 'Examples|Scenarios',
        'given'            => 'Given',
        'when'             => 'When',
        'then'             => 'Then',
        'and'              => 'And',
        'but'              => 'But'
    ),
  ));
  $lexer  = new Behat\Gherkin\Lexer($keywords);
  $parser = new Behat\Gherkin\Parser($lexer);

  return $parser;
}

/**
 * [behatrunner_get_file_absolute_path description]
 * @param  [type] $relative_path [description]
 * @return [type]                [description]
 */
function behatrunner_convert_relative_to_absolute_path($relative_path) {
  return DRUPAL_ROOT . '/' . $relative_path;
}

/**
 * [behatrunner_get_file_absolute_path description]
 * @param  [type] $relative_path [description]
 * @return [type]                [description]
 */
function behatrunner_convert_absolute_to_relative_path($absolute_path) {
  return str_replace(DRUPAL_ROOT . '/', '', $absolute_path);
}

/**
 * [behatrunner_module_is_registered description]
 * @param  [type] $module [description]
 * @return [type]         [description]
 */
function behatrunner_module_is_registered($module) {
  $query = 'SELECT bsid FROM {behatrunner_scenario}';
  $query .= ' WHERE module=:module LIMIT 1';
  $result = db_query($query, array(':module' => $module));
  $record = $result->fetchObject();

  return (boolean) $record;
}

/**
 * Looks up all registered scenarios for the given module.
 *
 * @param string $module
 *   The module name.
 *
 * @return array
 *   An array of locations for registered scenarios for the given module, keyed
 *   by entitiy id.
 */
function behatrunner_get_module_scenario_registrations($module) {
  $query = 'SELECT bsid, location FROM {behatrunner_scenario}';
  $query .= ' WHERE module=:module';
  $result = db_query($query, array(':module' => $module));
  $registrations = $result->fetchAllKeyed(0, 1);

  return $registrations;
}

/**
 * De-registers a test location.
 *
 * @param int $bsid
 *   The id for this location.
 */
function behatrunner_deregister_scenario($bsid) {
  entity_delete('behatrunner_scenario', $bsid);
}


/**
 * De-registers all scenario registrations for a given module.
 *
 * @param string $module
 *   The name for this location.
 *
 * @return boolean
 *   FALSE if the given entity type isn't compatible to the CRUD API.
 */
function behatrunner_deregister_module_scenarios($module) {
  $scenario_registrations = behatrunner_get_module_scenario_registrations($module);
  $entity_ids = array_keys($scenario_registrations);

  return entity_delete_multiple('behatrunner_scenario', $entity_ids);
}


/**
 * Retrieves all known test locations.
 *
 * @return array
 *   An array of location objects.
 */
function behatrunner_get_scenario_registrations() {
  $query = 'SELECT * FROM {behatrunner_scenario}';
  $result = db_query($query);
  $registrations = $result->fetchAll();

  return $registrations;
}

/**
 * Walk the known locations and discover tests to run.
 */
function behatrunner_discover_tests() {
  // Get all known test locations.
  $registrations = behatrunner_get_scenario_registrations();

  foreach ($registrations as $registration) {
    // TODO: Discover tests, record tags.
  }
}

/**
 * Execute Behat tests.
 *
 * @param array $location_filters
 *   An array of locations to filter out. If empty, all tests will be run.
 *
 * @param array $tag_filters
 *   An array of tags to filter out. If empty, all tests will be run.
 */
function behatrunner_execute_tests($location_filters = array(), $tag_filters = array()) {
  composer_manager_register_autoloader();
  $registrations = behatrunner_get_scenario_registrations();

  // TODO: Replace exec() logic with direct PHP calls.
  $results = array();

  // Construct tag filter params.
  $tags = '';
  if (count($tag_filters) > 0) {
    $tags = ' --tags "~@';
    $tags .= implode($tag_filters, '&&~@');
    $tags .= '"';
  }

  $results = '';
  foreach ($registrations as $registration) {
    // Skip locations listed in the location filter.
    if (!in_array($registration->location, $location_filters)) {
      // Construct the command.
      $vendor = composer_manager_vendor_dir();
      $command = $vendor . '/bin/behat';
      $command .= $tags;
      $command .= ' ' . DRUPAL_ROOT . '/' . $registration->location;
      $command .= ' --config ' . DRUPAL_ROOT . '/' . drupal_get_path('module', 'behatrunner') . '/behat.yml';
      // drush_print($command);
      $results .= shell_exec($command);
    }
  }

  return $results;
}

