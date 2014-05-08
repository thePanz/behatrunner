<?php

/**
 * @file
 * Administrative forms for Behat Runner.
 */

/**
 * Returns the administrative configure form for Behat Runner.
 */
function behatrunner_admin_form(&$form_state) {
  $form = array();

  $form['behatrunner_output'] = array(
    '#type' => 'select',
    '#title' => t('Output destination'),
    '#description' => t('Outputting to the Drupal database allows tests results to be viewed via Views.'),
    '#options' => array(
      'database' => t('Drupal database and file system'),
      'files' => t('File system'),
    ),
    '#default_value' => variable_get('behatrunner_format'),
  );

  return $form;
}

