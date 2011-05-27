<?php 
/**
 * Implementation of hook_drush_command().
 * 
 * @See drush_parse_command() for a list of recognized keys.
 *
 * @return
 *   An associative array describing your command(s).
 */
function drush_pull_drush_command() {
  $items = array();
  $items['git-pull'] = array(
    'description' => "Doing a git pull if all features are in default state.",
    //'drupal dependencies' => array('features'),
    'aliases' => array('pull'),
    'argument' => array(
    	'force' => 'Force the command',
    )
  );
  $items['git-push'] = array(
    'description' => "Doing a git push if all features are in default state.",
    //'drupal dependencies' => array('features'),
    'aliases' => array('push'),
  	'argument' => array(
    	'force' => 'Force the command',
    )
  );
  return $items;
}

/**
 * Implementation of hook_drush_help().
 */
function drush_pull_drush_help($section) {
  switch ($section) {
    case 'drush:git-pull':
      return dt("Doing a git pull if all features are in default state.");
    case 'drush:git-push':
      return dt("Doing a git push if all features are in default state.");
  }
}

/**
 * git pull.
 *
 * One of the problems combine features module w/ Git is, 
 * if the pulling from git occur before updating all features changes to code.
 * 
 * Well, let say we got some feature changes comming form the git pull, 
 * and and we got a feature in override state (local changes are not exported to code yet).
 * There two options heading us now:
 *  - Revert the feature and ignore all of the settings changes in the DB
 *  - Recreate the feature (export the local changes to code) and override some else's job.
 * 
 * This drush command is trying to help enforce the very basic procedures: 
 *"Alwayes update your feature to code before you pulling from git"
 *
 * @param type $force */

function drush_drush_pull_git_pull($force = '') {
  if (_git_ready() || ($force == 'force' && _confirm())) {
    if (drush_shell_exec('git pull')){
      $output = drush_shell_exec_output();
      foreach ($output as $line) {
        drush_print(dt($line));
      }
    }
    drush_print(dt("\nThe code base might changed, please check if your features need to\nrevert (drush fr) to deafult state before you continue working."));
    //exec('git pull 2>&1', $output);
    //TODO: check "drush_shell_exec('git pull')" and maybe "drush_shell_exec_output()"
  }
}

/**
 * git push.
 * @param type $force 
 */
function drush_drush_pull_git_push($force = '') {
  if (drush_shell_exec('git push')){
    $output = drush_shell_exec_output();
    foreach ($output as $line) {
      drush_print(dt($line));
    }
  }
    //exec('git push 2>&1', $output);
  //TODO: check "drush_shell_exec('git pull')" and maybe "drush_shell_exec_output()"
}

/**
 * check the features status
 * @return TRUE if all features are in default state.
 * @return FALSE if one or more features are in override state.
 */
function _git_ready() {
  module_load_include('inc', 'features', 'features.export');
  $overridden_modules = array();
  $need_review_modules = array();
  foreach (features_get_features(NULL, TRUE) as $key => $module) {
    switch (features_get_storage($module->name)) {
      case FEATURES_OVERRIDDEN:
      case FEATURES_NEEDS_REVIEW:
        $overridden_modules[] = $module->name;
        break;
    }
  }
  if (!empty($overridden_modules)) {
    drush_print(dt("\nThe following features are overridden: \n\n- !modules\n\nplease update (drush fu) or revert (drush fr) your features,\nBEFORE you continue with pulling new code.", array("!modules" => implode("\n- ", $overridden_modules))));
    return FALSE;
  }
  else {
    drush_print(dt("\nAll features are in default status, continue...\n"));
    return TRUE;
  }  
}

/**
 * confimation on forcing drush pull
 * @return TRUE on YES. 
 * Or aborting on NO
 */
function _confirm() {
    if (drush_confirm(dt('Do you really want to force and continue?'))) {
      return TRUE;
    }
    else {
      drush_die('Aborting.');
    }
}