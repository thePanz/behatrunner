Installation
================================================================================

*This module must be enabled via drush.*

`drush en -y behatrunner`

When first enabled, the module scan and registered the scenarios defined by the
currently enabled modules, run `drush bscan` to update and register them.

The module will auto-deregister the scenarios defined in the modules subsequently
disabled.

Commands
--------------------------------------------------------------------------------

- List the registered scenarios
  `drush bls`

- Scan enabled modules for new scenarios and features
  `drush bscan`

- Clear the list of registered scenarios
  `drush bclear`

- Run all the registered tests
  `drush brun`

- Run all the registered test for the given modules
  `drush brun --modules=my_module,my_other_module`
  
- Run all the registered test for a given tag (single tags supported)
  `drush brun --tags=theTag`

      
- Run all the registered test for a given module and a given tag
  `drush brun --modules=my_module --tags=theTag`  ( *multiple TAGs not currently supported: only the first TAG is used!* )


FAQS
================================================================================

How do I run javascript tests?
--------------------------------------------------------------------------------

To run javascript tests:

1. All javascript scenarios must be tagged with @javascript
2. You must have a javascript-enabled web session available.

By default, behatrunner is set to run all javascript tests with
Selenium2. This is dictated by the javascript_session key in public://behat.yml.
Therefore, a Selenium2 server must be running when the test is executed.

@see "How do I install Selenium2" for installation instructions.
@see http://docs.seleniumhq.org/download

How do I install Selenium2?
--------------------------------------------------------------------------------

1. Download Selenium2 standalone server:
   https://code.google.com/p/selenium/downloads/detail?name=selenium-server-standalone-2.39.0.jar&can=1&q=
2. Start the selenium server locally:
   java -jar path/to/selenium-server-standalone-2.39.0.jar

When I run tests, the output says "No scenarios, No steps"
--------------------------------------------------------------------------------

You've probably change a scenario since the last time that Behat Runner
registered it. To update Behat Runner's registration, run `drush bscan`.

How do I integrate this module with a continuous integration solution?
--------------------------------------------------------------------------------

**In Behat 3.x the JUnit support has not been merged into a stable release yet.
See: https://github.com/Behat/Behat/pull/676 **

Behat Runner writes the results of every test run to public://behatrunner. You
can configure your Jenkins or Travis CI jobs to look in this directory for the
JUnit-formatted XML output.

Error: Maximum nesting level of 100 reached
--------------------------------------------------------------------------------
This error is caused by XDebug, check: https://github.com/Behat/Behat/pull/730
for further details (this issue will be closed when a stable release of Behat
that includes the PR will be available)

Error: User not logged-in
--------------------------------------------------------------------------------
The DrupalExtension checks the "I am logged in" by lookinf for a "log-out" link
on the main page. Depending on your theme, this could not be always true.
DrupalExtension PR https://github.com/jhedstrom/drupalextension/pull/131 tackle
this issue, but at time of writing (2015-06-23) it is still pending and not
committed.

Error: Symfony\Component\Process\Exception\ProcessTimedOutException exception
--------------------------------------------------------------------------------
There can be two reasons for this exception: 

1. Selenium2 is not compatible with firefox versions above 46. Downgrade your 
   firefox to 46 or earlier. 
2. Your tests take too long to execute. You can set the timeout limit through 
   the behatrunner_process_timeout drupal variable, e.g. 
   `drush vset behatrunner_process_timeout 240` 
