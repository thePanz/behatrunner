<?php

/**
 * @file
 * API Implementation examples for Behat Runner.
 */

/**
 * Provides information about implementing module's behat features.
 */
function hook_behat_info() {
  $info = array();

  $info['my_module_name'] = array(
    'paths' => array(
      // Specify the directory in which .feature files are located, relative
      // to the module directory.
      'features' => 'features',
    ),
  );

  return $info;
}
