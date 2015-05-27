<?php

use Drupal\DrupalExtension\Context\DrupalSubContextInterface;
use Drupal\DrupalDriverManager;

class FeatureContext implements DrupalSubContextInterface {

    public function __construct(DrupalDriverManager $drupal) {
    }
}
